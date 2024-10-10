#include "sBSP_TIM.h"

#include "sGCARCv4_hal_msp.h"


//241009 PM08:17 sGCARCv4
//bySightseer. inHNIP9607

/*Given the timer clock freq,target freq,and ARR value,auto calculate the required PSC value*/
#define __TIM_GET_PSC(__TIM_CLK_FREQ,__TARGET_FREQ,__TIM_ARR_VAL) \
        ((__TIM_CLK_FREQ / (__TARGET_FREQ * (__TIM_ARR_VAL + 1))) - 1)


/*TIM2 -> 2xMotor*/
//CH1->ML_PWM1,CH2->ML_PWM2,CH3->MR_PWM1,CH4->MR_PWM2
TIM_HandleTypeDef g_htim2;
//TIM2 input clock frequency,the timer clock is provided by the APB1 bus(x2)
#define TIM2_CLK_FREQ  (84000000u)
//? tips: TIM2 is a 32bit timer,so the ARR,CNT,and CCR are all 32bit wide,except for the PSC.
//pwm duty accuracy=0.1%,max frequency=84KHz
const uint32_t TIM2_ARRVal = 1000 - 1;
const uint32_t TIM2_PSCVal = 0;






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








