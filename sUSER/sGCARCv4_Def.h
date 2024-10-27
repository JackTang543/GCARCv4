#pragma once

/*This file is used to define the pin assignments for the sGCARCv4*/



/*指定变量或函数存放的区域,注意看分散加载文件*/
//attribute:Normal memory
#define IRAM1_ATTR     __attribute__((section("IRAM1")))
//attribute:Core coupled memory
#define IRAM2_ATTR     __attribute__((section("IRAM2")))
//attribute:flash memory
#define IROM_ATTR      __attribute__((section("IROM")))
//attribute:手动设置对齐
#define ALIGN_ATTR(__BYTES) __attribute__((aligned(__BYTES)))


/*User LED*/
#define USER_LED_Pin        GPIO_PIN_13
#define USER_LED_GPIO_Port  GPIOC

/*Motor PWM*/
#define ML_PWM1_Pin         GPIO_PIN_0
#define ML_PWM1_GPIO_Port   GPIOA
#define ML_PWM2_Pin         GPIO_PIN_1
#define ML_PWM2_GPIO_Port   GPIOA
#define MR_PWM1_Pin         GPIO_PIN_2
#define MR_PWM1_GPIO_Port   GPIOA
#define MR_PWM2_Pin         GPIO_PIN_3
#define MR_PWM2_GPIO_Port   GPIOA

/*GMR*/
#define GMR_RA_Pin          GPIO_PIN_4
#define GMR_RA_GPIO_Port    GPIOB
#define GMR_RB_Pin          GPIO_PIN_5
#define GMR_RB_GPIO_Port    GPIOB
#define GMR_LA_Pin          GPIO_PIN_6
#define GMR_LA_GPIO_Port    GPIOB
#define GMR_LB_Pin          GPIO_PIN_7
#define GMR_LB_GPIO_Port    GPIOB

/*Light*/
#define LIGHT_Pin           GPIO_PIN_14
#define LIGHT_GPIO_Port     GPIOB

/*I2C*/
#define BI2C_SCL_Pin        GPIO_PIN_8
#define BI2C_SCL_GPIO_Port  GPIOB
#define BI2C_SDA_Pin        GPIO_PIN_9
#define BI2C_SDA_GPIO_Port  GPIOB
#define EI2C_SCL_Pin        GPIO_PIN_10
#define EI2C_SCL_GPIO_Port  GPIOB
#define EI2C_SDA_Pin        GPIO_PIN_11
#define EI2C_SDA_GPIO_Port  GPIOB

/*SPI1 -> OLED*/
#define OLED_SCK_GPIO_Port      GPIOA
#define OLED_SCK_Pin            GPIO_PIN_5
#define OLED_DC_GPIO_Port       GPIOA
#define OLED_DC_Pin             GPIO_PIN_6
#define OLED_SDA_GPIO_Port      GPIOA
#define OLED_SDA_Pin            GPIO_PIN_7
#define OLED_CS_GPIO_Port       GPIOA
#define OLED_CS_Pin             GPIO_PIN_15

/*SPI2 -> IMU*/
#define IMU_SCK_GPIO_Port       GPIOB
#define IMU_SCK_Pin             GPIO_PIN_13
#define IMU_MISO_GPIO_Port      GPIOC
#define IMU_MISO_Pin            GPIO_PIN_2
#define IMU_MOSI_GPIO_Port      GPIOC
#define IMU_MOSI_Pin            GPIO_PIN_3

/*SPI3 -> TRACK*/
#define TRACK_SCK_GPIO_Port     GPIOC
#define TRACK_SCK_Pin           GPIO_PIN_10
#define TRACK_MISO_GPIO_Port    GPIOC
#define TRACK_MISO_Pin          GPIO_PIN_11
#define TRACK_MOSI_GPIO_Port    GPIOC
#define TRACK_MOSI_Pin          GPIO_PIN_12
#define TRACK_CS_GPIO_Port      GPIOD
#define TRACK_CS_Pin            GPIO_PIN_2

/*IMU CS*/
#define ICM_CS_Pin GPIO_PIN_0
#define ICM_CS_GPIO_Port GPIOC
#define LIS3_CS_Pin GPIO_PIN_1
#define LIS3_CS_GPIO_Port GPIOC

/*VBAT ADC*/
#define VBAT_ADC_Pin GPIO_PIN_4
#define VBAT_ADC_GPIO_Port GPIOA

/*UART1 Debug*/
#define DEBUG_TX_Pin GPIO_PIN_9
#define DEBUG_TX_GPIO_Port GPIOA
#define DEBUG_RX_Pin GPIO_PIN_10
#define DEBUG_RX_GPIO_Port GPIOA

/*UART6 Top*/
#define TOP_TX_Pin GPIO_PIN_6
#define TOP_TX_GPIO_Port GPIOC
#define TOP_RX_Pin GPIO_PIN_7
#define TOP_RX_GPIO_Port GPIOC

