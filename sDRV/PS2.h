#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "sGCARCv4_Def.h"
#include "sDBG_Debug.h"

#define DI   		HAL_GPIO_ReadPin(TRACK_MISO_GPIO_Port,TRACK_MISO_Pin)

#define DO_H 		HAL_GPIO_WritePin(TRACK_MOSI_GPIO_Port,TRACK_MOSI_Pin,GPIO_PIN_SET)
#define DO_L 		HAL_GPIO_WritePin(TRACK_MOSI_GPIO_Port,TRACK_MOSI_Pin,GPIO_PIN_RESET)

#define CS_H 		HAL_GPIO_WritePin(TRACK_CS_GPIO_Port,TRACK_CS_Pin,GPIO_PIN_SET)
#define CS_L 		HAL_GPIO_WritePin(TRACK_CS_GPIO_Port,TRACK_CS_Pin,GPIO_PIN_RESET)
#define CLK_H  	HAL_GPIO_WritePin(TRACK_SCK_GPIO_Port,TRACK_SCK_Pin,GPIO_PIN_SET)
#define CLK_L  	HAL_GPIO_WritePin(TRACK_SCK_GPIO_Port,TRACK_SCK_Pin,GPIO_PIN_RESET)


//These are our button constants

#define PSB_NULL				0
#define PSB_SELECT      1
#define PSB_L3          2
#define PSB_R3          3
#define PSB_START       4
#define PSB_PAD_UP      5
#define PSB_PAD_RIGHT   6
#define PSB_PAD_DOWN    7
#define PSB_PAD_LEFT    8
#define PSB_L2          9
#define PSB_R2          10
#define PSB_L1          11
#define PSB_R1          12
#define PSB_GREEN       13
#define PSB_RED         14
#define PSB_BLUE        15
#define PSB_PINK        16

#define PSB_TRIANGLE    13
#define PSB_CIRCLE      14
#define PSB_CROSS       15
#define PSB_SQUARE      16

//#define WHAMMY_BAR		8

//These are stick values
#define PSS_RX 5                //��ҡ��X������
#define PSS_RY 6
#define PSS_LX 7
#define PSS_LY 8

extern uint8_t Data[9];
extern uint16_t MASK[16];
extern uint16_t Handkey;

extern int PS2_LX,PS2_LY,PS2_RX,PS2_RY,PS2_KEY;     //PS2ң�����


void PS2_ShortPoll(void);

void vPS2Init(void);
uint8_t PS2_RedLight(void);   //�ж��Ƿ�Ϊ���ģʽ
void PS2_ReadData(void); //���ֱ�����
void PS2_Cmd(uint8_t CMD);		  //���ֱ���������
uint8_t PS2_DataKey(void);		  //����ֵ��ȡ
uint8_t PS2_AnologData(uint8_t button); //�õ�һ��ҡ�˵�ģ����
void PS2_ClearData(void);	  //������ݻ�����
void PS2_Vibration(uint8_t motor1, uint8_t motor2);//������motor1  0xFF���������أ�motor2  0x40~0xFF

void PS2_EnterConfing(void);	 //��������
void PS2_TurnOnAnalogMode(void); //����ģ����
void PS2_VibrationMode(void);    //������
void PS2_ExitConfing(void);	     //�������
void PS2_SetInit(void);		     //���ó�ʼ��
bool bReadPS2Data(void);









#ifdef __cplusplus
}
#endif










