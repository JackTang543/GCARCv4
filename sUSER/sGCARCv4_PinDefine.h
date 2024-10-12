#pragma once

/*This file is used to define the pin assignments for the sGCARCv4*/


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
#define BI2C_SCL_Pin GPIO_PIN_8
#define BI2C_SCL_GPIO_Port GPIOB
#define BI2C_SDA_Pin GPIO_PIN_9
#define BI2C_SDA_GPIO_Port GPIOB
#define EI2C_SCL_Pin GPIO_PIN_10
#define EI2C_SCL_GPIO_Port GPIOB
#define EI2C_SDA_Pin GPIO_PIN_11
#define EI2C_SDA_GPIO_Port GPIOB


