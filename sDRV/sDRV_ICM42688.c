#include "sDRV_ICM42688.h"


//time 2024.08.11~08.16第一版开始 只支持SPI
/**
 * 加速度计
 * +-2G:16384LSB/g=1671.8LSB/m/s^2
 * 尺度因子误差:+-0.5%
 * 误差随温度改变:0.005%/摄氏度
 * 非线性度:+-0.1%
 * 非正交误差:+-1%
 * 零输入输出:+-20mg=+-0.196m/s^2
 * 零输入输出随温度变化:+-0.15mg/摄氏度
 * 
 * 陀螺仪
 * +-250度/s:131LSB/度
 * 尺度因子误差:+-0.5%
 * 误差随温度改变:+-0.005%/摄氏度
 * 非线性度:+-0.1%
 * 非正交误差:+-1.25%
 * 零输入输出:+-0.5度/s
 * 零输入输出随温度变化:+-0.005度/s/摄氏度
 * 
 */

#include "sLib_Reg.h"

#define USE_SPI_IF

//PC0 -> ICM_CS
#define SPI_CS_CLK_EN    __GPIOC_CLK_ENABLE
#define SPI_CS_PORT      GPIOC
#define SPI_CS_PIN       GPIO_PIN_0

#ifdef USE_SPI_IF
    #include "sBSP_SPI.h"
#endif


sDRV_ICM_Data_t g_icm;
sDRV_ICM_Conf_t g_icm_conf;

//寄存器地址和掩码
/*Bank0的寄存器*/

//todo 设备配置
#define ADDR_DEVICECONFIG                       (0x11)
//SPI模式选择 0:模式0和模式3(默认) 1:模式1和模式2
#define MSK_DEVICECONFIG_SPIMODE                (0b00010000)
//软复位 写入1器件将在1ms后复位
#define MSK_DEVICECONFIG_SOFTRESETCONFIG        (0b00000001)

//todo 驱动配置
#define ADDR_DRIVECONFIG                        (0x13)
//I2C的速率 000:20ns~60ns直到101:<2ns 110,111 保留
//Controls slew rate for output pin 14 in I2C mode only
#define MSK_DRIVECONFIG_I2CSLEWRATE             (0b00111000)
//SPI的速率 000:20ns~60ns直到101:<2ns 110,111 保留
//Controls slew rate for output pin 14 in SPI or I3CSM mode, and for all other output pins
#define MSK_DRIVECONFIG_SPISLEWRATE             (0b00000111)

//todo 中断配置
#define ADDR_INTCONFIG                          (0x14)
//INT2中断模式 0:脉冲模式 1:锁存模式
#define MSK_INTCONFIG_INT2MODE                  (0b00100000)
//INT2驱动电路 0:开漏 1:推挽
#define MSK_INTCONFIG_INT2DRIVECIRCUIT          (0b00010000)
//INT2输出极性 0:激活为LO(默认) 1:激活为HI
#define MSK_INTCONFIG_INT2POLARITY              (0b00001000)
//INT1中断模式 0:脉冲模式 1:锁存模式
#define MSK_INICONFIG_INT1MODE                  (0b00000100)
//INT1驱动电路 0:开漏 1:推挽
#define MSK_INTCONFIG_INT1DRIVECIRCUIT          (0b00000010)
//INT1输出极性 0:激活为LO(默认) 1:激活为HI
#define MSK_INTCONFIG_INT1POLARITY              (0b00000001)

//todo FIFO配置
#define ADDR_FIFOCONFIG                         (0x16)
//FIFO模式配置 00:旁路模式(不使用FIFO) 01:数据流FIFO模式 10/11:满了就停模式
#define MSK_FIFOCONFIG_FIFOMODE                 (0b11000000)

//todo TEMP_DATA1 温度数据高8位
#define ADDR_TEMPDATA1                          (0x1D)

//todo TEMP_DATA1 温度数据低8位
#define ADDR_TEMPDATA0                          (0x1E)

//todo ACCEL_DATA_X1
#define ADDR_ACCELDATA_X1                       (0x1F)

//todo ACCEL_DATA_X0
#define ADDR_ACCELDATA_X0                       (0x20)

//todo ACCEL_DATA_Y1
#define ADDR_ACCELDATA_Y1                       (0x21)

//todo ACCEL_DATA_Y0
#define ADDR_ACCELDATA_Y0                       (0x22)

//todo ACCEL_DATA_Z1
#define ADDR_ACCELDATA_Z1                       (0x23)

//todo ACCEL_DATA_Z0
#define ADDR_ACCELDATA_Z0                       (0x24)

//todo GYRO_DATA_X1
#define ADDR_GYRODATA_X1                        (0x25)

//todo GYRO_DATA_X0
#define ADDR_GYRODATA_X0                        (0x26)

//todo GYRO_DATA_Y1
#define ADDR_GYRODATA_Y1                        (0x27)

//todo GYRO_DATA_Y0
#define ADDR_GYRODATA_Y0                        (0x28)

//todo GYRO_DATA_Z1
#define ADDR_GYRODATA_Z1                        (0x29)

//todo GYRO_DATA_Z0
#define ADDR_GYRODATA_Z0                        (0x2A)

//todo TMST_FSYNCH 
//Stores the upper byte of the time delta from the rising edge of FSYNC to the latest ODR until the UI Interface reads the FSYNC tag in the status register
#define ADDR_TMSTFSYNCH                         (0x2B)

//todo TMST_FSYNCL
#define ADDR_TMSTFSYNCL                         (0x2C)

//todo INT_STATUS
#define ADDR_INTSTATUS                          (0x2D)
//UI FSYNC中断如果产生此位自动set
#define MSK_INTSTATUS_UIFSYNCINT                (0b01000000)
//PLL准备好中断标志
#define MSK_INTSTATUS_PLLRDYINT                 (0b00100000)
//复位完成中断标志
#define MSK_INTSTATUS_RESETDONEINT              (0b00010000)
//数据准备好中断标志
#define MSK_INTSTATUS_DATARDYINT                (0b00001000)
//FIFO快要满了中断标志
#define MSK_INTSTATUS_FIFOTHSINT                (0b00000100)
//FIFO满了中断标志
#define MSK_INTSTATUS_FIFOFULLINT               (0b00000010)
//AGC准备好中断标志
#define MSK_INTSTATUS_AGCRDYINT                 (0b00000001)

//todo FIFO_COUNTH FIFO计数HI
#define ADDR_FIFOCOUNTH                         (0x2E)

//todo FIFO_COUNTH FIFO计数HL
#define ADDR_FIFOCOUNTL                         (0x2F)

//todo FIFO_DATA FIFO数据读取
#define ADDR_FIFODATA                           (0x30)

/*0x31~0x38寄存器忽略*/

//todo SIGNAL_PATH_RESET 信号路径重置
#define ADDR_SIGNALPATHRESET                    (0x4B)
//置位使能DMP
#define MSK_SIGNALPATHRESET_DMPINITEN           (0b01000000)
//置位把DMP的内存清空
#define MSK_SIGNALPATHRESET_DMPMEMRESETEN       (0b00100000)
//置位信号路径和ODR计数器将被重置
#define MSK_SIGNALPATHRESET_ABORTANDRESET       (0b00001000)
//置位时间戳计数器将锁存到时间戳寄存器中
#define MSK_SIGNALPATHRESET_TMSTSTROBE          (0b00000100)
//置位FIFO将被清空
#define MSK_SIGNALPATHRESET_FIFOFLUSH           (0b00000010)

//todo INTF_CONFIG0
#define ADDR_INTFCONFIG0                        (0x4C)
#define MSK_INTFCONFIG0_FIFOHOLDLASTDATAEN      (0b10000000)
#define MSK_INTFCONFIG0_FIFOCOUNTREC            (0b01000000)
#define MSK_INTFCONFIG0_FIFOCOUNTENDIAN         (0b00100000)
#define MSK_INTFCONFIG0_SENSORDATAENDIAN        (0b00010000)
//10:禁用SPI 11:禁用I2C
#define MSK_INTFCONFIG0_UISIFSCFG               (0b00000011)

//todo INTF_CONFIG1
#define ADDR_INTFCONFIG1                        (0x4D)
//加速度计低功耗时钟设置 0:加速度计低功耗模式使用Wake up振荡器时钟 1:使用RC振荡器时钟
#define MSK_INTFCONFIG1_ACCELLPCLKSEL           (0b00001000)
//RTC模式 0:不需要输入RTC时钟 1:需要输入RTC时钟
#define MSK_INTFCONFIG1_RTCMODE                 (0b00000100)
//时钟配置 00:总是使用内部RC振荡器 01:使用PLL(如果可用),否则使用RC(默认值) 11:禁用所有时钟
#define MSK_INTFCONFIG1_CLKSEL                  (0b00000011)

//todo PWR_MGMT0 电源管理
#define ADDR_PWRMGMT0                           (0x4E)
//0:启动温度传感器(默认) 1:禁用温度传感器
#define MSK_PWRMGMT0_TEMPDIS                    (0b00100000)
//1:即使加速度计和陀螺仪电源关闭,RC也会开启 0:跟随两个传感器自动关闭(默认)
#define MSK_PWRMGMT0_IDLE                       (0b00010000)
//陀螺仪模式 00:关闭陀螺仪(默认) 01:standby模式 11:低噪声模式
//注意:启动陀螺仪要等待至少45ms使其稳定,并且在200us内不要读写他的寄存器
#define MSK_PWRMGMT0_GYROMODE                   (0b00001100)
//00/01:关闭加速度计(默认) 10:低功耗模式 11:低噪声模式
//注意:启动加速度计在200us内不要读写他的寄存器
#define MSK_PWRMGMT0_ACCELMODE                  (0b00000011)

//todo GYRO_CONFIG0 陀螺仪配置0
#define ADDR_GYROCONFIG0                        (0x4F)
//设置陀螺仪满量程 
//000:+-2000dps(默认) 001:+-1000dps  010:+-500dps   011:+-250dps
//100:+-125dps        101:+-62.5dps 110:+-31.25dps 111:+-15.625dps
#define MSK_GYROCONFIG0_GYROFSSEL               (0b11100000)
//设置陀螺仪输出速率
//0000:保留   0001:32KHz 0010:16KHz 0011:8KHz 0100:4KHz  0101:2KHz  0110:1KHz(默认)
//0111:200Hz 1000:100Hz 1001:50Hz  1010:25Hz 1011:12.5Hz 1111:500Hz
#define MSK_GYROCONFIG0_GYROODR                 (0b00001111)

//todo ACCEL_CONFIG0 加速度计配置0
#define ADDR_ACCELLCONFIG0                      (0x50)
//加速度计满量程配置 000:+-16g(默认) 001::+-8g 010:+-4g 011:+-2g
#define MSK_ACCELCONFIG0_ACCELFSSEL             (0b11100000)
//加速度计输出速率  0001:32KHz(LN模式)  0010:16KHz(LN)     0011:8KHz(LN)    0100:4KHz(LN)
//0101:2KHz(LN)    0110:1KHz(LN)(默认) 0111:200Hz(LP/LN) 1000:100Hz(LP/LN)
//1001:50Hz(LP/LN) 1010:25Hz(LP/LN)   1011:12.5Hz(LP/LN) 1100:6.25(LP)
//1101:3.125Hz(LP) 1110:1.5625Hz(LP)  1111:500Hz(LP/LN)
#define MSK_ACCELCONFIG0_ACCELODR               (0b00001111)

//todo GYRO_CONFIG1
#define ADDR_GYROCONFIG1                        (0x51)
//温度数字低通滤波
//值      DLPF带宽       DLPF延时
//000     4KHz          0.125ms(默认)
//001     170Hz         1ms
//010     82Hz          2ms
//011     40Hz          4ms
//100     20Hz          8ms
//101     10Hz          16ms
//110/111 5Hz           32ms
#define MSK_GYROCONFIG1_TEMPFILTBW              (0b11100000)
//选择陀螺仪UI滤波器的阶数 00:1阶 01:2阶 10:3阶
#define MSK_GYROCONFIG1_GYROUIFILTORD           (0b00001100)
//选择陀螺仪 DEC2 M2滤波器 10:3阶滤波器
#define MSK_GYROCONFIG1_GYRODEC2M2ORD           (0b00000011)

//todo GYRO_ACCEL_CONFIG0 陀螺仪和加速度计配置
#define ADDR_GYROACCELCONFIG0                   (0x52)
//设置加速度计LPF的带宽 在LN模式下:
//0001 BW=max(400Hz, ODR)/4 (默认)
//0010 BW=max(400Hz, ODR)/5
//0011 BW=max(400Hz, ODR)/8
//0100 BW=max(400Hz, ODR)/10
//0101 BW=max(400Hz, ODR)/16
//0110 BW=max(400Hz, ODR)/20
//0111 BW=max(400Hz, ODR)/40
//1110 低延时模式 Dec2滤波器输出进行简单抽取 滤波器运行在max(400Hz,ODR)
//1111 低延时模式 Dec2滤波器输出进行简单抽取 滤波器运行在max(400Hz,ODR * 8)
//在LP模式下
//0001 1x平均滤波(默认)
//0110 16x平均滤波
#define MSK_GYROACCELCONFIG0_ACCELUIFILTBW      (0b11110000)
//设置陀螺仪LPF的带宽,在LN模式下:
//0000 BW=ODR/2
//0001 BW=max(400Hz, ODR)/4 (默认)
//0010 BW=max(400Hz, ODR)/5
//0011 BW=max(400Hz, ODR)/8
//0100 BW=max(400Hz, ODR)/10
//0101 BW=max(400Hz, ODR)/16
//0110 BW=max(400Hz, ODR)/20
//0111 BW=max(400Hz, ODR)/40
//1110 低延时模式 Dec2滤波器输出进行简单抽取 滤波器运行在max(400Hz,ODR)
//1111 低延时模式 Dec2滤波器输出进行简单抽取 滤波器运行在max(400Hz,ODR * 8)
#define MSK_GYROACCELCONFIG0_GYROUIFILTBW       (0b00001111)

//todo ACCEL_CONFIG1
#define ADDR_ACCELCONFIG1                       (0x53)
//选择加速度计UI滤波器的阶数 00:1阶 01:2阶 10:3阶
#define MSK_ACCELCONFIG1_ACCELUIFILTORD         (0b00011000)
//选择陀螺仪 DEC2 M2滤波器 10:3阶滤波器
#define MSK_ACCELCONFIG1_ACCELDEC2M2ORD         (0b00000110)

/*0x54~0x6D 忽略*/

//todo SELF_TEST_CONFIG 自检配置
#define ADDR_SELFTESTCONFIG                     (0x70)
//1:启动加速度计自检电源,记得在自检完之后设置为0
#define MSK_SELFTESTCONFIG_ACCELSTPOWER         (0b01000000)
//Z轴加速度计自检使能
#define MSK_SELFTESTCONFIG_ENAZST               (0b00100000)
//Y轴加速度计自检使能
#define MSK_SELFTESTCONFIG_ENAYST               (0b00010000)
//X轴加速度计自检使能
#define MSK_SELFTESTCONFIG_ENAXST               (0b00001000)
//Z轴陀螺仪自检使能
#define MSK_SELFTESTCONFIG_ENGZST               (0b00000100)
//Y轴陀螺仪自检使能
#define MSK_SELFTESTCONFIG_ENGYST               (0b00000010)
//X轴陀螺仪自检使能
#define MSK_SELFTESTCONFIG_ENGXST               (0b00000001)

//todo WHO_AM_I 我是谁
//读出值始终为0x47
#define ADDR_WHOAMI                             (0x75)

//todo REG_BANK_SEL 选择访问哪一个寄存器bank
#define ADDR_REGBANKSEL                         (0x76)
//000:bank0(默认) 001:bank1 010:bank2 011:bank3 100:bank4
#define MSK_REGBANKSEL_BANKSEL                  (0b00000111)

/*Bank1的寄存器*/

//todo 传感器配置
#define ADDR_SENSORCONFIG0                      (0x03)
//0:启动 1:关闭
#define MSK_SENSORCONFIG_ZG_DISABLE             (0b00100000)
#define MSK_SENSORCONFIG_YG_DISABLE             (0b00010000)
#define MSK_SENSORCONFIG_XG_DISABLE             (0b00001000)
#define MSK_SENSORCONFIG_ZA_DISABLE             (0b00000100)
#define MSK_SENSORCONFIG_YA_DISABLE             (0b00000010)
#define MSK_SENSORCONFIG_XA_DISABLE             (0b00000001)

//todo GYRO_CONFIG_STATIC2 陀螺仪静态配置2
#define ADDR_GYROCONFIGSTATIC2                  (0x0B)
//0:使能陀螺仪抗混叠滤波器(默认) 1:关闭
#define MSK_GYROCONFIG_STATIC2_GYROAAFDIS       (0b00000010)
//0:使能陀螺仪陷波器(默认) 1:关闭
#define MSK_GYROCONFIG_STATIC2_GYRONFDIS        (0b00000001)

//todo GYRO_CONFIG_STATIC3 陀螺仪静态配置3
#define ADDR_GYROCONFIGSTATIC3                  (0x0C)
//控制陀螺仪抗混叠滤波器的带宽

#define MSK_GYROCONFIGSTATIC3_GYROAAFDELT       (0b00111111)

//todo GYRO_CONFIG_STATIC4 陀螺仪静态配置4
#define ADDR_GYROCONFIGSTATIC4                  (0x0C)
//控制陀螺仪抗混叠滤波器的带宽
#define MSK_GYROCONFIGSTATIC4_GYROAAFDELTSQR    (0b11111111)


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
    //读时序有两个字节,第一个字节最高位为1表示读,为0表示写时序,其他位为寄存器地址
    portSetCS(0);
    //首先发送第一个字节
    sBSP_SPI_IMU_SendByte(0x80 | addr);
    //然后读取第二个字节
    uint8_t tmp = sBSP_SPI_IMU_RecvByte();
    portSetCS(1);
    return tmp;
}
static void write_reg(uint8_t addr,uint8_t data){
    portSetCS(0);
    //首先发送第一个字节
    sBSP_SPI_IMU_SendByte(0x00 | addr);
    //然后发送第二个字节
    sBSP_SPI_IMU_SendByte(data);
    portSetCS(1);
}
//一次读取多个寄存器,用于加速读取原始数据速度
static void read_regs(uint8_t addr,uint8_t* pData,uint8_t len){
    portSetCS(0);
    //首先发送第一个字节
    sBSP_SPI_IMU_SendByte(0x80 | addr);
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









int sDRV_ICM_Init(){
    #ifdef USE_SPI_IF
        portCSInit();
        portSetCS(1);
    #endif
    
    //检查通信是否正常
    uint8_t who_am_i = read_reg(ADDR_WHOAMI);
    if(who_am_i != 0x47){
        //sHMI_Debug_Printf("[ERR ]ICM42688初始化失败:0x%0X\n",who_am_i);
        return -1;
    }
    //上电复位器件
    reg_modify(ADDR_DEVICECONFIG,MSK_DEVICECONFIG_SOFTRESETCONFIG,1);
    HAL_Delay(50);

    //i2c dis
    reg_modify(ADDR_INTFCONFIG0,MSK_INTFCONFIG0_UISIFSCFG,0b11);

    //启动加速度计和陀螺仪
    reg_modify(ADDR_PWRMGMT0,MSK_PWRMGMT0_ACCELMODE,0b11);
    reg_modify(ADDR_PWRMGMT0,MSK_PWRMGMT0_GYROMODE ,0b11);
    HAL_Delay(50);
    

    //初始化配置:实时性+++
    sDRV_ICM_Conf_t icm_conf = {0};
    icm_conf.gyro_fs           = SDRV_ICM_GYROFS_500DPS;
    icm_conf.gyro_odr          = SDRV_ICM_GYROODR_100HZ;
    icm_conf.accel_fs          = SDRV_ICM_ACCELFS_2G;
    icm_conf.accel_odr         = SDRV_ICM_ACCELODR_100HZ;
    icm_conf.temp_filt_bw      = SDRV_ICM_TEMPFILTBW_5HZ;
    icm_conf.gyro_ui_filt_ord  = SDRV_ICM_GYROUIFILTORD_1;
    icm_conf.gyro_dec2_m2_ord  = SDRV_ICM_GYRODEC2M2ORD_3;
    icm_conf.accel_ui_filt_bw  = SDRV_ICM_ACCELUIFILTBW_LNLLODR;
    icm_conf.gyro_ui_filt_bw   = SDRV_ICM_GYROUIFILTBW_LNLLODR;
    icm_conf.accel_ui_filt_ord = SDRV_ICM_ACCELUIFILTORD_1;
    icm_conf.accel_dec2_m2_ord = SDRV_ICM_ACCELDEC2M2ORD_3;
    sDRV_ICM_SetConfig(&icm_conf);

    //启用RTC时钟输入
    //reg_modify(ADDR_INTFCONFIG1,MSK_INTFCONFIG1_RTCMODE,1);

    //sHMI_Debug_Printf("[OK  ]ICM42688初始化成功\n");

    return 0;
}

void sDRV_ICM_SetConfig(const sDRV_ICM_Conf_t* p_conf){
    //如果传入NULL则更新本地的配置结构体,否则更新为用户传入的配置
    if(p_conf != NULL){
        memcpy(&g_icm_conf,p_conf,sizeof(sDRV_ICM_Conf_t));
    }
    
    //bank=0,上传配置
    reg_modify(ADDR_REGBANKSEL,MSK_REGBANKSEL_BANKSEL,SDRV_ICM_REGBANKSEL_BANK0);
    
    reg_modify(ADDR_GYROCONFIG0     ,MSK_GYROCONFIG0_GYROFSSEL         ,g_icm_conf.gyro_fs);
    reg_modify(ADDR_GYROCONFIG0     ,MSK_GYROCONFIG0_GYROODR           ,g_icm_conf.gyro_odr);
    reg_modify(ADDR_ACCELLCONFIG0   ,MSK_ACCELCONFIG0_ACCELFSSEL       ,g_icm_conf.accel_fs);
    reg_modify(ADDR_ACCELLCONFIG0   ,MSK_ACCELCONFIG0_ACCELODR         ,g_icm_conf.accel_odr);
    reg_modify(ADDR_GYROCONFIG1     ,MSK_GYROCONFIG1_TEMPFILTBW        ,g_icm_conf.temp_filt_bw);
    reg_modify(ADDR_GYROCONFIG1     ,MSK_GYROCONFIG1_GYROUIFILTORD     ,g_icm_conf.gyro_ui_filt_ord);
    reg_modify(ADDR_GYROCONFIG1     ,MSK_GYROCONFIG1_GYRODEC2M2ORD     ,g_icm_conf.gyro_dec2_m2_ord);
    reg_modify(ADDR_GYROACCELCONFIG0,MSK_GYROACCELCONFIG0_ACCELUIFILTBW,g_icm_conf.accel_ui_filt_bw);
    reg_modify(ADDR_GYROACCELCONFIG0,MSK_GYROACCELCONFIG0_GYROUIFILTBW ,g_icm_conf.gyro_ui_filt_bw);
    reg_modify(ADDR_ACCELCONFIG1    ,MSK_ACCELCONFIG1_ACCELUIFILTORD   ,g_icm_conf.accel_ui_filt_ord);
    reg_modify(ADDR_ACCELCONFIG1    ,MSK_ACCELCONFIG1_ACCELDEC2M2ORD   ,g_icm_conf.accel_dec2_m2_ord);
}

float sDRV_ICM_GetTemp(){
    uint8_t buf[2];
    read_regs(ADDR_TEMPDATA1,buf,2);
    int16_t temp = (buf[0] << 8) | buf[1];
    return (float)temp / 132.48 + 25;
}

void sDRV_ICM_GetData(){
    static uint8_t buf[14];
    //read_regs(ADDR_TEMPDATA1,buf,14);

    buf[1] = read_reg(ADDR_TEMPDATA0);
    buf[0] = read_reg(ADDR_TEMPDATA1);

    buf[3] = read_reg(ADDR_ACCELDATA_X0);
    buf[2] = read_reg(ADDR_ACCELDATA_X1);

    buf[5] = read_reg(ADDR_ACCELDATA_Y0);
    buf[4] = read_reg(ADDR_ACCELDATA_Y1);

    buf[7] = read_reg(ADDR_ACCELDATA_Z0);
    buf[6] = read_reg(ADDR_ACCELDATA_Z1);

    buf[9] = read_reg(ADDR_GYRODATA_X0);
    buf[8] = read_reg(ADDR_GYRODATA_X1);

    buf[11] = read_reg(ADDR_GYRODATA_Y0);
    buf[10] = read_reg(ADDR_GYRODATA_Y1);

    buf[13] = read_reg(ADDR_GYRODATA_Z0);
    buf[12] = read_reg(ADDR_GYRODATA_Z1);

    g_icm.temp = (float)(int16_t)((buf[0] << 8) | buf[1]) / 132.48f + 25.0f;

    g_icm.acc_x  = (float)(int16_t)((uint16_t)(buf[ 2] << 8)  | (uint16_t)buf[ 3]);
    g_icm.acc_y  = (float)(int16_t)((uint16_t)(buf[ 4] << 8)  | (uint16_t)buf[ 5]);
	g_icm.acc_z  = (float)(int16_t)((uint16_t)(buf[ 6] << 8)  | (uint16_t)buf[ 7]);
	g_icm.gyro_x = (float)(int16_t)((uint16_t)(buf[ 8] << 8)  | (uint16_t)buf[ 9]);
	g_icm.gyro_y = (float)(int16_t)((uint16_t)(buf[10] << 8)  | (uint16_t)buf[11]);
    g_icm.gyro_z = (float)(int16_t)((uint16_t)(buf[12] << 8)  | (uint16_t)buf[13]);

    //sHMI_Debug_Printf("%d,%d,%d\n",acc_x,acc_y,acc_z);

    //处理数据
	//处理加速度,单位m/s^2
	if(g_icm_conf.accel_fs == SDRV_ICM_ACCELFS_2G){
		g_icm.acc_x *= ( 2.0f / 32768.0f) * M_GRAVITY;
        g_icm.acc_y *= ( 2.0f / 32768.0f) * M_GRAVITY;
        g_icm.acc_z *= ( 2.0f / 32768.0f) * M_GRAVITY;
	}else if(g_icm_conf.accel_fs == SDRV_ICM_ACCELFS_4G){
		g_icm.acc_x *= ( 4.0f / 32768.0f) * M_GRAVITY;
        g_icm.acc_y *= ( 4.0f / 32768.0f) * M_GRAVITY;
        g_icm.acc_z *= ( 4.0f / 32768.0f) * M_GRAVITY;
	}else if(g_icm_conf.accel_fs == SDRV_ICM_ACCELFS_8G){
		g_icm.acc_x *= ( 8.0f / 32768.0f) * M_GRAVITY;
        g_icm.acc_y *= ( 8.0f / 32768.0f) * M_GRAVITY;
        g_icm.acc_z *= ( 8.0f / 32768.0f) * M_GRAVITY;
	}else if(g_icm_conf.accel_fs == SDRV_ICM_ACCELFS_16G){
		g_icm.acc_x *= (16.0f / 32768.0f) * M_GRAVITY;
        g_icm.acc_y *= (16.0f / 32768.0f) * M_GRAVITY;
        g_icm.acc_z *= (16.0f / 32768.0f) * M_GRAVITY;
	}

    //处理角速度
	if(g_icm_conf.gyro_fs == SDRV_ICM_GYROFS_250DPS){
		g_icm.gyro_x *= ( 250.0f / 32768.0f);
        g_icm.gyro_y *= ( 250.0f / 32768.0f);
        g_icm.gyro_z *= ( 250.0f / 32768.0f);
	}else if(g_icm_conf.gyro_fs == SDRV_ICM_GYROFS_500DPS){
		g_icm.gyro_x *= ( 500.0f / 32768.0f);
        g_icm.gyro_y *= ( 500.0f / 32768.0f);
        g_icm.gyro_z *= ( 500.0f / 32768.0f);
	}else if(g_icm_conf.gyro_fs == SDRV_ICM_GYROFS_1000DPS){
		g_icm.gyro_x *= (1000.0f / 32768.0f);
        g_icm.gyro_y *= (1000.0f / 32768.0f);
        g_icm.gyro_z *= (1000.0f / 32768.0f);
	}else if(g_icm_conf.gyro_fs == SDRV_ICM_GYROFS_2000DPS){
		g_icm.gyro_x *= (2000.0f / 32768.0f);
        g_icm.gyro_y *= (2000.0f / 32768.0f);
        g_icm.gyro_z *= (2000.0f / 32768.0f);
	}else if(g_icm_conf.gyro_fs == SDRV_ICM_GYROFS_125DPS){
		g_icm.gyro_x *= ( 125.0f / 32768.0f);
        g_icm.gyro_y *= ( 125.0f / 32768.0f);
        g_icm.gyro_z *= ( 125.0f / 32768.0f);
	}else if(g_icm_conf.gyro_fs == SDRV_ICM_GYROFS_62D5DPS){
		g_icm.gyro_x *= (  62.5f / 32768.0f);
        g_icm.gyro_y *= (  62.5f / 32768.0f);
        g_icm.gyro_z *= (  62.5f / 32768.0f);
	}else if(g_icm_conf.gyro_fs == SDRV_ICM_GYROFS_31D25DPS){
		g_icm.gyro_x *= ( 31.25f / 32768.0f);
        g_icm.gyro_y *= ( 31.25f / 32768.0f);
        g_icm.gyro_z *= ( 31.25f / 32768.0f);
	}else if(g_icm_conf.gyro_fs == SDRV_ICM_GYROFS_15D625DPS){
		g_icm.gyro_x *= (15.625f / 32768.0f);
        g_icm.gyro_y *= (15.625f / 32768.0f);
        g_icm.gyro_z *= (15.625f / 32768.0f);
	}


}



