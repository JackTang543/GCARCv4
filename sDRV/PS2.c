#include "PS2.h"


/*********************************************************     
**********************************************************/	 
#define FULL_COUNTER 1000
int PS2_LX=128,PS2_LY=128,PS2_RX=128,PS2_RY=128,PS2_KEY;     //PS2ң�����
bool bEmergencyStop = false;

#include "sBSP_DWT.h"
			
static void delay_us(uint32_t nus)
{		
	sBSP_DWT_Delay_us(nus);
}


#define DELAY_TIME  delay_us(5); 

uint16_t Handkey;	// ����ֵ��ȡ����ʱ�洢��
uint8_t Comd[2]={0x01,0x42};	//��ʼ�����������
uint8_t Data[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //���ݴ洢����
uint16_t MASK[]={
    PSB_SELECT,
    PSB_L3,
    PSB_R3 ,
    PSB_START,
    PSB_PAD_UP,
    PSB_PAD_RIGHT,
    PSB_PAD_DOWN,
    PSB_PAD_LEFT,
    PSB_L2,
    PSB_R2,
    PSB_L1,
    PSB_R1 ,
    PSB_GREEN,
    PSB_RED,
    PSB_BLUE,
    PSB_PINK
	};	//����ֵ�밴����

/**
	Init PS2 Function
	*/
void vPS2Init(void){
	PS2_SetInit();
}

//���ֱ���������
void PS2_Cmd(uint8_t CMD)
{
	volatile uint16_t ref=0x01;
	Data[1] = 0;
	for(ref=0x01;ref<0x0100;ref<<=1)
	{
		if(ref&CMD)
		{
			DO_H;                   //���һλ����λ
		}
		else DO_L;

		CLK_H;                        //ʱ������
		DELAY_TIME;
		CLK_L;
		DELAY_TIME;
		CLK_H;
		if(DI)
			Data[1] = ref|Data[1];
	}
	delay_us(16);
}
//�ж��Ƿ�Ϊ���ģʽ,0x41=ģ���̵ƣ�0x73=ģ����
//����ֵ��0�����ģʽ
//		  ����������ģʽ
uint8_t PS2_RedLight(void)
{
	CS_L;
	PS2_Cmd(Comd[0]);  //��ʼ����
	PS2_Cmd(Comd[1]);  //��������
	CS_H;
	if( Data[1] == 0X73)   return 0 ;
	else return 1;

}
//��ȡ�ֱ�����
void PS2_ReadData(void)
{
	volatile uint8_t byte=0;
	volatile uint16_t ref=0x01;
	CS_L;
	PS2_Cmd(Comd[0]);  //��ʼ����
	PS2_Cmd(Comd[1]);  //��������
	for(byte=2;byte<9;byte++)          //��ʼ��������
	{
		for(ref=0x01;ref<0x100;ref<<=1)
		{
			CLK_H;
			DELAY_TIME;
			CLK_L;
			DELAY_TIME;

		      if(DI)
		      Data[byte] = ref|Data[byte];
			CLK_H;					
		}
        delay_us(16);
	}
	CS_H;
}

//�Զ�������PS2�����ݽ��д���,ֻ������������  
//ֻ��һ����������ʱ����Ϊ0�� δ����Ϊ1
uint8_t PS2_DataKey()
{
	uint8_t index;

	PS2_ClearData();
	PS2_ReadData();

	Handkey=(Data[4]<<8)|Data[3];     //����16������  ����Ϊ0�� δ����Ϊ1
	for(index=0;index<16;index++)
	{	    
		if((Handkey&(1<<(MASK[index]-1)))==0)
		return index+1;
	}
	return 0;          //û���κΰ�������
}

//�õ�һ��ҡ�˵�ģ����	 ��Χ0~256
uint8_t PS2_AnologData(uint8_t button)
{
	return Data[button];
}

//������ݻ�����
void PS2_ClearData()
{
	uint8_t a;
	for(a=0;a<9;a++)
		Data[a]=0x00;
}
/******************************************************
Function:    void PS2_Vibration(uint8_t motor1, uint8_t motor2)
Description: �ֱ��𶯺�����
Calls:		 void PS2_Cmd(uint8_t CMD);
Input: motor1:�Ҳ�С�𶯵�� 0x00�أ�������
	   motor2:�����𶯵�� 0x40~0xFF �������ֵԽ�� ��Խ��
******************************************************/
void PS2_Vibration(uint8_t motor1, uint8_t motor2)
{
	CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  //��ʼ����
	PS2_Cmd(0x42);  //��������
	PS2_Cmd(0X00);
	PS2_Cmd(motor1);
	PS2_Cmd(motor2);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	CS_H;
	delay_us(16);  
}
//short poll
void PS2_ShortPoll(void)
{
	CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x42);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0x00);
	CS_H;
	delay_us(16);	
}
//��������
void PS2_EnterConfing(void)
{
    CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x43);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x01);
	PS2_Cmd(0x00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	CS_H;
	delay_us(16);
}
//����ģʽ����
void PS2_TurnOnAnalogMode(void)
{
	CS_L;
	PS2_Cmd(0x01);  
	PS2_Cmd(0x44);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x01); //analog=0x01;digital=0x00  �������÷���ģʽ
	PS2_Cmd(0x03); //Ox03�������ã�������ͨ��������MODE������ģʽ��
				   //0xEE�������������ã���ͨ��������MODE������ģʽ��
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	CS_H;
	delay_us(16);
}
//������
void PS2_VibrationMode(void)
{
	CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x4D);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0X01);
	CS_H;
	delay_us(16);	
}
//��ɲ���������
void PS2_ExitConfing(void)
{
    CS_L;
	delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x43);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	CS_H;
	delay_us(16);
}
//�ֱ����ó�ʼ��
void PS2_SetInit(void)
{
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_EnterConfing();		//��������ģʽ
	PS2_TurnOnAnalogMode();	//�����̵ơ�����ģʽ����ѡ���Ƿ񱣴�
//	PS2_VibrationMode();	//������ģʽ
	PS2_ExitConfing();		//��ɲ���������
}

bool bReadPS2Data(void){
	static float fLineScale=0.5f,fAngleScale = 0.5f;
	static int16_t sAngle = 0;
	static bool bKeyReleaseFlag = true;
	int iThreshold =2;
	float fLineX,fAngleZ;
  float LX,LY,RX;
	
	PS2_KEY=PS2_DataKey();
	PS2_LX=PS2_AnologData(PSS_LX);    //PS2���ݲɼ�    
	PS2_LY=PS2_AnologData(PSS_LY);
	PS2_RX=PS2_AnologData(PSS_RX);
	PS2_RY=PS2_AnologData(PSS_RY);

	//sDBG_Debug_Printf("%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x\r\n",Data[0],Data[1],Data[2],Data[3],Data[4],Data[5],Data[6],Data[7],Data[8]);

	
	if((PS2_KEY == PSB_R2) || ((PS2_KEY == PSB_L2))){
		bEmergencyStop = true;
	}
	else{
		bEmergencyStop = false;
	}
	
	if(bKeyReleaseFlag){
		switch (PS2_KEY){
			case PSB_TRIANGLE:
				fLineScale += 0.1f;
				break;
			case PSB_CIRCLE:
				fAngleScale += 0.1f;
				break;
			case PSB_CROSS:
				fLineScale -= 0.1f;
				break;
			case PSB_SQUARE:
				fAngleScale -= 0.1f;
				break;
			case PSB_SELECT:
				fLineScale = 0.5f;
				fAngleScale = 0.5f;
				break;
			case PSB_PAD_RIGHT:
				sAngle-=10;
			break;
			case PSB_PAD_LEFT:
				sAngle+=10;
			break;
			default:
				break;
		}	
	}
	if(PSB_NULL == PS2_KEY)
		bKeyReleaseFlag = true;
	else
		bKeyReleaseFlag = false;
	
	if(fLineScale < 0.1f) fLineScale = 0.1f;
	else if(fLineScale > 1.0f) fLineScale = 1.0f;
	if(fAngleScale < 0.1f) fAngleScale = 0.1f;
	else 	if(fAngleScale > 1.0f) fAngleScale = 1.0f;
	LY=PS2_LY-128;     //����ƫ��
	LX=PS2_LX-128;
	RX=PS2_RX-128;
	if(PS2_LY == PS2_LX && (PS2_RY == PS2_RX)){
		return false;
	}
	if(LY>-iThreshold && LY<iThreshold)	LY=0.0f;   //С�Ƕ���Ϊ���� ��ֹ���������쳣
	if(RX>-iThreshold && RX<iThreshold)	RX=0.0f;
	if(LX>-iThreshold && LX<iThreshold)	LX=0.0f;
	if(LY != 0.0f || RX !=0.0f || LX != 0.0f){
		sDBG_Debug_Printf("%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x\r\n",Data[0],Data[1],Data[2],Data[3],Data[4],Data[5],Data[6],Data[7],Data[8]);

		return true;
	}

	
	return false;
}
















