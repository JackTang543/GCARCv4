#include "sDRV_PS2.h"


//
#define SPI_CS_CLK_EN    __GPIOD_CLK_ENABLE
#define SPI_CS_PORT      TRACK_CS_GPIO_Port
#define SPI_CS_PIN       TRACK_CS_Pin

#include "sDBG_Debug.h"


#include "sBSP_DWT.h"


static void portSPIInit(){
    //实测64:657KHz正常运行
    sBSP_SPI_PS2_Init(SPI_BAUDRATEPRESCALER_256);
    sBSP_SPI_PS2_SetEN(1);
}

static void portDelayUs(uint16_t us){
    sBSP_DWT_Delay_us(us);
}


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


static inline uint8_t portReadByte(){
    portDelayUs(10);
    return sBSP_SPI_TRACK_RecvByte();
}

static inline void portSendByte(uint8_t byte){
    sBSP_SPI_TRACK_SendByte(byte);
    portDelayUs(10);
}

static inline uint8_t portTransferByte(uint8_t byte){
    portDelayUs(10);
    return sBSP_SPI_PS2_TransferByte(byte);
}



typedef enum{
    sDRV_PS2_MODE_RED   = 0x73,
    sDRV_PS2_MODE_GREEN = 0x41,
}sDRV_PS2_MODE_t;

sDRV_PS2_MODE_t mode;

int sDRV_PS2_Init(){
    portSPIInit();
    portCSInit();
    portSetCS(1);

    portSetCS(0);
    portSendByte(0x01);
    mode = portTransferByte(0x42);

    portSetCS(1);

    return 0;
}

//0x00 右侧小震动电机关 0xFF 开
static uint8_t LeftV  = 0x00;
static uint8_t RightV = 0x00;

sDRV_PS2_t ps2;



typedef void(*sDRV_PS2_Cb_t)(char* pReciData,uint16_t length);


void sDRV_PS2_Handler(){
    uint8_t tmp = 0;
    portSetCS(0);
    //开始通信
    portSendByte(0x01);
    //请求发送数据,此时接收到红绿灯模式
    mode = portTransferByte(0x42);
    //如果正常这个值应该是0x5A
    tmp  = portTransferByte(0x00);
    //读取KEY1组,并发送右侧小振动电机是否开启
    ps2.key1 = portTransferByte(RightV);
    //读取KEY2组
    ps2.key2 = portTransferByte(LeftV);
    //读取轴
    ps2.rightX = portTransferByte(0x00);
    ps2.rightY = portTransferByte(0x00);
    ps2.leftX = portTransferByte(0x00);
    ps2.leftY = portTransferByte(0x00);


    portSetCS(1);

    //sDBG_Debug_Printf("0x%2X,0x%2X\n", ps2.leftX,ps2.leftY);
}





