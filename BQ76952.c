
#include <msp430.h>
#include <string.h>

// I2C Communication Functions (platform specific)
extern bool I2C_Write(uint8_t addr, uint8_t *data, uint8_t length);
extern bool I2C_Read(uint8_t addr, uint8_t reg, uint8_t *data, uint8_t length);

// Global data
BQ76952_Data_t bqData = {0};


void BQ76952_Init(void) {
    uint8_t data[2];
    
    // Wait for device to be ready
    __delay_cycles(100000);  // 100ms delay
    
    // Read device number to verify communication
    if (BQ76952_SubcommandRead(BQ76952_SUBCMD_DEVICE_NUMBER, data, 2)) {
        // Device is responding
    }
    
    // Enable both FETs
    BQ76952_EnableFET(BQ76952_FET_BOTH_ENABLE);
    
    // Set number of cells
    bqData.numCells = MAX_CELLS;
    
    // Initialize other parameters
    bqData.soc = 50;
    bqData.cycleCount = 0;
}


bool BQ76952_DirectRead(uint8_t reg, uint8_t *data, uint8_t length) {
    return I2C_Read(BQ76952_I2C_ADDR, reg, data, length);
}


bool BQ76952_DirectWrite(uint8_t reg, uint8_t *data, uint8_t length) {
    uint8_t txData[length + 1];
    txData[0] = reg;
    memcpy(&txData[1], data, length);
    return I2C_Write(BQ76952_I2C_ADDR, txData, length + 1);
}


bool BQ76952_SubcommandRead(uint16_t subcmd, uint8_t *data, uint8_t length) {
    uint8_t txData[4];
    
    // Write subcommand to 0x3E and 0x3F
    txData[0] = 0x3E;
    txData[1] = subcmd & 0xFF;        // Low byte
    txData[2] = (subcmd >> 8) & 0xFF; // High byte
    
    if (!I2C_Write(BQ76952_I2C_ADDR, txData, 3)) {
        return false;
    }
    
    __delay_cycles(10000); // Wait for command to execute
    
    // Read response from 0x40
    return I2C_Read(BQ76952_I2C_ADDR, 0x40, data, length);
}


bool BQ76952_SubcommandWrite(uint16_t subcmd, uint8_t *data, uint8_t length) {
    uint8_t txData[32];
    
    txData[0] = 0x3E;
    txData[1] = subcmd & 0xFF;
    txData[2] = (subcmd >> 8) & 0xFF;
    
    if (data && length > 0) {
        memcpy(&txData[3], data, length);
    }
    
    return I2C_Write(BQ76952_I2C_ADDR, txData, 3 + length);
}


void BQ76952_ReadCellVoltages(BQ76952_Data_t *data) {
    uint8_t buffer[2];
    
    for (uint8_t i = 0; i < data->numCells; i++) {
        // Cell voltages start at 0x14 and are 2 bytes apart
        if (BQ76952_DirectRead(BQ76952_CELL1_VOLTAGE + (i * 2), buffer, 2)) {
            // BQ76952 reports voltage in mV (little endian)
            data->cellVoltages[i] = buffer[0] | (buffer[1] << 8);
        }
    }
}


void BQ76952_ReadStackVoltage(BQ76952_Data_t *data) {
    uint8_t buffer[2];
    
    if (BQ76952_DirectRead(BQ76952_STACK_VOLTAGE, buffer, 2)) {
        data->stackVoltage = (buffer[0] | (buffer[1] << 8)) * 10; // Convert to mV
    }
}

void BQ76952_ReadCurrent(BQ76952_Data_t *data) {
    uint8_t buffer[2];
    
    if (BQ76952_DirectRead(BQ76952_CC2_CURRENT, buffer, 2)) {
        // Current in mA (signed 16-bit)
        int16_t raw = (int16_t)(buffer[0] | (buffer[1] << 8));
        data->current = raw;  // BQ76952 reports in mA
    }
}

void BQ76952_ReadTemperatures(BQ76952_Data_t *data) {
    uint8_t buffer[2];
    
  
    if (BQ76952_DirectRead(BQ76952_TS1_TEMPERATURE, buffer, 2)) {
        // Temperature in 0.1K units, need to convert to 0.1°C
        uint16_t temp_k = buffer[0] | (buffer[1] << 8);
        data->temperatures[0] = (int16_t)(temp_k - 2731);  // K to °C * 10
    }
    
   
    if (BQ76952_DirectRead(BQ76952_TS2_TEMPERATURE, buffer, 2)) {
        uint16_t temp_k = buffer[0] | (buffer[1] << 8);
        data->temperatures[1] = (int16_t)(temp_k - 2731);
    }
    
 
    if (BQ76952_DirectRead(BQ76952_TS3_TEMPERATURE, buffer, 2)) {
        uint16_t temp_k = buffer[0] | (buffer[1] << 8);
        data->temperatures[2] = (int16_t)(temp_k - 2731);
    }
    
   
    if (BQ76952_DirectRead(BQ76952_INT_TEMPERATURE, buffer, 2)) {
        uint16_t temp_k = buffer[0] | (buffer[1] << 8);
        data->temperatures[3] = (int16_t)(temp_k - 2731);
    }
}


void BQ76952_ReadProtectionStatus(BQ76952_Data_t *data) {
    uint8_t safetyA, safetyB, safetyC;
    uint8_t batteryStatus;
    
    // Read safety status registers
    BQ76952_DirectRead(BQ76952_SAFETY_STATUS_A, &safetyA, 1);
    BQ76952_DirectRead(BQ76952_SAFETY_STATUS_B, &safetyB, 1);
    BQ76952_DirectRead(BQ76952_SAFETY_STATUS_C, &safetyC, 1);
    BQ76952_DirectRead(BQ76952_BATTERY_STATUS, &batteryStatus, 1);
    
    // Parse protection status
    data->protectionStatus.overvoltage = (safetyA & 0x01) != 0;
    data->protectionStatus.undervoltage = (safetyA & 0x02) != 0;
    data->protectionStatus.overtemperature = (safetyB & 0x01) != 0;
    data->protectionStatus.undertemperature = (safetyB & 0x02) != 0;
    data->protectionStatus.overcurrentCharge = (safetyC & 0x01) != 0;
    data->protectionStatus.overcurrentDischarge = (safetyC & 0x02) != 0;
    data->protectionStatus.shortCircuit = (safetyC & 0x04) != 0;
    
    // FET status from battery status register
    data->fetStatus.chargeFET = (batteryStatus & 0x01) != 0;
    data->fetStatus.dischargeFET = (batteryStatus & 0x02) != 0;
}


void BQ76952_EnableFET(uint8_t fetMask) {
    uint8_t data = fetMask;
    BQ76952_SubcommandWrite(BQ76952_SUBCMD_FET_ENABLE, &data, 1);
}


void BQ76952_DisableFET(uint8_t fetMask) {
    uint8_t data = fetMask;
    BQ76952_SubcommandWrite(BQ76952_SUBCMD_FET_DISABLE, &data, 1);
}


void BQ76952_EnableBalancing(uint16_t cellMask) {
    uint8_t data[2];
    data[0] = cellMask & 0xFF;
    data[1] = (cellMask >> 8) & 0xFF;
    BQ76952_SubcommandWrite(BQ76952_SUBCMD_CB_ACTIVE_CELLS, data, 2);
    bqData.balancingCells = cellMask;
}


void BQ76952_DisableBalancing(void) {
    uint8_t data[2] = {0, 0};
    BQ76952_SubcommandWrite(BQ76952_SUBCMD_CB_ACTIVE_CELLS, data, 2);
    bqData.balancingCells = 0;
}


uint8_t BQ76952_CalculateSOC(BQ76952_Data_t *data) {
    // Voltage-based SOC estimation
    uint32_t avgVoltage = data->stackVoltage / data->numCells;
    
    const uint16_t MAX_CELL_V = 4200;  // mV
    const uint16_t MIN_CELL_V = 2800;  // mV
    
    if (avgVoltage >= MAX_CELL_V) return 100;
    if (avgVoltage <= MIN_CELL_V) return 0;
    
    uint8_t soc = ((avgVoltage - MIN_CELL_V) * 100) / (MAX_CELL_V - MIN_CELL_V);
    return soc;
}


void BQ76952_UpdateAll(BQ76952_Data_t *data) {
    BQ76952_ReadCellVoltages(data);
    BQ76952_ReadStackVoltage(data);
    BQ76952_ReadCurrent(data);
    BQ76952_ReadTemperatures(data);
    BQ76952_ReadProtectionStatus(data);
    data->soc = BQ76952_CalculateSOC(data);
    
    // Auto cell balancing logic
    if (data->current > 100) {  // Only when charging
        uint16_t minV = 5000;
        uint16_t maxV = 0;
        
        for (uint8_t i = 0; i < data->numCells; i++) {
            if (data->cellVoltages[i] < minV) minV = data->cellVoltages[i];
            if (data->cellVoltages[i] > maxV) maxV = data->cellVoltages[i];
        }
        
        if (maxV - minV > 50) {  // Imbalance > 50mV
            uint16_t balanceMask = 0;
            for (uint8_t i = 0; i < data->numCells; i++) {
                if (data->cellVoltages[i] > minV + 30) {
                    balanceMask |= (1 << i);
                }
            }
            BQ76952_EnableBalancing(balanceMask);
            data->protectionStatus.cellImbalance = true;
        } else {
            BQ76952_DisableBalancing();
            data->protectionStatus.cellImbalance = false;
        }
    }
}
