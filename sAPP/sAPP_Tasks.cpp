#include "sAPP_Tasks.hpp"

#include "main.h"


void sAPP_Tasks_AHRS(void* param){
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    for(;;){
        ahrs.update();
        motor.update();



        sAPP_BlcCtrl_Handler();

        //高精确度延时10ms
        xTaskDelayUntil(&xLastWakeTime,10 / portTICK_PERIOD_MS);
    }
}


void sAPP_Tasks_BtnHdr(void* param){
    for(;;){
        sGBD_Handler();

        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
}


void sAPP_Tasks_500ms(void* param){

    for(;;){
        led.toggle();

        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}



void sAPP_Tasks_CreateAll(){
    xTaskCreate(sAPP_Tasks_AHRS         , "AHRS"       , 2048 / 4, NULL, 2, NULL);

    xTaskCreate(sAPP_Tasks_BtnHdr       , "BtnHdr"     ,  512 / 4, NULL, 1, NULL);
    xTaskCreate(sAPP_Tasks_500ms        , "500ms"      ,  512 / 4, NULL, 1, NULL);
    
}








