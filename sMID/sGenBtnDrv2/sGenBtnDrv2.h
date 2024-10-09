/* sHMI_GenBtnDrv2.h
 * Sightseer's General Button Driver2 通用按键驱动模块
 * 用于驱动按键,读取按键单机双击长按等事件
 * 详见.c文件
 * 
 * 知乎:   https://www.zhihu.com/people/bad-boy17
 * B站:    https://space.bilibili.com/322702901
 * GitHub: https://github.com/JackTang543
 * QQ1583031618 
 * 
 * TIME:2024.01.31 By Sightseer in home.
 */

#ifndef __S_GENBTNDRV2_H__
#define __S_GENBTNDRV2_H__


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"{
#endif

//电平变化标志
typedef enum{
    lv_mark_skip = 0,   //无/跳过
    lv_mark_no_chrg,    //未改变
    lv_mark_rising,     //上升沿
    lv_mark_falling     //下降沿
}lv_mark_t;

//状态机的状态
typedef enum{
    fsm_idle = 0,       //空闲态
    fsm_press,          //刚按下
    fsm_pressed,        //按下了
    fsm_release,        //刚松手
    fsm_dpress,         //双击按下了
    fsm_waitdp,         //等待双击中
    fsm_dp_release      //双击松手了
}fsm_state_t;

//长按模式
typedef enum{
    lp_disable = 0,     //禁用长按
    lp_tpres_single,    //按下超时就触发,触发一次
    lp_tpres_loop,      //按下就触发,循环触发
    lp_trlsd_single     //松手才触发,触发一次
}lp_mode_t;

//双击模式,禁用双击会获得更短的两次单击间隔时间,就是能单击得更快
typedef enum{
    dp_disable = 0,     //禁用双击
    dp_enable,          //使能双击
}dp_mode_t;

//单击模式
typedef enum{
    sp_tpres = 0,       //单击按下触发单击
    sp_tpres_to_lp,     //按下触发单击后还可进入长按
    sp_trlsd,           //单击松手触发单击
}sp_mode_t;

//事件标志位
typedef enum{
    ev_null = 0,        //无事件
    ev_pres,            //press:按下
    ev_lp_loop,         //longpress loop:长按循环触发
    ev_lp,              //longpress:长按
    ev_rlsd,            //release:松手
    ev_lp_rlsd,         //longpress release:长按松手
    ev_dp,              //doublepress:双击按下
    ev_dp_rlsd          //doublepress release:双击松手
}ev_flag_t;

//电平反转
typedef enum{
    lv_non_reverse = 0, //电平不反转,低电平空闲
    lv_reverse          //电平反转  ,高电平空闲
}lv_reverse_t;

// //condition:转换条件
// typedef enum{
//     cdtn_NON = 0,       //无操作 ret true
//     cdtn_01,            //具体参考状态转移表
//     cdtn_02,
//     cdtn_03,
//     cdtn_04,
//     cdtn_05,
//     cdtn_06,
//     cdtn_07,
//     cdtn_08,
//     cdtn_09,
//     cdtn_10,
//     cdtn_11,
//     cdtn_CLR            //清空变量值
// }cdtn_t;

//condition:转换条件
typedef enum{
    cdtn_NON = 0,       //无操作 ret true
    cdtn_01,            //具体参考状态转移表
    cdtn_02,
    cdtn_03,
    cdtn_04,
    cdtn_05,
    cdtn_06,
    cdtn_07,
    cdtn_08,
    cdtn_09,
    cdtn_10,
    cdtn_11,
    cdtn_12,
    cdtn_13,
    cdtn_14,
    cdtn_CLR            //清空变量值
}cdtn_t;

//用于存储每个按键的数据
typedef struct{
    bool             lv_curr      : 1  ;     //当前按键电平
    bool             lv_prev      : 1  ;     //上次按键电平
    dp_mode_t        dp_mode      : 2  ;     //init:双击触发的模式,三种选项
    bool             en           : 1  ;     //init:按键使能,为1这个按键才工作
    lv_reverse_t     lv_rev       : 1  ;     //init:电平是否反转
    lv_mark_t        edge_mark    : 2  ;     //电平边沿
    //以上8bit(1字节
    ev_flag_t        ev_flag      : 3  ;     //事件触发标志
    fsm_state_t      fsm_state    : 3  ;     //用于fsm切换状态
    bool             lp_triged_fg : 1  ;     //长按触发过了,就不再触发了
    bool             placeholder  : 1  ;     //少一个bit,占位符
    //以上8bit(1字节
    uint8_t          id           : 8  ;     //init:按键的id,最大256个按键(id:0到255)
    uint16_t         lp_loop_pridt: 12 ;     //init:长按循环触发事件的时间间隔,最大4.095s
    uint16_t         lp_trig_waitt: 12 ;     //init:从按下到触发长按所需要的最小时间,最大4.095s
    uint16_t         dp_prid_waitt: 12 ;     //init:双击最大间隔等待时间,最大4.095s
    uint32_t         t_next_loop  : 32 ;     //时间戳:记录下一次触发循环长按的SysTick
    uint32_t         t_next_press : 32 ;     //时间戳:记录第二次按下的SysTick
    uint32_t         t_press      : 32 ;     //时间戳:记录第一次按键按下的SysTick
    uint32_t         t_release    : 32 ;     //时间戳:记录松手的SysTick
    uint32_t         t_dpress     : 32 ;     //时间戳:记录双击按下的SysTick
    //以上205bit(25字节多5bit)
    //以上一共28字节,符合4字节对齐
}btn_t;

//按键初始化结构体
typedef struct{
    dp_mode_t        dp_mode      : 2  ;     //双击触发的模式,三种选项
    bool             en           : 1  ;     //按键使能,为1这个按键才工作
    lv_reverse_t     lv_rev       : 1  ;     //init:电平是否反转
    uint16_t         lp_loop_pridt: 12 ;     //长按循环触发事件的时间间隔,最大4.095s
    uint16_t         lp_trig_waitt: 12 ;     //从按下到触发长按所需要的最小时间,最大4.095s
    uint16_t         dp_prid_waitt: 12 ;     //双击最大间隔等待时间,最大4.095s
    //初始化结构体40bit(5字节)
}btn_init_t;

//用于状态转移表
typedef struct{
    fsm_state_t state_prev;                  //上态
    fsm_state_t state_tran;                  //次态
    cdtn_t      cdtn;                        //条件
    ev_flag_t   trig_ev;                     //需要触发的事件
}fsm_t;


//获取按键电平
typedef bool (*btn_lv_get_cb)(uint8_t btn_id);

//用户按键触发回调
typedef void (*btn_trig_cb)(uint8_t btn_id,ev_flag_t btn_ev);

//滴答定时器回调
typedef uint32_t (*btn_tick_func_cb)(void);


int8_t sGBD_Init(btn_lv_get_cb lv_get_f,btn_trig_cb trig_f,btn_tick_func_cb tick_f);
int8_t sGBD_SetBtnMode(uint8_t btn_id,btn_init_t *btn_init);
int8_t sGBD_SetAllBtnMode(btn_init_t *btn_init);
int8_t sGBD_SetBtnEnable(uint8_t btn_id,bool btn_en);
int8_t sGBD_SetAllBtnEnable(bool btn_en);
void sGBD_Handler();

#ifdef __cplusplus
}
#endif

#endif
