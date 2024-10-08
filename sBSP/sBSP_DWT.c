#include "sBSP_DWT.h"

#ifdef  __SBSP_DWT_EN__


//sBSP_DWT.c
//使用Cortex-M3及以上内核中的调试单元DWT进行时间测量和精确延时
//测试平台STM32F103VET6 HAL库
//By Sightseer. HNIP9607
//2023.12.30

//保存1us内的计数值
uint32_t us_period;
//保存1ms内的计数值
uint32_t ms_period;
//用于保存us级延时的数据
uint32_t delay_us_val_begin;
uint32_t delay_us_val_end;
//用于保存ms级延时的数据
uint32_t delay_ms_val_begin;
uint32_t delay_ms_val_end;
//用于保存测量值
uint32_t measure_val_begin;
uint32_t measure_val_end;

/*@brief  用于初始化DWT外设
*
* @param  uint32_t freq:传入系统主频(Hz)
*
* @return 无
*/
void sBSP_DWT_Init(uint32_t freq){
    //使能DWT外设,置DEMCR寄存器的TRCENA位为1
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    //清空CYCCNT
    DWT->CYCCNT = 0;
    //写DWTCR寄存器的CYCCNTTENA位为1,启动CYCCNT计数
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    
    //计算系统时钟周期,以算出1us和1ms里的DWT增加的值
    uint32_t sys_clk_freq = freq;
    us_period = sys_clk_freq * 0.000001;
    ms_period = sys_clk_freq * 0.001;
}
/*@brief  使用DWT微秒级延时
*
* @param  uint32_t time_us:微秒数
*
* @return 无
*/
void sBSP_DWT_Delay_us(uint32_t time_us){
    delay_us_val_begin = DWT->CYCCNT;
    delay_us_val_end = delay_us_val_begin + (time_us * us_period);
    while(DWT->CYCCNT < delay_us_val_end){__NOP();}
}
/*@brief  使用DWT毫秒级延时
*
* @param  uint32_t time_ms:毫秒数
*
* @return 无
*/
void sBSP_DWT_Delay_ms(uint32_t time_ms){
    delay_ms_val_begin = DWT->CYCCNT;
    delay_ms_val_end = delay_ms_val_begin + (time_ms * ms_period);
    while(DWT->CYCCNT < delay_ms_val_end){__NOP();}
}
/*@brief  开始DWT时间测量,注意测量时间不要让DWT->CYCCNT溢出
*
* @param  无
*
* @return 无
*/
inline void sBSP_DWT_MeasureStart(){
    measure_val_begin = DWT->CYCCNT;
}
/*@brief  DWT时间测量结束
*
* @param  无
*
* @return 无
*/
inline void sBSP_DWT_MeasureEnd(){
    measure_val_end = DWT->CYCCNT;
}
/*@brief  获取DWT时间测量的微秒值
*
* @param  无
*
* @return uint32_t:微秒
*/
uint32_t sBSP_DWT_GetMeasure_us(){
    //处理发生边界溢出的情况
    if(measure_val_end <= measure_val_begin){
        return ((0xFFFFFFFF - measure_val_begin) + measure_val_end) / us_period;
    }//正常情况
    else{
        return (measure_val_end - measure_val_begin) / us_period;
    }
}

#endif

