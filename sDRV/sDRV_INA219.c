#include "sDRV_INA219.h"

//2024.10.12 v1
//BySightseer. inHNIP9607

#define INA219_I2C_ADDR     (0x40 << 1)
#define RSHUNT_OHM          0.050f  // 50mR

// INA219寄存器地址
#define INA219_REG_CONFIG        0x00
#define INA219_REG_SHUNT_VOLTAGE 0x01
#define INA219_REG_BUS_VOLTAGE   0x02
#define INA219_REG_CALIBRATION   0x05
#define INA219_REG_CURRENT       0x04
#define INA219_REG_POWER         0x03

// 写入寄存器
static inline void write_reg(uint8_t reg_addr, uint8_t data) {
    sBSP_I2C1M_MemSendByte(INA219_I2C_ADDR, reg_addr, I2C_MEMADD_SIZE_8BIT, data);
}

// 读取寄存器
static inline uint8_t read_reg(uint8_t reg_addr) {
    return sBSP_I2C1M_MemReadByte(INA219_I2C_ADDR, reg_addr, I2C_MEMADD_SIZE_8BIT);
}

// 连续读取寄存器
static void read_regs(uint8_t reg_addr, uint8_t* pData, uint8_t len) {
    sBSP_I2C1M_MemReadBytes(INA219_I2C_ADDR, reg_addr, I2C_MEMADD_SIZE_8BIT, pData, len);
}

// 初始化INA219
void sDRV_INA219_Init() {

    uint16_t config = 0x399F; // 配置值需要根据具体应用调整
    uint16_t calibration = 4096; // 计算得到的校准值

    // 写入配置寄存器
    write_reg(INA219_REG_CONFIG, (config >> 8) & 0xFF);
    write_reg(INA219_REG_CONFIG + 1, config & 0xFF);

    // 写入校准寄存器
    write_reg(INA219_REG_CALIBRATION, (calibration >> 8) & 0xFF);
    write_reg(INA219_REG_CALIBRATION + 1, calibration & 0xFF);
}

// 获取电流,单位安培,正表示流出,负表示流入
float sDRV_INA219_GetCurrA() {
    uint8_t buffer[2];
    read_regs(INA219_REG_CURRENT, buffer, 2);
    int16_t rawCurrent = (buffer[0] << 8) | buffer[1];
    // 假设Current_LSB是1mA/bit
    float currentLSB = 0.001; // 1mA per bit
    return rawCurrent * 1;
}

// 获取分压电阻两端的电压
float sDRV_INA219_GetRshuntV() {
    uint8_t buffer[2];
    read_regs(INA219_REG_SHUNT_VOLTAGE, buffer, 2);
    int16_t rawShuntVoltage = (buffer[0] << 8) | buffer[1];
    // 假设Shunt Voltage LSB是10uV/bit
    float shuntVoltageLSB = 0.00001; // 10uV per bit
    return rawShuntVoltage * shuntVoltageLSB;
}

// 获取总线电压
float sDRV_INA219_GetBusV() {
    uint8_t buffer[2];
    read_regs(INA219_REG_BUS_VOLTAGE, buffer, 2);
    uint16_t rawBusVoltage = (buffer[0] << 8) | buffer[1];
    // 总线电压寄存器的值需要右移3位
    rawBusVoltage >>= 3;
    // 假设Bus Voltage LSB是4mV/bit
    float busVoltageLSB = 0.004; // 4mV per bit
    return rawBusVoltage * busVoltageLSB;
}

// 获取总线功率
float sDRV_INA219_GetPwrW() {
    uint8_t buffer[2];
    read_regs(INA219_REG_POWER, buffer, 2);
    int16_t rawPower = (buffer[0] << 8) | buffer[1];
    // 假设Power_LSB是20 * Current_LSB
    float powerLSB = 20 * 0.001; // 20 * Current_LSB
    return rawPower * powerLSB;
}
