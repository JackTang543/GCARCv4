#include "sLib.h"



//把一个浮点数从一个范围转换到另一个范围
float inline sLib_Fmap(float x, float in_min, float in_max, float out_min, float out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//把一个整数从一个范围转换到另一个范围
uint32_t inline sLib_Imap(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//快速求平方根倒数
float sLib_InvSqrt(float x){
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}

//从气压转换到高度,返回单位m
float sLib_Press2Alt(float press_pa){
    //匿名科创的算法,返回m
    float x = (101000.0f - press_pa) / 1000.0f;
    return 0.82f * x * x * x + 0.9f * x * 100.0f;
}

//比较两个浮点数的大小,可以用于qsort的比较函数,返回1: a>b, 0: a==b(可能不精确), -1: a<b
int sLib_FCmp(const void *a, const void *b){
    float fa = *(const float*)a;
    float fb = *(const float*)b;
    return (fa > fb) - (fa < fb);
}

//判断两个浮点数是否相等,返回1: 相等, 0: 不相等
int inline sLib_FIsEqual(float fa, float fb){
    //如果差值小于FLT_EPSILON,则认为两数相等
    if(fabs(fa - fb) < FLT_EPSILON){
        return 1;  //相等
    }else{
        return 0;  //不相等
    }
}

//获得航向角的差值
float inline sLib_GetHeadingDiff(float heading1, float heading2){
    float diff = heading1 - heading2;
    while (diff < -180.0) diff += 360.0;
    while (diff > 180.0) diff -= 360.0;
    return diff;
}

//浮点数限幅
void inline sLib_FLimit(float *val, float min, float max){
    if(*val > max) *val = max;
    if(*val < min) *val = min;
}


//伽马校正,把输入的亮度百分比0~100% -> 0~100PWM
float sLib_GammaCorrect(float percent_brightness){
    #define GAMMA_VAL 2.2f
    //将输入的亮度百分比转换为0到1之间的范围
    float normalized_brightness = percent_brightness / 100.0;
    //应用伽马校正公式
    float corrected_brightness = powf(normalized_brightness, GAMMA_VAL);
    //将校正后的亮度映射回PWM占空比范围
    float pwm_duty_cycle = corrected_brightness * 100.0;
    return pwm_duty_cycle;
}




