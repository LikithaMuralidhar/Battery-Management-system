#include <msp430.h>
#include <stdio.h>
#include <string.h>


#define USE_UART 1
#define USE_I2C_SLAVE 1
#define USE_CAN 1

// Configuration
#define UART_BAUD 115200
#define I2C_SLAVE_ADDR 0x55
#define CAN_BITRATE 500000

typedef struct {
    uint16_t cellVoltages[16];
    uint32_t packVoltage;
    int32_t current;
    int16_t temperatures[4];
    uint8_t soc;
    uint8_t numCells;
    uint8_t protectionFlags;
    uint8_t fetStatus;
    uint16_t balancingStatus;
    uint32_t timestamp;
} SharedBMSData_t;

SharedBMSData_t sharedData = {0};

// I2C Slave register map
uint8_t i2cRegisters[128];
volatile uint8_t i2cRegisterAddress = 0;

void MSP430_I2C_Master_Init(void) {
    // Configure I2C pins (P1.6=SDA, P1.7=SCL for MSP430G2553)
    P1SEL |= BIT6 + BIT7;
    P1SEL2 |= BIT6 + BIT7;
    
    UCB0CTL1 |= UCSWRST;          
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC; 
    UCB0CTL1 = UCSSEL_2 + UCSWRST;  
    UCB0BR0 = 12;                     
    UCB0BR1 = 0;
    UCB0CTL1 &= ~UCSWRST;             // Clear SW reset, resume operation
}

bool I2C_Write(uint8_t addr, uint8_t *data, uint8_t length) {
    UCB0I2CSA = addr;                 // Set slave address
    UCB0CTL1 |= UCTR + UCTXSTT;       // I2C TX, start condition
    
    for (uint8_t i = 0; i < length; i++) {
        while (!(IFG2 & UCB0TXIFG));  // Wait for TX buffer ready
        UCB0TXBUF = data[i];           // Transmit data
    }
    
    while (UCB0CTL1 & UCTXSTP);      
    UCB0CTL1 |= UCTXSTP;             
    
    return true;
}

bool I2C_Read(uint8_t addr, uint8_t reg, uint8_t *data, uint8_t length) {
    
    UCB0I2CSA = addr;
    UCB0CTL1 |= UCTR + UCTXSTT;       // I2C TX, start
    while (!(IFG2 & UCB0TXIFG));
    UCB0TXBUF = reg;
    while (!(IFG2 & UCB0TXIFG));
    
    // Read data
    UCB0CTL1 &= ~UCTR;                // I2C RX
    UCB0CTL1 |= UCTXSTT;              // Repeated start
    
    for (uint8_t i = 0; i < length; i++) {
        if (i == length - 1) {
            UCB0CTL1 |= UCTXSTP;      // Send stop before last byte
        }
        while (!(IFG2 & UCB0RXIFG));
        data[i] = UCB0RXBUF;
    }
    
    return true;
}

void MSP430_UART_Init(void) {
    // Configure UART pins (P1.1=RX, P1.2=TX)
    P1SEL |= BIT1 + BIT2;
    P1SEL2 |= BIT1 + BIT2;
    
    UCA0CTL1 |= UCSSEL_2;             // SMCLK
    UCA0BR0 = 104;                    // 1MHz/115200 = 8.68
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS0;                // Modulation
    UCA0CTL1 &= ~UCSWRST;             // Initialize USCI state machine
    IE2 |= UCA0RXIE;                  // Enable USCI_A0 RX interrupt
}

void UART_SendChar(char c) {
    while (!(IFG2 & UCA0TXIFG));      // Wait for TX buffer ready
    UCA0TXBUF = c;
}

void UART_SendString(const char *str) {
    while (*str) {
        UART_SendChar(*str++);
    }
}

void UART_Printf(const char *format, ...) {
    char buffer[128];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    UART_SendString(buffer);
}

void UART_SendBMSData(void) {
    UART_Printf("SOC: %d%%\r\n", sharedData.soc);
    UART_Printf("Pack Voltage: %ld.%03ldV\r\n", 
                sharedData.packVoltage / 1000,
                sharedData.packVoltage % 1000);
    UART_Printf("Current: %s%ld.%03ldA\r\n",
                sharedData.current >= 0 ? "+" : "",
                sharedData.current / 1000,
                abs(sharedData.current) % 1000);
    
    UART_SendString("\r\nCell Voltages:\r\n");
    for (uint8_t i = 0; i < sharedData.numCells; i++) {
        UART_Printf("  Cell %2d: %d.%03dV", i+1,
                    sharedData.cellVoltages[i] / 1000,
                    sharedData.cellVoltages[i] % 1000);
        if (sharedData.balancingStatus & (1 << i)) {
            UART_SendString(" [BAL]");
        }
        UART_SendString("\r\n");
    }
    
    UART_SendString("\r\nTemperatures:\r\n");
    for (uint8_t i = 0; i < 4; i++) {
        UART_Printf("  Sensor %d: %d.%d°C\r\n", i+1,
                    sharedData.temperatures[i] / 10,
                    abs(sharedData.temperatures[i]) % 10);
    }
  
}

void UpdateSharedData(BQ76952_Data_t *bqData) {
    
    memcpy(sharedData.cellVoltages, bqData->cellVoltages, sizeof(sharedData.cellVoltages));
    sharedData.packVoltage = bqData->stackVoltage;
    sharedData.current = bqData->current;
    memcpy(sharedData.temperatures, bqData->temperatures, sizeof(sharedData.temperatures));
    sharedData.soc = bqData->soc;
    sharedData.numCells = bqData->numCells;
    
    
    sharedData.protectionFlags = 0;
    sharedData.protectionFlags |= (bqData->protectionStatus.overvoltage << 0);
    sharedData.protectionFlags |= (bqData->protectionStatus.undervoltage << 1);
    sharedData.protectionFlags |= (bqData->protectionStatus.overcurrentCharge << 2);
    sharedData.protectionFlags |= (bqData->protectionStatus.overcurrentDischarge << 3);
    sharedData.protectionFlags |= (bqData->protectionStatus.overtemperature << 4);
    sharedData.protectionFlags |= (bqData->protectionStatus.undertemperature << 5);
    sharedData.protectionFlags |= (bqData->protectionStatus.shortCircuit << 6);
    

    sharedData.fetStatus = 0;
    sharedData.fetStatus |= (bqData->fetStatus.chargeFET << 0);
    sharedData.fetStatus |= (bqData->fetStatus.dischargeFET << 1);
    
    // Balancing
    sharedData.balancingStatus = bqData->balancingCells;
}


void main(void) {
    WDTCTL = WDTPW | WDTHOLD;        
    
    // Configure clock
    BCSCTL1 = CALBC1_1MHZ;            
    DCOCTL = CALDCO_1MHZ;
    
 
    MSP430_I2C_Master_Init();
    MSP430_UART_Init();
    
  
    BQ76952_Init();
  
    __enable_interrupt();    
    
  
    while (1) {
       
        BQ76952_UpdateAll(&bqData);
        
      
        UpdateSharedData(&bqData);
        
    
        static uint16_t uartCounter = 0;
        if (++uartCounter >= 10) {
            uartCounter = 0;
            UART_SendBMSData();
        }
          
        __delay_cycles(100000);
    }
}
