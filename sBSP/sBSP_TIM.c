#include "sBSP_TIM.h"

#include "sDBG_Debug.h"

#include "sGCARCv4_hal_msp.h"


//first modify at 241009 PM08:17 sGCARCv4
////completed at 241009 PM08:17 sGCARCv4
//update at 241011 PM05:10
//bySightseer. inHNIP9607

/*Given the timer clock freq,target freq,and ARR value,auto calculate the required PSC value*/
#define __TIM_GET_PSC(__TIM_CLK_FREQ,__TARGET_FREQ,__TIM_ARR_VAL) \
        ((__TIM_CLK_FREQ / (__TARGET_FREQ * (__TIM_ARR_VAL + 1))) - 1)


//todo TIM2 -> 2xMotor
//CH1->ML_PWM1,CH2->ML_PWM2,CH3->MR_PWM1,CH4->MR_PWM2
TIM_HandleTypeDef g_htim2;
//TIM2 input clock frequency,the timer clock is provided by the APB1 bus(x2)
#define TIM2_CLK_FREQ  (84000000u)
//? tips: TIM2 is a 32bit timer,so the ARR,CNT,and CCR are all 32bit wide,except for the PSC.
//pwm duty accuracy=0.1%,max frequency=84KHz
const uint32_t TIM2_ARRVal = 1000 - 1;
const uint32_t TIM2_PSCVal = 0;

//todo TIM3 -> RightGMR
//CH1->GMR_RA,CH2->GMR_RB
TIM_HandleTypeDef g_htim3;

//todo TIM4 -> LeftGMR
//CH1->GMR_LA,CH2->GMR_LB
TIM_HandleTypeDef g_htim4;



//todo TIM12_CH1 -> Light
TIM_HandleTypeDef g_htim12;
#define TIM12_CLK_FREQ  (84000000u)
//pwm duty accuracy=0.1%,max frequency=84KHz
const uint32_t TIM12_ARRVal = 1000 - 1;
const uint32_t TIM12_PSCVal = 0;








void sBSP_TIM_Motor_Init(){
    //timer initialization
    g_htim2.Instance               = TIM2;
    g_htim2.Init.Prescaler         = TIM2_PSCVal;
    g_htim2.Init.CounterMode       = TIM_COUNTERMODE_UP;
    g_htim2.Init.Period            = TIM2_ARRVal;
    g_htim2.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    g_htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_PWM_Init(&g_htim2);

    //configure timer master mode:disable
    TIM_MasterConfigTypeDef master = {0};
    master.MasterOutputTrigger     = TIM_TRGO_RESET;
    master.MasterSlaveMode         = TIM_MASTERSLAVEMODE_DISABLE;
    HAL_TIMEx_MasterConfigSynchronization(&g_htim2, &master);

    //set output compare mode(PWM1)
    TIM_OC_InitTypeDef oc = {0};
    oc.OCMode                      = TIM_OCMODE_PWM1;
    oc.Pulse                       = 0;     //default not pulse(duty=0%)
    oc.OCPolarity                  = TIM_OCPOLARITY_HIGH;
    oc.OCFastMode                  = TIM_OCFAST_DISABLE;


    HAL_TIM_PWM_ConfigChannel(&g_htim2,&oc,TIM_CHANNEL_1);
    HAL_TIM_PWM_ConfigChannel(&g_htim2,&oc,TIM_CHANNEL_2);
    HAL_TIM_PWM_ConfigChannel(&g_htim2,&oc,TIM_CHANNEL_3);
    HAL_TIM_PWM_ConfigChannel(&g_htim2,&oc,TIM_CHANNEL_4);

    HAL_TIM_MspPostInit(&g_htim2);
}

void sBSP_TIM_Motor_SetPWMFreq(uint32_t freq){
    //Given ARR=999,input freq range:2Hz to 84KHz
    __HAL_TIM_SET_PRESCALER(&g_htim2,__TIM_GET_PSC(TIM2_CLK_FREQ,freq,TIM2_ARRVal));
}

void sBSP_TIM_Motor_L1SetEN(bool is_en){
    is_en ? HAL_TIM_PWM_Start(&g_htim2,TIM_CHANNEL_1) : HAL_TIM_PWM_Stop(&g_htim2,TIM_CHANNEL_1);
}

void sBSP_TIM_Motor_L2SetEN(bool is_en){
    is_en ? HAL_TIM_PWM_Start(&g_htim2,TIM_CHANNEL_2) : HAL_TIM_PWM_Stop(&g_htim2,TIM_CHANNEL_2);
}

void sBSP_TIM_Motor_R1SetEN(bool is_en){
    is_en ? HAL_TIM_PWM_Start(&g_htim2,TIM_CHANNEL_3) : HAL_TIM_PWM_Stop(&g_htim2,TIM_CHANNEL_3);
}

void sBSP_TIM_Motor_R2SetEN(bool is_en){
    is_en ? HAL_TIM_PWM_Start(&g_htim2,TIM_CHANNEL_4) : HAL_TIM_PWM_Stop(&g_htim2,TIM_CHANNEL_4);
}


//duty范围:0~100%
void sBSP_TIM_Motor_L1SetDuty(float percent){
    __HAL_TIM_SET_COMPARE(&g_htim2,TIM_CHANNEL_1,(uint32_t)(percent * 10.0f));
}

void sBSP_TIM_Motor_L2SetDuty(float percent){
    __HAL_TIM_SET_COMPARE(&g_htim2,TIM_CHANNEL_2,(uint32_t)(percent * 10.0f));
}

void sBSP_TIM_Motor_R1SetDuty(float percent){
    __HAL_TIM_SET_COMPARE(&g_htim2,TIM_CHANNEL_3,(uint32_t)(percent * 10.0f));
}

void sBSP_TIM_Motor_R2SetDuty(float percent){
    __HAL_TIM_SET_COMPARE(&g_htim2,TIM_CHANNEL_4,(uint32_t)(percent * 10.0f));
}


//tim3
void sBSP_TIM_GMRR_Init(){
    /*init the timer*/
    g_htim3.Instance               = TIM3;
    g_htim3.Init.Prescaler         = 0;
    g_htim3.Init.CounterMode       = TIM_COUNTERMODE_UP;
    g_htim3.Init.Period            = 65535;
    g_htim3.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    g_htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    
    /*init encoder mode*/
    TIM_Encoder_InitTypeDef encoder = {0};
    encoder.EncoderMode            = TIM_ENCODERMODE_TI12;      //! 4倍频模式
    encoder.IC1Polarity            = TIM_ICPOLARITY_RISING;
    encoder.IC1Selection           = TIM_ICSELECTION_DIRECTTI;
    encoder.IC1Prescaler           = TIM_ICPSC_DIV1;
    encoder.IC1Filter              = 0x7;
    encoder.IC2Polarity            = TIM_ICPOLARITY_RISING;
    encoder.IC2Selection           = TIM_ICSELECTION_DIRECTTI;
    encoder.IC2Prescaler           = TIM_ICPSC_DIV1;
    encoder.IC2Filter              = 0x7;
    if (HAL_TIM_Encoder_Init(&g_htim3, &encoder) != HAL_OK){
        Error_Handler();
    }

    /*no master mode*/
    TIM_MasterConfigTypeDef master = {0};
    master.MasterOutputTrigger = TIM_TRGO_RESET;
    master.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&g_htim3, &master) != HAL_OK){
        Error_Handler();
    }
}


inline uint32_t sBSP_TIM_GMRR_Get(){
    return __HAL_TIM_GET_COUNTER(&g_htim3);
}

inline void sBSP_TIM_GMRR_Set(uint32_t count){
    __HAL_TIM_SET_COUNTER(&g_htim3,count);
}

void sBSP_TIM_GMRR_SetEN(bool is_en){
    is_en ? HAL_TIM_Encoder_Start(&g_htim3,TIM_CHANNEL_ALL) : HAL_TIM_Encoder_Stop(&g_htim3,TIM_CHANNEL_ALL);
}


//tim4
void sBSP_TIM_GMRL_Init(){
    /*init the timer*/
    g_htim4.Instance               = TIM4;
    g_htim4.Init.Prescaler         = 0;
    g_htim4.Init.CounterMode       = TIM_COUNTERMODE_UP;
    g_htim4.Init.Period            = 65535;
    g_htim4.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    g_htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    
    /*init encoder mode*/
    TIM_Encoder_InitTypeDef encoder = {0};
    encoder.EncoderMode            = TIM_ENCODERMODE_TI12;      //! 4倍频模式
    encoder.IC1Polarity            = TIM_ICPOLARITY_RISING;
    encoder.IC1Selection           = TIM_ICSELECTION_DIRECTTI;
    encoder.IC1Prescaler           = TIM_ICPSC_DIV1;
    encoder.IC1Filter              = 0x7;
    encoder.IC2Polarity            = TIM_ICPOLARITY_RISING;
    encoder.IC2Selection           = TIM_ICSELECTION_DIRECTTI;
    encoder.IC2Prescaler           = TIM_ICPSC_DIV1;
    encoder.IC2Filter              = 0x7;
    if (HAL_TIM_Encoder_Init(&g_htim4, &encoder) != HAL_OK){
        Error_Handler();
    }

    /*no master mode*/
    TIM_MasterConfigTypeDef master = {0};
    master.MasterOutputTrigger = TIM_TRGO_RESET;
    master.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&g_htim4, &master) != HAL_OK){
        Error_Handler();
    }
}

inline uint32_t sBSP_TIM_GMRL_Get(){
    return __HAL_TIM_GET_COUNTER(&g_htim4);
}

inline void sBSP_TIM_GMRL_Set(uint32_t count){
    __HAL_TIM_SET_COUNTER(&g_htim4,count);
}

void sBSP_TIM_GMRL_SetEN(bool is_en){
    is_en ? HAL_TIM_Encoder_Start(&g_htim4,TIM_CHANNEL_ALL) : HAL_TIM_Encoder_Stop(&g_htim4,TIM_CHANNEL_ALL);
}






//tim12
void sBSP_TIM_Light_Init(){
    g_htim12.Instance               = TIM12;
    g_htim12.Init.Prescaler         = TIM12_PSCVal;
    g_htim12.Init.CounterMode       = TIM_COUNTERMODE_UP;
    g_htim12.Init.Period            = TIM12_ARRVal;
    g_htim12.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    g_htim12.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_PWM_Init(&g_htim12) != HAL_OK){
        Error_Handler();
    }

    TIM_OC_InitTypeDef oc = {0};
    oc.OCMode       = TIM_OCMODE_PWM1;
    oc.Pulse        = 0;
    oc.OCPolarity   = TIM_OCPOLARITY_HIGH;
    oc.OCFastMode   = TIM_OCFAST_DISABLE;
    if (HAL_TIM_PWM_ConfigChannel(&g_htim12, &oc, TIM_CHANNEL_1) != HAL_OK){
        Error_Handler();
    }

    HAL_TIM_MspPostInit(&g_htim12);
}

void sBSP_TIM_Light_SetPWMFreq(uint32_t freq){
    //Given ARR=999,input freq range:2Hz to 84KHz
    __HAL_TIM_SET_PRESCALER(&g_htim12,__TIM_GET_PSC(TIM12_CLK_FREQ,freq,TIM12_ARRVal));
}

void sBSP_TIM_Light_SetEN(bool is_en){
    is_en ? HAL_TIM_PWM_Start(&g_htim12,TIM_CHANNEL_1) : HAL_TIM_PWM_Stop(&g_htim12,TIM_CHANNEL_1);
}

void sBSP_TIM_Light_Set(float percent){
    if(percent > 100.0f){percent = 100.0f;}
    if(percent < 0.0f)  {percent = 0.0f;}
    __HAL_TIM_SET_COMPARE(&g_htim12,TIM_CHANNEL_1,(uint32_t)(percent * 10.0f));
}







