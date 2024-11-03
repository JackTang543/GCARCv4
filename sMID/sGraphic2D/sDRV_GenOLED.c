#include "sDRV_GenOLED.h"

/** 
 * sDRV_GenOLED.c
 * 通用OLED显示驱动
 * Sightseer's General OLED Driver
 * 
 * 更新日志:
 * 
 * v1.0  2024.04.19inHNIP9607
 * 说明: 初版,只支持SH1106
 * 
 * v1.1  2024.04.20inHNIP9607
 * 说明: 第二版,提供对SH1106,SSD1306为芯片的0.91"OLED 0.96"OLED 1.3"OLED的支持
 * 
 * v1.2  2024.10.12inHNIP9607
 * 说明：改进,在文件内初始化 设置CS 设置RST
 * 
 * v1.3  2024.10.31inHNIP9607
 * 说明: 改进:优化刷屏方式,从原来的SPI带宽21MBit/s时10.1ms,优化到1.02ms,O3优化:0.52ms
 * 
 * 
 */

//伽马矫正值
#define GAMMA_VAL 2.2f

//默认使用I2C通信,启用这个宏来使用4线SPI通信
#define USE_4WSPI

//选择屏幕类型,只能选一个
//#define SH1106_1D3INCH
//#define SSD1306_0D91INCH
#define SSD1306_0D96INCH


#ifdef USE_4WSPI
    #include "sBSP_SPI.h"
#else
    #include "sBSP_I2C.h"
#endif

//选择是否使用硬件RST
#define USE_HW_RST

//选择SPI模式下DC RST的GPIO口
#define DC_GPIO             GPIOA
#define DC_GPIO_PIN         GPIO_PIN_6
#define DC_GPIO_CLK_EN      __GPIOA_CLK_ENABLE

//PB12 -> RST
#define RST_GPIO_CLK_EN      __GPIOB_CLK_ENABLE
#define RST_GPIO             GPIOB
#define RST_GPIO_PIN         GPIO_PIN_12



//选择I2C模式下的地址
#ifdef SH1106_1D3INCH
    #define SH1106_ADDR         (0x78)
    //#define SH1106_ADDR        (0x7A)
#endif

#if defined(SSD1306_0D91INCH) || defined(SSD1306_0D96INCH)
    #define SSD1306_ADDR         (0x78)
    //#define SSD1306_ADDR        (0x7A)
#endif



//******************************************一些值****************************************

#ifdef SH1106_1D3INCH
    //宏参数__VOLTAGE_VAL__:电荷泵电压
    #define PUMP_VOLT_6D4_VPP   (0b00)
    #define PUMP_VOLT_7D4_VPP   (0b01)
    #define PUMP_VOLT_8D0_VPP   (0b10)
    #define PUMP_VOLT_9D0_VPP   (0b11)
    //宏参数__SEG_DIR__:段重映射值
    #define SET_DIR_NOR_DIR     (0x00)
    #define SET_DIR_REV_DIR     (0x01)
    //宏参数__ALL_DIS__:是否使能点亮所有像素模式
    #define ENTIRE_DIS_OFF      (0x00)
    #define ENTIRE_DIS_ON       (0x01)
#endif



//******************************************命令字****************************************

#ifdef SH1106_1D3INCH
    //命令
    //设置列(竖行)的低四位地址(双字节命令和下一个一起发)
    #define  __COMM_SET_LOW_COL_ADDR(__COL_VAL__)       ((0x00) | (__COL_VAL__))
    //设置列(竖行)的高四位地址
    #define __COMM_SET_HIGH_COL_ADDR(__COL_VAL__)       ((0x10) | (__COL_VAL__))
    //设置电荷泵电压
    #define __COMM_SET_PUMP_VOLT_VAL(__VOLTAGE_VAL__)   ((0x30) | (__VOLTAGE_VAL__))
    //设置显示起始行
    #define __COMM_SET_DIS_S_LINE(__LINE_ADDR__)        ((0x40) | (__LINE_ADDR__))
    //设置对比度控制寄存器(双字节命令)
    #define __COMM_CONTRAST_CTRL_MODE_SET               (0x81)
    #define __COMM_CONTRAST_DATA_SET(__CONTRAST_VAL__)  (__CONTRAST_VAL__)
    //设置Segment重映射
    #define __COMM_SET_SEG_REMAP(__SEG_DIR__)           (0xA0 | (__SEG_DIR__))
    //设置所有所有像素亮起或熄灭
    #define __COMM_SET_ENTIRE_DIS(__ALL_DIS__)          (0xA4 | (__ALL_DIS__))
    //设置是否像素显示是否反转,参数1或0
    #define __COMM_SET_NOR_REV_DIS(__IS_REV_DIS__)      (0xA6 | (__IS_REV_DIS__))
    //设置多工比(控制显示范围的上下宽度)(双字节命令)
    #define __COMM_SET_MUL_RATION_MODE                  (0xA8)
    #define __COMM_SET_MUL_RATION_DATA(__MUL_RATIO__)   (__MUL_RATIO__)
    //设置DCDC启动或关闭(双字节命令)
    #define __COMM_SET_DCDC_OFFON                       (0xAD)
    #define __COMM_SET_DCDC_OFFON_MODE(__IS_ON__)       (0x8A | (__IS_ON__))
    //显示关闭或启动,当设置为OFF,将进入省电模式
    #define __COMM_DISPLAY_OFFON(__IS_DIS_ON__)         (0xAE | (__IS_DIS_ON__))
    //设置页地址(0~7)
    #define __COMM_SET_PAGE_ADDR(__PAGE_ADDR__)         (0xB0 | (__PAGE_ADDR__))
    //设置显示方向
    #define __COMM_COM_OUTPUT_SCAN_DIR(__DIR__)         (0xC0 | (__DIR__ << 3))
    //设置显示偏移(0~63)(改变屏幕内容的垂直起始位置)(双字节命令)
    #define __COMM_SET_DIS_OFFSET                       (0xD3)
    #define __COMM_SET_DIS_OFFSET_DATA(__DATA__)        (__DATA__)
    //设置显示时钟分频比/振荡器频率(0~15)(双字节命令)
    #define __COMM_SET_DIS_DR_OF                        (0xD5)
    #define __COMM_SET_DIS_DR_OF_DATA(__DIV__,__FREQ__) (__DIV__ | (__FREQ__ << 4))
    //设置Dis-charge/Pre-charge Period(0~15)(双字节命令)
    #define __COMM_SET_DC_PC_PERIOD                     (0xD9)
    #define __COMM_SET_DC_PC_PERIOD_DATA(__DC__,__PC__) (__PC__ | (__DC__ << 4))
    //设置Common pads hardware配置(0~1)(双字节命令)
    #define __COMM_SET_COM_PADS_HW_CONF                 (0xDA)
    #define __COMM_SET_COM_PADS_HW_CONF_DATA(__CONF__)  (0x02 | (__CONF__ << 4))
    //设置VCOM Deselect Level(0~0xFF)(双字节命令)
    #define __COMM_SET_VCOM_DESELECT_LV                 (0xDB)
    #define __COMM_SET_VCOM_DESELECT_LV_DATA(__DATA__)  (__DATA__)
    //读改写开始
    #define __COMM_RMW_BEGIN                            (0xE0)
    //读改写结束
    #define __COMM_RMW_END                              (0xEE)
    //空操作
    #define __COMM_NOP                                  (0xE3)

    //控制字
    #define __COMM_CTRL_B(__IS_2B_COMM__,__IS_DATA__)   (0x00 | (__IS_2B_COMM__ << 7) | (__IS_DATA__ << 6))
#endif

#if defined(SSD1306_0D91INCH) || defined(SSD1306_0D96INCH)
    //命令
    //设置列(竖行)的低四位地址(双字节命令和下一个一起发)
    #define  __COMM_SET_LOW_COL_ADDR(__COL_VAL__)       ((0x00) | (__COL_VAL__))
    //设置列(竖行)的高四位地址
    #define __COMM_SET_HIGH_COL_ADDR(__COL_VAL__)       ((0x10) | (__COL_VAL__))
    //设置内存寻址模式(双字节命令) 0:水平寻址 1:垂直寻址 2:页寻址
    #define __COMM_SET_MEM_ADDRING_MODE                 (0x20)
    #define __COMM_SET_MEM_ADDRING_MODE_DATA(__MODE__)  (__MODE__)
    //设置显示起始行
    #define __COMM_SET_DIS_S_LINE(__LINE_ADDR__)        ((0x40) | (__LINE_ADDR__))
    //设置对比度控制寄存器(双字节命令)
    #define __COMM_CONTRAST_CTRL_MODE_SET               (0x81)
    #define __COMM_CONTRAST_DATA_SET(__CONTRAST_VAL__)  (__CONTRAST_VAL__)
    //设置Segment重映射
    #define __COMM_SET_SEG_REMAP(__SEG_DIR__)           (0xA0 | (__SEG_DIR__))
    //设置所有所有像素亮起或熄灭
    #define __COMM_SET_ENTIRE_DIS(__ALL_DIS__)          (0xA4 | (__ALL_DIS__))
    //设置是否像素显示是否反转,参数1或0
    #define __COMM_SET_NOR_REV_DIS(__IS_REV_DIS__)      (0xA6 | (__IS_REV_DIS__))
    //设置多工比(控制显示范围的上下宽度)(双字节命令)
    #define __COMM_SET_MUL_RATION_MODE                  (0xA8)
    #define __COMM_SET_MUL_RATION_DATA(__MUL_RATIO__)   (__MUL_RATIO__)
    //设置电荷泵启动或关闭(双字节命令)
    #define __COMM_SET_PUMP_OFFON                       (0x8D)
    #define __COMM_SET_PUMP_OFFON_MODE(__IS_ON__)       (0x10 | (__IS_ON__ << 2))
    //显示关闭或启动,当设置为OFF,将进入省电模式
    #define __COMM_DISPLAY_OFFON(__IS_DIS_ON__)         (0xAE | (__IS_DIS_ON__))
    //设置页地址(0~7)
    #define __COMM_SET_PAGE_ADDR(__PAGE_ADDR__)         (0xB0 | (__PAGE_ADDR__))
    //设置显示方向
    #define __COMM_COM_OUTPUT_SCAN_DIR(__DIR__)         (0xC0 | (__DIR__ << 3))
    //设置显示偏移(0~63)(改变屏幕内容的垂直起始位置)(双字节命令)
    #define __COMM_SET_DIS_OFFSET                       (0xD3)
    #define __COMM_SET_DIS_OFFSET_DATA(__DATA__)        (__DATA__)
    //设置显示时钟分频比/振荡器频率(0~15)(双字节命令)
    #define __COMM_SET_DIS_DR_OF                        (0xD5)
    #define __COMM_SET_DIS_DR_OF_DATA(__DIV__,__FREQ__) (__DIV__ | (__FREQ__ << 4))
    //设置Dis-charge/Pre-charge Period(0~15)(双字节命令)
    #define __COMM_SET_DC_PC_PERIOD                     (0xD9)
    #define __COMM_SET_DC_PC_PERIOD_DATA(__DC__,__PC__) (__PC__ | (__DC__ << 4))
    //设置Common pads hardware配置(0~1)(双字节命令)
    #define __COMM_SET_COM_PADS_HW_CONF                 (0xDA)
    #define __COMM_SET_COM_PADS_HW_CONF_DATA(__CONF__)  (0x02 | (__CONF__ << 4))
    //设置VCOM Deselect Level(0~0xFF)(双字节命令)
    #define __COMM_SET_VCOM_DESELECT_LV                 (0xDB)
    #define __COMM_SET_VCOM_DESELECT_LV_DATA(__DATA__)  (__DATA__)

    //控制字
    #define __COMM_CTRL_B(__IS_2B_COMM__,__IS_DATA__)   (0x00 | (__IS_2B_COMM__ << 7) | (__IS_DATA__ << 6))
#endif


//伽马校正
static float gamma_correction(float percent_brightness) {
    //将输入的亮度百分比转换为0到1之间的范围
    float normalized_brightness = percent_brightness / 100.0;
    //应用伽马校正公式
    float corrected_brightness = powf(normalized_brightness, GAMMA_VAL);
    //将校正后的亮度映射回PWM占空比范围
    float pwm_duty_cycle = corrected_brightness * 100.0;
    return pwm_duty_cycle;
}


//******************************************接口****************************************

// static void delay(uint16_t ms){
//     delay(ms);
// }

//和SPI相关事宜
#ifdef USE_4WSPI
//初始化DC的GPIO口
static void init_dc_gpio(){
    //pinMode(14,OUTPUT);

    DC_GPIO_CLK_EN();
    GPIO_InitTypeDef gpio;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pin = DC_GPIO_PIN;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DC_GPIO,&gpio);
}


//设置DC电平:DC=1 data,DC=0,comm
static inline void setDC(uint8_t is_data){
    //digitalWrite(14,is_data);
    HAL_GPIO_WritePin(DC_GPIO,DC_GPIO_PIN,is_data);
}
//设置CS
static inline void setCS(uint8_t lv){
    sBSP_SPI_OLED_SetCS(lv);
}
#endif

static void init_rst_gpio(){
    //pinMode(14,OUTPUT);

    RST_GPIO_CLK_EN();
    GPIO_InitTypeDef gpio;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pin = RST_GPIO_PIN;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(RST_GPIO,&gpio);
}
//设置RST
static inline void setRST(uint8_t lv){
    HAL_GPIO_WritePin(RST_GPIO,RST_GPIO_PIN,(GPIO_PinState)lv);
}


static inline bool portGetSPIIsIdle(){
    return sBSP_SPI_OLED_IsIdle();
}



//写单字节命令
static inline void write_comm1b(uint8_t comm1){
    #ifdef USE_4WSPI
        setCS(0);
        //选择命令模式
        setDC(0);
        sBSP_SPI_OLED_SendByte(comm1);
        setCS(1);
    #else
        uint8_t seq[] = {__COMM_CTRL_B(0,0),comm1};
        #if defined(SSD1306_0D91INCH) || defined(SSD1306_0D96INCH)
            sBSP_I2C1M_SendBytes(SSD1306_ADDR,seq,2);
        #endif
        #ifdef SH1106_1D3INCH
            sBSP_I2C1M_SendBytes(SH1106_ADDR,seq,2);
        #endif
    #endif
}

//写双字节命令
static inline void write_comm2b(uint8_t comm1,uint8_t comm2){
    

    #ifdef USE_4WSPI
        setCS(0);
        setDC(0);
        sBSP_SPI_OLED_SendByte(comm1);
        sBSP_SPI_OLED_SendByte(comm2);

        //sBSP_SPI1M_SendBytes(seq,2);

        setCS(1);
    #else
        uint8_t seq[] = {__COMM_CTRL_B(1,0),comm1,__COMM_CTRL_B(0,0),comm2};
        #if defined(SSD1306_0D91INCH) || defined(SSD1306_0D96INCH)
            sBSP_I2C1M_SendBytes(SSD1306_ADDR,seq,4);
        #endif
        #ifdef SH1106_1D3INCH
            sBSP_I2C1M_SendBytes(SH1106_ADDR,seq,4);
        #endif
    #endif
}

//写数据
static inline void write_data(uint8_t data){
    #ifdef USE_4WSPI
        setCS(0);
        setDC(1);
        sBSP_SPI_OLED_SendByte(data);
        setCS(1);
    #else
        #if defined(SSD1306_0D91INCH) || defined(SSD1306_0D96INCH)
            sBSP_I2C1M_MemSendByte(SSD1306_ADDR,__COMM_CTRL_B(0,1),I2C_MEMADD_SIZE_8BIT,data);
        #endif
        #ifdef SH1106_1D3INCH
            sBSP_I2C1M_MemSendByte(SH1106_ADDR,__COMM_CTRL_B(0,1),I2C_MEMADD_SIZE_8BIT,data);
        #endif
    #endif
}

static inline void write_bytes(uint8_t* pData,uint16_t len){
    #ifdef USE_4WSPI
        setCS(0);
        setDC(1);
        sBSP_SPI_OLED_SendBytes(pData,len);
        //setCS(1);
    #else

    #endif
}


/**
  * @brief  初始化
  *
  * @param  无
  *
  * @return 返回0表示正常
  */
int8_t sDRV_GenOLED_Init(){
    //选择初始化SPI还是I2C
    #ifdef USE_4WSPI
        init_dc_gpio();
        
        if(sBSP_SPI_OLED_Init(SPI_BAUDRATEPRESCALER_4) != 0){
            return -1;
        }
        //设置默认电平
        setCS(1);
        setDC(1);
        
        #else
        if(sBSP_I2C1_Init(400000) != 0){
            return -1;
        }
    #endif

    #ifdef USE_HW_RST
        init_rst_gpio();
        setRST(1);
        HAL_Delay(1);
        setRST(0);
        HAL_Delay(1);
        setRST(1);
        HAL_Delay(50);
    #endif


    //初始化SH1106
    #ifdef SH1106_1D3INCH
        //关闭显示
        write_comm1b(__COMM_DISPLAY_OFFON(0));
        //设置列起始地址
        write_comm2b(__COMM_SET_LOW_COL_ADDR(0x02),__COMM_SET_HIGH_COL_ADDR(0x00));
        //设置显示起始行
        write_comm1b(__COMM_SET_DIS_S_LINE(0));
        //设置显示偏移
        write_comm2b(__COMM_SET_DIS_OFFSET,__COMM_SET_DIS_OFFSET_DATA(0));
        //设置起始页
        write_comm1b(__COMM_SET_PAGE_ADDR(0));
        //设置对比度
        write_comm2b(__COMM_CONTRAST_CTRL_MODE_SET,__COMM_CONTRAST_DATA_SET(0xFF));
        //设置段重映射
        write_comm1b(__COMM_SET_SEG_REMAP(1));
        //关闭全显
        write_comm1b(__COMM_SET_ENTIRE_DIS(0));
        //设置显示方向
        write_comm1b(__COMM_COM_OUTPUT_SCAN_DIR(1));
        //开启显示
        write_comm1b(__COMM_DISPLAY_OFFON(1));
        //设置正常/反向显示
        write_comm1b(__COMM_SET_NOR_REV_DIS(0));
        //设置多路复用率
        write_comm2b(__COMM_SET_MUL_RATION_MODE,__COMM_SET_MUL_RATION_DATA(0x3F));
        //设置显示偏移
        write_comm2b(__COMM_SET_DIS_OFFSET,__COMM_SET_DIS_OFFSET_DATA(0));
        //设置显示时钟分频比 振荡器频率
        write_comm2b(__COMM_SET_DIS_DR_OF,__COMM_SET_DIS_DR_OF_DATA(0,0x0F));
        //设置预充电周期
        write_comm2b(__COMM_SET_DC_PC_PERIOD,__COMM_SET_DC_PC_PERIOD_DATA(0xF,0x1));
        //设置COM引脚配置
        write_comm2b(__COMM_SET_COM_PADS_HW_CONF,__COMM_SET_COM_PADS_HW_CONF_DATA(1));
        //设置VCOMH解调电平
        write_comm2b(__COMM_SET_VCOM_DESELECT_LV,__COMM_SET_VCOM_DESELECT_LV_DATA(0x40));
        //关闭全显
        write_comm1b(__COMM_SET_ENTIRE_DIS(0));
        //设置正常/反向显示
        write_comm1b(__COMM_SET_NOR_REV_DIS(0));
        //开启显示
        write_comm1b(__COMM_DISPLAY_OFFON(1));
    #endif

    //初始化1306
    #if defined(SSD1306_0D91INCH) || defined(SSD1306_0D96INCH)
        //关闭显示
        write_comm1b(__COMM_DISPLAY_OFFON(0));
        //设置内存寻址模式:默认模式
        // write_comm2b(__COMM_SET_MEM_ADDRING_MODE,__COMM_SET_MEM_ADDRING_MODE_DATA(0x02));
        //设置内存寻址模式:页寻址模式,这样就不用每次发完1page重新修改page了,自增~,一次就能发整个屏幕
        write_comm2b(__COMM_SET_MEM_ADDRING_MODE,__COMM_SET_MEM_ADDRING_MODE_DATA(0b00));
        //设置页起始地址
        write_comm1b(__COMM_SET_PAGE_ADDR(0));
        //设置显示方向
        write_comm1b(__COMM_COM_OUTPUT_SCAN_DIR(1));
        //设置列起始地址
        write_comm2b(__COMM_SET_LOW_COL_ADDR(0x00),__COMM_SET_HIGH_COL_ADDR(0x00));
        //设置显示起始行
        write_comm1b(__COMM_SET_DIS_S_LINE(0));
        //设置对比度
        write_comm2b(__COMM_CONTRAST_CTRL_MODE_SET,__COMM_CONTRAST_DATA_SET(0xFF));
        //设置段重映射
        write_comm1b(__COMM_SET_SEG_REMAP(1));
        //设置正常/反向显示
        write_comm1b(__COMM_SET_NOR_REV_DIS(0));
        //设置多工比(就是显示屏刷新多少行)
        #ifdef SSD1306_0D96INCH
        write_comm2b(__COMM_SET_MUL_RATION_MODE,__COMM_SET_MUL_RATION_DATA(0x3F));
        #endif
        #ifdef SSD1306_0D91INCH
        write_comm2b(__COMM_SET_MUL_RATION_MODE,__COMM_SET_MUL_RATION_DATA(0x1F));
        #endif
        //设置所有所有像素亮起或熄灭(全显)
        write_comm1b(__COMM_SET_ENTIRE_DIS(0));
        //设置显示偏移
        write_comm2b(__COMM_SET_DIS_OFFSET,__COMM_SET_DIS_OFFSET_DATA(0));
        //设置显示时钟分频和振荡器频率
        write_comm2b(__COMM_SET_DIS_DR_OF,__COMM_SET_DIS_DR_OF_DATA(0,0x0F));
        //设置预充电周期
        write_comm2b(__COMM_SET_DC_PC_PERIOD,__COMM_SET_DC_PC_PERIOD_DATA(0x2,0x2));
        //设置COM的硬件连接
        #ifdef SSD1306_0D96INCH
        write_comm2b(__COMM_SET_COM_PADS_HW_CONF,__COMM_SET_COM_PADS_HW_CONF_DATA(1));
        #endif
        #ifdef SSD1306_0D91INCH
        write_comm2b(__COMM_SET_COM_PADS_HW_CONF,__COMM_SET_COM_PADS_HW_CONF_DATA(0));
        #endif
        //设置VCOMH解调电平
        write_comm2b(__COMM_SET_VCOM_DESELECT_LV,__COMM_SET_VCOM_DESELECT_LV_DATA(0x20));
        //设置DCDC使能
        write_comm2b(__COMM_SET_PUMP_OFFON,__COMM_SET_PUMP_OFFON_MODE(1));
        //开启显示
        write_comm1b(__COMM_DISPLAY_OFFON(1));
    #endif

    return 0;
}

void sDRV_GenOLED_SetBrightness(uint8_t bl){
    uint8_t val = gamma_correction((float)bl) * 2.55f;
    write_comm2b(__COMM_CONTRAST_CTRL_MODE_SET,__COMM_CONTRAST_DATA_SET(val));
}

void sDRV_GenOLED_SetShowEN(uint8_t is_show){
    write_comm1b(__COMM_DISPLAY_OFFON(!!is_show));
}

void sDRV_GenOLED_SetDisRev(uint8_t is_reverse){
    write_comm1b(__COMM_SET_NOR_REV_DIS(!!is_reverse));
}

void sDRV_GenOLED_SetHorizontalFlip(uint8_t is_flip){
    write_comm1b(__COMM_SET_SEG_REMAP(!is_flip));
}

void sDRV_GenOLED_SetVerticalFlip(uint8_t is_flip){
    write_comm1b(__COMM_COM_OUTPUT_SCAN_DIR(!is_flip));
}


/**
  * @brief  把GRAM全部发送到屏幕上(全屏刷新)
  *
  * @param  无
  *
  * @return 无
  */
void sDRV_GenOLED_UpdateScreen(uint8_t* gram){
    #ifdef SH1106_1D3INCH
        //设置起始列,起始行
        write_comm2b(__COMM_SET_LOW_COL_ADDR(2),__COMM_SET_HIGH_COL_ADDR(0));
        write_comm1b(__COMM_SET_PAGE_ADDR(page));
        //一次发送1KB,刷新整个屏幕
        write_bytes(gram,1024);
    #endif
    #ifdef SSD1306_0D96INCH
        //设置起始列,起始行
        write_comm2b(__COMM_SET_LOW_COL_ADDR(0),__COMM_SET_HIGH_COL_ADDR(0));
        write_comm1b(__COMM_SET_PAGE_ADDR(0));
        //一次发送1KB,刷新整个屏幕
        write_bytes(gram,128 * 8);
    #endif
    #ifdef SSD1306_0D91INCH
        //设置起始列,起始行
        write_comm2b(__COMM_SET_LOW_COL_ADDR(0),__COMM_SET_HIGH_COL_ADDR(0));
        write_comm1b(__COMM_SET_PAGE_ADDR(0));
        //一次发送512B,刷新整个屏幕
        write_bytes((uint8_t*)&GRAM[0],512);
    #endif
}

bool sDRV_GenOLED_IsIdle(){
    return portGetSPIIsIdle();
}








