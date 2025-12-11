
#ifndef BQ76952_H
#define BQ76952_H

#include <stdint.h>
#include <stdbool.h>

// BQ76952 I2C Address
#define BQ76952_I2C_ADDR 0x08  

// BQ76952 Command Types
#define BQ76952_CMD_DIRECT     0x00 
#define BQ76952_CMD_SUBCOMMAND 0x3E  


#define BQ76952_CONTROL_STATUS    0x00
#define BQ76952_SAFETY_ALERT_A    0x02
#define BQ76952_SAFETY_STATUS_A   0x03
#define BQ76952_SAFETY_ALERT_B    0x04
#define BQ76952_SAFETY_STATUS_B   0x05
#define BQ76952_SAFETY_ALERT_C    0x06
#define BQ76952_SAFETY_STATUS_C   0x07
#define BQ76952_BATTERY_STATUS    0x12
#define BQ76952_ALARM_STATUS      0x62
#define BQ76952_ALARM_ENABLE      0x66


#define BQ76952_CELL1_VOLTAGE     0x14
#define BQ76952_CELL2_VOLTAGE     0x16
#define BQ76952_CELL3_VOLTAGE     0x18
#define BQ76952_CELL4_VOLTAGE     0x1A
#define BQ76952_CELL5_VOLTAGE     0x1C
#define BQ76952_CELL6_VOLTAGE     0x1E
#define BQ76952_CELL7_VOLTAGE     0x20
#define BQ76952_CELL8_VOLTAGE     0x22
#define BQ76952_CELL9_VOLTAGE     0x24
#define BQ76952_CELL10_VOLTAGE    0x26
#define BQ76952_CELL11_VOLTAGE    0x28
#define BQ76952_CELL12_VOLTAGE    0x2A
#define BQ76952_CELL13_VOLTAGE    0x2C
#define BQ76952_STACK_VOLTAGE     0x34
#define BQ76952_PACK_PIN_VOLTAGE  0x36
#define BQ76952_LD_PIN_VOLTAGE    0x38
#define BQ76952_CC2_CURRENT       0x3A
#define BQ76952_ALARM_RAW_STATUS  0x62


#define BQ76952_INT_TEMPERATURE   0x68
#define BQ76952_TS1_TEMPERATURE   0x70
#define BQ76952_TS2_TEMPERATURE   0x72
#define BQ76952_TS3_TEMPERATURE   0x74


#define BQ76952_SUBCMD_DEVICE_NUMBER    0x0001
#define BQ76952_SUBCMD_FET_ENABLE       0x0022
#define BQ76952_SUBCMD_FET_DISABLE      0x0023
#define BQ76952_SUBCMD_ALL_FETS_ON      0x0024
#define BQ76952_SUBCMD_ALL_FETS_OFF     0x0025
#define BQ76952_SUBCMD_SLEEP            0x0099
#define BQ76952_SUBCMD_CB_ACTIVE_CELLS  0x0083
#define BQ76952_SUBCMD_CB_SET_LVL       0x0084


#define BQ76952_FET_CHG_ENABLE  0x01
#define BQ76952_FET_DSG_ENABLE  0x02
#define BQ76952_FET_BOTH_ENABLE 0x03


#define MAX_CELLS 13
#define MAX_TEMP_SENSORS 4


typedef struct {
    uint16_t cellVoltages[16];      // mV, up to 16 cells
    uint32_t stackVoltage;          // mV, total stack voltage
    uint32_t packVoltage;           // mV, pack voltage at PACK pin
    int32_t current;                // mA, positive = charging
    int16_t temperatures[4];        // 0.1°C (TS1, TS2, TS3, Internal)
    uint8_t numCells;               // Actual number of cells
    uint8_t soc;                    // State of charge %
    uint16_t cycleCount;            // Charge cycles
    
    // Status flags
    struct {
        bool chargeFET;
        bool dischargeFET;
        bool balancingActive;
    } fetStatus;
    
    struct {
        bool overvoltage;
        bool undervoltage;
        bool overtemperature;
        bool undertemperature;
        bool overcurrentCharge;
        bool overcurrentDischarge;
        bool shortCircuit;
        bool cellImbalance;
    } protectionStatus;
    
    uint16_t balancingCells;        // Bit field for cells being balanced
    
} BQ76952_Data_t;

// Function Prototypes
void BQ76952_Init(void);
bool BQ76952_DirectRead(uint8_t reg, uint8_t *data, uint8_t length);
bool BQ76952_DirectWrite(uint8_t reg, uint8_t *data, uint8_t length);
bool BQ76952_SubcommandRead(uint16_t subcmd, uint8_t *data, uint8_t length);
bool BQ76952_SubcommandWrite(uint16_t subcmd, uint8_t *data, uint8_t length);
void BQ76952_ReadCellVoltages(BQ76952_Data_t *data);
void BQ76952_ReadStackVoltage(BQ76952_Data_t *data);
void BQ76952_ReadCurrent(BQ76952_Data_t *data);
void BQ76952_ReadTemperatures(BQ76952_Data_t *data);
void BQ76952_ReadProtectionStatus(BQ76952_Data_t *data);
void BQ76952_EnableFET(uint8_t fetMask);
void BQ76952_DisableFET(uint8_t fetMask);
void BQ76952_EnableBalancing(uint16_t cellMask);
void BQ76952_DisableBalancing(void);
void BQ76952_UpdateAll(BQ76952_Data_t *data);
uint8_t BQ76952_CalculateSOC(BQ76952_Data_t *data);

#endif // BQ76952_H
