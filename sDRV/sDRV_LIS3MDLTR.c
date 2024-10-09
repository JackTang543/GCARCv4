#include "sDRV_LIS3MDLTR.h"


#include "sLib_Reg.h"

//240820~082x   第一版 支持SPI

#define USE_SPI_IF

//PC1 -> LIS3_CS
#define SPI_CS_CLK_EN    __GPIOC_CLK_ENABLE
#define SPI_CS_PORT      GPIOC
#define SPI_CS_PIN       GPIO_PIN_1

#ifdef USE_SPI_IF
    #include "sBSP_SPI.h"

#endif

sDRV_LIS3_Data_t g_lis3;


//寄存器地址和掩码

//todo WHO_AM_I
//读出始终为0x3D
#define ADDR_WHOAMI                            (0x0F)

//todo CTRL_REG1
#define ADDR_CTRLREG1                          (0x20)
//温度传感器使能 默认:0
#define MSK_CTRLREG1_TEMPEN                    (0b10000000)
//X和Y轴操作模式选择 默认:00
//OM1        OM0         XY轴操作模式
//0          0           LP 低功耗模式
//0          1           MP 中性能模式
//1          0           HP 高性能模式
//1          1           UHP超高性能模式
#define MSK_CTRLREG1_OM                        (0b01100000)
//输出数据速率选择 默认:100
//DO2     DO1     DO0       ODR Hz
//0       0       0         0.625
//0       0       1         1.25
//0       1       0         2.5
//0       1       1         5
//1       0       0         10
//1       0       1         20
//1       1       0         40
//1       1       1         80
#define MSK_CTRLREG1_DO                        (0b00011100)
//使能FAST_ODR来得到高于80Hz的输出速率 默认:0
//FAST_ODR      ODR Hz       OM
//1             1000Hz       LP
//1             560 Hz       MP
//1             300 Hz       HP
//1             155 Hz       UHP
#define MSK_CTRLREG1_FASTODR                   (0b00000010)
//自检使能 默认:0
#define MSK_CTRLREG1_ST                        (0b00000001)

//todo CTRL_REG2
#define ADDR_CTRLREG2                          (0x21)
//满量程配置 默认:00
//FS1      FS0        满量程
//0        0          +-4gauss
//0        1          +-8gauss
//1        0          +-12gauss
//1        1          +-16gauss
#define MSK_CTRLREG2_FS                        (0b01100000)
//复位内存内容 默认0
#define MSK_CTRLREG2_REBOOT                    (0b00001000)
//软复位配置寄存器和用户寄存器
#define MSK_CTRLREG2_SOFTRST                   (0b00000100)

//todo CTRL_REG3
#define ADDR_CTRLREG3                          (0x22)
//低功耗模式配置 默认:0
#define MSK_CTRLREG3_LP                        (0b00100000)
//SPI接口模式 0:4线SPI(默认) 1:3线SPI
#define MSK_CTRLREG3_SIM                       (0b00000100)
//操作模式 默认:11
//MD1   MD0   模式
//0     0     连续转换模式
//0     1     单次模式
//1     0     关机模式
//1     1     关机模式
#define MSK_CTRLREG3_MD                        (0b00000011)

//todo CTRL_REG4
#define ADDR_CTRLREG4                          (0x23)
//Z轴操作模式选择 默认:00
//OM1        OM0         Z轴操作模式
//0          0           LP 低功耗模式
//0          1           MP 中性能模式
//1          0           HP 高性能模式
//1          1           UHP超高性能模式
#define MSK_CTRLREG4_OMZ                       (0b00001100)
//大小端模式选择 默认:0
//0:数据LSB在低地址 1:数据MSB在低地址
#define MSK_CTRLREG4_BLE                       (0b00000010)

//todo CTRL_REG5
#define ADDR_CTRLREG5                          (0x24)
//快速读取模式 启动允许只读取DATAOUT的高位部分以提高效率 默认:0
#define MSK_CTRLREG5_FASTREAD                  (0b10000000)
//阻塞模式读取数据 默认:0
//0:连续更新模式 1:输出寄存器不更新直到MSB和LSB已被读取
#define MSK_CTRLREG5_BDU                       (0b01000000)

//todo STATUS_REG
#define ADDR_STATUSREG                         (0x27)
//XYZ数据超限 0:没有超限发生 1:一个新的数据集覆写了上一个数据集
#define MSK_STATUSREG_ZYXOR                    (0b10000000)
//Z轴数据超限
#define MSK_STATUSREG_ZOR                      (0b01000000)
//Y轴数据超限
#define MSK_STATUSREG_YOR                      (0b00100000)
//X轴数据超限
#define MSK_STATUSREG_XOR                      (0b00010000)
//XYZ轴有新数据可用 0:新数据还不可用 1:新数据集可用
#define MSK_STATUSREG_ZYXDA                    (0b00001000)
//Z轴
#define MSK_STATUSREG_ZDA                      (0b00000100)
//Y轴
#define MSK_STATUSREG_YDA                      (0b00000010)
//X轴
#define MSK_STATUSREG_XDA                      (0b00000001)

//todo OUT_X_L
#define ADDR_OUTXL                             (0x28)

//todo OUT_X_H
#define ADDR_OUTXH                             (0x29)

//todo OUT_Y_L
#define ADDR_OUTYL                             (0x2A)

//todo OUT_Y_H
#define ADDR_OUTYH                             (0x2B)

//todo OUT_Z_L
#define ADDR_OUTZL                             (0x2C)

//todo OUT_Z_H
#define ADDR_OUTZH                             (0x2D)

//todo TEMP_OUT_L
#define ADDR_TEMPOUTL                          (0x2E)

//todo TEMP_OUT_H
#define ADDR_TEMPOUTH                          (0x2F)

//todo INT_CFG
#define ADDR_INTCFG                            (0x30)
//X轴中断产生使能 默认:1
#define MSK_INTCFG_XIEN                        (0b10000000)
//Y轴中断产生使能 默认:1
#define MSK_INTCFG_YIEN                        (0b01000000)
//Z轴中断产生使能 默认:1
#define MSK_INTCFG_ZIEN                        (0b00100000)
//中断INT引脚激活时电平 0:LO(默认) 1:HI
#define MSK_INTCFG_IEA                         (0b00000100)
//锁存中断请求 0:中断请求被锁存(默认) 1:中断请求不被锁存
#define MSK_INTCFG_LIR                         (0b00000010)
//INT脚中断使能 默认:0
#define MSK_INTCFG_IEN                         (0b00000001)

//todo INT_SRC
#define ADDR_INTSRC                            (0x31)
#define MSK_INTSRC_PTHX                        (0b10000000)
#define MSK_INTSRC_PTHY                        (0b01000000)
#define MSK_INTSRC_PTHZ                        (0b00100000)
#define MSK_INTSRC_NTHX                        (0b00010000)
#define MSK_INTSRC_NTHY                        (0b00001000)
#define MSK_INTSRC_NTHZ                        (0b00000100)
#define MSK_INTSRC_MROI                        (0b00000010)
#define MSK_INTSRC_INT                         (0b00000001)

//todo INT_THS_L
#define ADDR_INTTHSL                           (0x32)

//todo INT_THS_H
#define ADDR_INTTHSH                           (0x33)


//接口
//初始化CS引脚
static void portCSInit(){
    SPI_CS_CLK_EN();
    GPIO_InitTypeDef gpio = {0};
    gpio.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio.Pull  = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
    gpio.Pin   = SPI_CS_PIN;
    HAL_GPIO_Init(SPI_CS_PORT,&gpio);
}

static inline void portSetCS(bool lv){
    HAL_GPIO_WritePin(SPI_CS_PORT,SPI_CS_PIN,(GPIO_PinState)lv);
}

static uint8_t read_reg(uint8_t addr){
    //读时序有两个字节,第一个字节最高位为1表示读,为0表示写时序
    //第二个字节为MS 为1在多字节读写时自增,为0不自增,这里始终为1
    //其他位为寄存器地址
    portSetCS(0);
    //首先发送第一个字节
    sBSP_SPI_IMU_SendByte(0x80 | 0x40 | addr);
    //然后读取第二个字节
    uint8_t tmp = sBSP_SPI_IMU_RecvByte();
    portSetCS(1);
    return tmp;
}
static void write_reg(uint8_t addr,uint8_t data){
    portSetCS(0);
    //首先发送第一个字节
    sBSP_SPI_IMU_SendByte(0x00 | 0x40 | addr);
    //然后发送第二个字节
    sBSP_SPI_IMU_SendByte(data);
    portSetCS(1);
}
//一次读取多个寄存器,用于加速读取原始数据速度
static void read_regs(uint8_t addr,uint8_t* pData,uint8_t len){
    portSetCS(0);
    //首先发送第一个字节
    sBSP_SPI_IMU_SendByte(0x80 | 0x40 | addr);
    //然后读取
    sBSP_SPI_IMU_RecvBytes(pData,len);
    portSetCS(1);
    
}
// 对寄存器进行修改,形参:寄存器地址,修改的部分,修改的数据
static void reg_modify(uint8_t reg_addr, uint8_t reg_msk, uint8_t data){
    //读改写
    //首先读出寄存器
    uint8_t tmpreg = read_reg(reg_addr);
	//sHMI_Debug_Printf("读出位置0x%X的内容:0x%X\n",reg_addr,tmpreg);
    //进行修改
    sLib_ModifyReg(&tmpreg, reg_msk, data);
	//sHMI_Debug_Printf("位掩码:0x%X,数据:0x%X,修改后的内容:0x%X\n\n",reg_msk,data,tmpreg);
    //写回
    write_reg(reg_addr, tmpreg);
}





int sDRV_LIS3_Init(){
    #ifdef USE_SPI_IF
        portCSInit();
        portSetCS(1);
    #endif
    //检查通信是否正常
    uint8_t who_am_i = read_reg(ADDR_WHOAMI);
    if(who_am_i != 0x3D){
        //sHMI_Debug_Printf("[ERR ]LIS3MDLTR初始化失败:0x%0X\n",who_am_i);
        return -1;
    }

    //使能温度传感器
    reg_modify(ADDR_CTRLREG1,MSK_CTRLREG1_TEMPEN,1);
    //配置操作模式为连续转换模式
    reg_modify(ADDR_CTRLREG3,MSK_CTRLREG3_MD,SDRV_LIS3_MD_CONTINUOUS);
    //配置量程4Ga
    reg_modify(ADDR_CTRLREG2,MSK_CTRLREG2_FS,SDRV_LIS3_FS_4GAUSS);
    g_lis3.fs = SDRV_LIS3_FS_4GAUSS;
    //配置ODR
    reg_modify(ADDR_CTRLREG1,MSK_CTRLREG1_DO,SDRV_LIS3_ODR_80HZ);
    //配置XYZ轴操作模式为超高性能模式
    reg_modify(ADDR_CTRLREG1,MSK_CTRLREG1_OM,SDRV_LIS3_XYOM_UHP);
    reg_modify(ADDR_CTRLREG4,MSK_CTRLREG4_OMZ,SDRV_LIS3_ZOM_UHP);
    //设置阻塞读取模式
    reg_modify(ADDR_CTRLREG5,MSK_CTRLREG5_BDU,1);


    //sHMI_Debug_Printf("[OK  ]LIS3MDLTR初始化成功\n");

    return 0;
}

void sDRV_LIS3_GetData(){
    uint8_t buf[8] = {0};

    buf[0] = read_reg(ADDR_OUTXL);
    buf[1] = read_reg(ADDR_OUTXH);
    buf[2] = read_reg(ADDR_OUTYL);
    buf[3] = read_reg(ADDR_OUTYH);
    buf[4] = read_reg(ADDR_OUTZL);
    buf[5] = read_reg(ADDR_OUTZH);
    buf[6] = read_reg(ADDR_TEMPOUTL);
    buf[7] = read_reg(ADDR_TEMPOUTH);

    int16_t temp = buf[7] << 8 | buf[6];

    g_lis3.mag_x = (float)(int16_t)((uint16_t)(buf[1] << 8)  | (uint16_t)buf[0]);
    g_lis3.mag_y = (float)(int16_t)((uint16_t)(buf[3] << 8)  | (uint16_t)buf[2]);
    g_lis3.mag_z = (float)(int16_t)((uint16_t)(buf[5] << 8)  | (uint16_t)buf[4]);

    //sHMI_Debug_Printf("0x%0X, 0x%0X, 0x%0X, 0x%0X, 0x%0X, 0x%0X\n",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5]);

    //处理数据
	if(g_lis3.fs == SDRV_LIS3_FS_4GAUSS){
		g_lis3.mag_x *= ( 4000.0f / 32768.0f);
        g_lis3.mag_y *= ( 4000.0f / 32768.0f);
        g_lis3.mag_z *= ( 4000.0f / 32768.0f);
	}else if(g_lis3.fs == SDRV_LIS3_FS_8GAUSS){
		g_lis3.mag_x *= ( 8000.0f / 32768.0f);
        g_lis3.mag_y *= ( 8000.0f / 32768.0f);
        g_lis3.mag_z *= ( 8000.0f / 32768.0f);
	}else if(g_lis3.fs == SDRV_LIS3_FS_12GAUSS){
		g_lis3.mag_x *= (12000.0f / 32768.0f);
        g_lis3.mag_y *= (12000.0f / 32768.0f);
        g_lis3.mag_z *= (12000.0f / 32768.0f);
	}else if(g_lis3.fs == SDRV_LIS3_FS_16GAUSS){
		g_lis3.mag_x *= (16000.0f / 32768.0f);
        g_lis3.mag_y *= (16000.0f / 32768.0f);
        g_lis3.mag_z *= (16000.0f / 32768.0f);
	}

    g_lis3.temp = (float)temp;

    //硬磁校准
    // g_lis3.mag_x = g_lis3.mag_x - 162.68 - 36.64;
    // g_lis3.mag_y = g_lis3.mag_y - 30;
    // g_lis3.mag_z = g_lis3.mag_z - 158.29 - 38.78;


    // g_lis3.mag_x -= 162.68f;
    // g_lis3.mag_y -= -75.06f;
    // g_lis3.mag_z -= 158.29f;


    //float heading = atan2(g_lis3.mag_y, g_lis3.mag_x) * RAD2DEG;


    //sHMI_Debug_Printf("温度:%d,%.2f,%.2f,%.2f,%.2f\n",\
                        temp,g_lis3.mag_x,g_lis3.mag_y,g_lis3.mag_z, \
                        sqrtf(g_lis3.mag_x*g_lis3.mag_x + \
                            g_lis3.mag_y*g_lis3.mag_y + \
                            g_lis3.mag_z*g_lis3.mag_z));

    //sHMI_Debug_Printf("%.2f,%.2f,%.2f,%d\n",\
                        g_lis3.mag_x,g_lis3.mag_y,g_lis3.mag_z,temp \
                        );

}

