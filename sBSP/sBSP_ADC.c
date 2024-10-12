#include "sBSP_ADC.h"

#include "sDBG_Debug.h"


ADC_HandleTypeDef g_hadc1;


void sBSP_ADC_Init(){
    g_hadc1.Instance = ADC1;
    g_hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
    g_hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    g_hadc1.Init.ScanConvMode = DISABLE;
    g_hadc1.Init.ContinuousConvMode = DISABLE;
    g_hadc1.Init.DiscontinuousConvMode = DISABLE;
    g_hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    g_hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    g_hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    g_hadc1.Init.NbrOfConversion = 1;
    g_hadc1.Init.DMAContinuousRequests = DISABLE;
    g_hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    if (HAL_ADC_Init(&g_hadc1) != HAL_OK)
    {
        Error_Handler();
    }

    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = ADC_CHANNEL_4;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
    if (HAL_ADC_ConfigChannel(&g_hadc1, &sConfig) != HAL_OK){
        Error_Handler();
    }


}



float sBSP_ADC_GetBatVolt(){
    HAL_ADC_Start(&g_hadc1);
    HAL_ADC_PollForConversion(&g_hadc1,10);
    HAL_ADC_Stop(&g_hadc1);
    float volt = HAL_ADC_GetValue(&g_hadc1) / 4095.0f * 3300.0f / 1000.0f;
    volt /= 0.1803f;
    return volt;
}

