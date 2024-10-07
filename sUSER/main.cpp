#include "main.h"






int main(){
    HAL_Init();

    float32_t a = -1.5f;
    float32_t b = 0;

    arm_abs_f32(&a,&b,1);

    while(1){

    }
}





