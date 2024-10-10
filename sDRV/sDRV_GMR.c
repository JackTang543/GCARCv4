// #include "sDRV_GMR.h"


// //TIM3 CH1 CH2 -> Left电机
// //TIM4 CH1 CH2 -> Right电机

// //500线GMR转一圈30K个脉冲
// //好像不能使用双边沿模式,也就是四倍频模式

// #define GMR_ROUND_PLUSE     (30000)


// float right_rpm;
// float left_rpm;

// void sDRV_GMR_Init(){
//     sBSP_TIM3_Init();
//     sBSP_TIM3_EnSetEN(1);
//     sBSP_TIM3_EnSet(30000);

//     sBSP_TIM4_Init();
//     sBSP_TIM4_EnSetEN(1);
//     sBSP_TIM4_EnSet(30000);
// }


// float sDRV_GMR_GetLeftRPM(){
//     //! 两个电机方向是相反的
//     return -left_rpm;
// }

// float sDRV_GMR_GetRightRPM(){
//     return right_rpm;
// }

// //10ms一次
// void sDRV_GMR_Handler(){
//     #define DT_MS (10.0f)

//     //Right
//     right_rpm = (float)sBSP_TIM4_EnGet() - 30000.0f;
//     right_rpm *= (1000.0f / DT_MS);
//     right_rpm = right_rpm / GMR_ROUND_PLUSE * 60 * 0.5;
//     sBSP_TIM4_EnSet(30000);

//     //Left
//     left_rpm = (float)sBSP_TIM3_EnGet() - 30000.0f;
//     left_rpm *= (1000.0f / DT_MS);
//     left_rpm = left_rpm / GMR_ROUND_PLUSE * 60 * 0.5;
//     sBSP_TIM3_EnSet(30000);

//     #undef DT_MS
// }




