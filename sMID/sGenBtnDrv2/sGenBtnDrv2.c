/* sHMI_GenBtnDrv.c
 * General Button Driver v2
 * 通用按键驱动v2
 * 
 * 功    能 : 用于驱动按键,获取按键触发事件
 *
 * 特    点 : 1. 类linux kernel命名风格,纯C语言语法,模块和上下层充分解耦合.
 *            2. 支持最大255个按键(理论无限个).
 *            3. 每个按键都可以设置是否启用,单独设置长按,双击的触发时间,电平是否反转.
 *            4. 资源性能开销低,每个按键只占用28Byte RAM,使用状态机进行驱动.
 *            5. 没有使用Delay和动态内存,整体基于时间戳判断,非常适合低性能的嵌入式平台.
 *            6. 本模块可以用在STM32,51,Arduino等所有嵌入式平台,满足多种项目的按键驱动需求.
 *            7. 几乎不需要对模块代码进行任何修改即可使用.
 *            8. 对RTOS应用具有良好的支持.
 *
 * 提    示 : 1. 本模块建议放在人机交互层.
 *            2. 本文件的返回值说明(int8_t): 返回0:正常,返回非0(-1等):错误.
 *            3. 空闲时按键电平默认值反转(lv_reverse_t)说明: 
 *               如果发现按键按下就一直触发长按循环事件,初始化里反转这个位.
 *            4. 转换条件(cdtn_t)说明:
 * 
 *            5. 默认最大16个按键,可更改BTN_NUM宏进行修改
 *          
 * 使用方法 : 1.创建一个回调函数用于给sGBD2读取按键电平 eg:
 *              bool get_lv(uint8_t btn_id){
 *                  if(btn_id == 0){
 *                      return !!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);
 *                  }
 *                  else if(btn_id == 1){
 *                      return !!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1);
 *                  }
 *              return false;
 *              }
 *            2.创建一个回调函数用于sGBD2触发事件时通知用户 eg:
 *              void trig(uint8_t btn_id,ev_flag_t btn_ev){...}
 *            3.创建一个初始化结构体,和HAL库类似:btn_init_t btn_init;
 *            4.对btn_init进行配置 eg: 
 *              btn_init.en = 1;                //使能此按键
 *              btn_init.lv_rev = lv_reverse;   //空闲时的电平反转
 *              btn_init.dp_mode = dp_disable;  //禁用双击,可提高连续单击速度
 *              btn_init.lp_loop_pridt = 500;   //设置长按循环触发间隔每500ms触发一次
 *              btn_init.lp_trig_waitt = 2000;  //设置长按触发时间2000ms
 *              btn_init.dp_prid_waitt = 200;   //设置最大等待双击时间
 *              sGBD_SetAllBtnEnable(1);        //设置所有按键使能(默认设置16个按键)
 *              sGBD_SetAllBtnMode(&btn_init);  //装载btn_init的配置参数
 *            5.如果想要其中有几个按键模式不一样,可以参考HAL库初始化GPIO的操作:
 *              btn_init.dp_mode = dp_enable;   //设置双击使能
 *              btn_init.lp_loop_pridt = 200;   //长按循环触发时间为200ms一次
 *              sGBD_SetBtnMode(1,&btn_init);   //装载参数
 *            6.使用sGBD_Init(get_lv,trig,HAL_GetTick);初始化sGBD2
 *            7.周期性调用处理函数:sGBD_Handler();
 * 
 *
 * 知乎:   https://www.zhihu.com/people/bad-boy17
 * B站:    https://space.bilibili.com/322702901
 * GitHub: https://github.com/JackTang543
 * QQ1583031618        
 *     
 * By Sightseer. in home. 2024.01.31
 */


//v2.1 修复btn_init.lp_loop_pridt长按触发写错了的问题
//v2.2 修复双击之前触发单击的Bug 2024.06.17
//v2.3 修复启用双击后长按循环触发松手后会再触发一次按键按下的bug 2024.07.17电赛加油

#include "sGenBtnDrv2.h"


//设置按键数量
#define BTN_NUM 4

//按键信息保存
btn_t btn[BTN_NUM];

//状态机的状态转移表
fsm_t fsm[] = {
    //从fsm_press状态转换到fsm_pressed状态需要条件cdtn_12,这个状态切换触发事件ev_pres
    {fsm_idle       ,fsm_press      ,cdtn_01 ,ev_null    },
    {fsm_press      ,fsm_pressed    ,cdtn_12 ,ev_pres    },
    {fsm_press      ,fsm_pressed    ,cdtn_02 ,ev_null    },
    {fsm_pressed    ,fsm_pressed    ,cdtn_03 ,ev_lp      },
    {fsm_pressed    ,fsm_pressed    ,cdtn_04 ,ev_lp_loop },
    {fsm_pressed    ,fsm_release    ,cdtn_05 ,ev_null    },
    {fsm_release    ,fsm_release    ,cdtn_06 ,ev_rlsd    },
    {fsm_release    ,fsm_idle       ,cdtn_07 ,ev_lp_rlsd },
    {fsm_release    ,fsm_waitdp     ,cdtn_08 ,ev_null    },
    {fsm_waitdp     ,fsm_idle       ,cdtn_13 ,ev_pres    },
    {fsm_waitdp     ,fsm_idle       ,cdtn_09 ,ev_null    },
    {fsm_waitdp     ,fsm_dpress     ,cdtn_10 ,ev_dp      },
    {fsm_dpress     ,fsm_dp_release ,cdtn_11 ,ev_dp_rlsd },
    {fsm_dp_release ,fsm_idle       ,cdtn_CLR,ev_null    },
    {fsm_release    ,fsm_idle       ,cdtn_CLR,ev_null    },
};

//获取按键电平回调
btn_lv_get_cb btn_lv_get_f;
//事件触发回调
btn_trig_cb btn_trig_f;
//滴答定时器回调
btn_tick_func_cb btn_tick_func_f;


/*@brief  初始化
*
* @param  btn_lv_get_cb    : 注册获取按键电平回调
* @param  btn_trig_cb      : 注册事件触发回调
* @param  btn_tick_func_cb : 注册滴答定时器回调
*
* @return int8_t           : OK返回0, Error返回-1
*/
int8_t sGBD_Init(btn_lv_get_cb lv_get_f,btn_trig_cb trig_f,btn_tick_func_cb tick_f){
    if((lv_get_f == NULL) || (trig_f == NULL) || (tick_f == NULL)){
        return -1;
    }
    btn_lv_get_f    = lv_get_f;
    btn_trig_f      = trig_f;
    btn_tick_func_f = tick_f;

    return 0;
}

/*@brief  设置单个按键模式
*
* @param  uint8_t     : 要设置的按键的ID
* @param  btn_init_t  : 设置按键的结构体
*
* @return int8_t      : OK返回0, Error返回-1
*/
int8_t sGBD_SetBtnMode(uint8_t btn_id,btn_init_t *btn_init){
    if((btn_id > BTN_NUM) || (btn_init == NULL)){return -1;}
    btn[btn_id].dp_mode       = btn_init->dp_mode;
    btn[btn_id].en            = btn_init->en;
    btn[btn_id].lv_rev        = btn_init->lv_rev;
    btn[btn_id].lp_loop_pridt = btn_init->lp_loop_pridt;
    btn[btn_id].lp_trig_waitt = btn_init->lp_trig_waitt;
    btn[btn_id].dp_prid_waitt = btn_init->dp_prid_waitt;
    return 0;
}

/*@brief  设置所有按键模式
*
* @param  btn_init_t  : 设置按键的结构体
*
* @return int8_t      : OK返回0, Error返回-1
*/
int8_t sGBD_SetAllBtnMode(btn_init_t *btn_init){
    if(btn_init == NULL){return -1;}
    for(uint8_t id = 0; id < BTN_NUM; id++){
        sGBD_SetBtnMode(id,btn_init);
        btn[id].lv_curr = 0;
        btn[id].lv_prev = 0;
        btn[id].ev_flag = ev_null;
        btn[id].edge_mark = lv_mark_skip;
        btn[id].fsm_state = fsm_idle;
        btn[id].lp_triged_fg = 0;
    }
    return 0;
}

/*@brief  设置单个按键的使能
*
* @param  uint8_t     : 要设置的按键的ID
* @param  bool        : 按键是否使能
*
* @return int8_t      : OK返回0, Error返回-1
*/
int8_t sGBD_SetBtnEnable(uint8_t btn_id,bool btn_en){
    if(btn_id > BTN_NUM){return -1;}
    btn[btn_id].en = btn_en;
    return 0;
}

/*@brief  设置所有按键的使能
*
* @param  bool        : 按键是否使能
*
* @return int8_t      : OK返回0
*/
int8_t sGBD_SetAllBtnEnable(bool btn_en){
    for(uint8_t id = 0; id < BTN_NUM; id++){
        btn[id].en = btn_en;
    }
    return 0;
}

//电平处理
static void Level_Handler(uint8_t id){
    //上一次的状态等于这一次的状态
    btn[id].lv_prev = btn[id].lv_curr;
    //电平反转处理
    if(btn[id].lv_rev == lv_reverse){
        //这一次的状态由读取到的电平决定
        btn[id].lv_curr =  !btn_lv_get_f(id);
    }else{
        btn[id].lv_curr = !!btn_lv_get_f(id);
    }
    //下面对两个状态进行处理得到上升/下降沿
    
    //如果当前电平等于上次,说明按键电平没有变化
    if(btn[id].lv_curr == btn[id].lv_prev){
        btn[id].edge_mark = lv_mark_no_chrg;
    }//如果上一次是高,现在是低,则说明有下降沿
    else if((btn[id].lv_prev == 1) && (btn[id].lv_curr == 0)){
        btn[id].edge_mark = lv_mark_falling;
    }//如果上一次是低,现在是高,则说明有上升沿
    else if((btn[id].lv_prev == 0) && (btn[id].lv_curr == 1)){
        btn[id].edge_mark = lv_mark_rising;
    }
    //我们现在得到了按键电平的上升/下降沿或者无变化标志位
}

//条件检查
bool cdtn_check(uint8_t id,cdtn_t cdtn){
    switch (cdtn){
    	case cdtn_NON:
            //总是返回true
            return true;
    		break;
        case cdtn_01:
            if(btn[id].edge_mark == lv_mark_falling){
                //记录第一次按下的时间戳
                btn[id].t_press = btn_tick_func_f();
                //记录下一次触发loop的时间点
                //btn[id].t_next_loop = btn[id].t_press + btn[id].lp_loop_pridt;
                btn[id].t_next_loop = btn[id].t_press + btn[id].lp_trig_waitt;
                return true;
            }else{
                return false;
            }
    		break;
    	case cdtn_02:
            if(btn[id].edge_mark == lv_mark_no_chrg){
                //切换到pressed状态执行的动作
                return true;
            }else{
                return false;
            }
    		break;
        case cdtn_03:
            //PRESSED循环执行的动作
            //如果当前时间大于长按触发时间并且没有被触发过才触发长按
            if((btn_tick_func_f() > (btn[id].t_press + btn[id].lp_trig_waitt)) && btn[id].lp_triged_fg == false){
                //置标志位,表明长按已经被触发过了
                btn[id].lp_triged_fg = true;
                return true;
            }else{
                return false;
            }
    		break;
        case cdtn_04:
            //loop
            if(btn_tick_func_f() > btn[id].t_next_loop){
                btn[id].t_next_loop += btn[id].lp_loop_pridt;
                return true;
            }else{
                return false;
            }
    		break;
        case cdtn_05:
            //pressed->release
            if(btn[id].edge_mark == lv_mark_rising){
                btn[id].t_release = btn_tick_func_f();
                return true;
            }else{
                return false;
            }
    		break;
        case cdtn_06:
            //触发松手ev
            if(((btn[id].t_release - btn[id].t_press) < btn[id].lp_trig_waitt) && (btn[id].dp_mode == dp_disable)){  
                return true;
            }else{
                return false;
            }
    		break;
        case cdtn_07:
            //触发长按松手ev
            if((btn[id].t_release - btn[id].t_press) >= btn[id].lp_trig_waitt){  
                cdtn_check(id,cdtn_CLR);
                return true;
            }else{
                return false;
            }
    		break;
        case cdtn_08:
            //进入等待双击
            if(btn[id].dp_mode != dp_disable){
                btn[id].t_next_press = btn[id].t_release + btn[id].dp_prid_waitt;
                return true;
            }else{
                return false;
            }
    		break;
        case cdtn_09:
            //等待第二次按下要计时
            //超时处理
            if(btn_tick_func_f() > btn[id].t_next_press){
                //调用自己CLR
                cdtn_check(id,cdtn_CLR);
                return true;
            }else{
                return false;
            }
    		break;
        case cdtn_10:
            //waitdp->dpress
            if(btn[id].edge_mark == lv_mark_falling){
                //记录第二次按下时间戳
                btn[id].t_dpress = btn_tick_func_f();
                return true;
            }else{
                return false;
            }
    		break;
        case cdtn_11:
            //dpress->dp_release
            if(btn[id].edge_mark == lv_mark_rising){
                return true;
            }else{
                return false;
            }
    		break;
        case cdtn_12:
            if(btn[id].dp_mode == dp_disable){
                return true;
            }else{
                return false;
            }
    		break;
        case cdtn_13:
            if((btn_tick_func_f() > btn[id].t_next_press) && (btn[id].dp_mode == dp_enable)){
                return true;
            }else{
                return false;
            }
    		break;
        case cdtn_14:
            if((((btn[id].t_release - btn[id].t_press) >= btn[id].lp_trig_waitt)) && (btn[id].dp_mode == dp_enable)){
                return true;
            }else{
                return false;
            }
    		break;
        case cdtn_CLR:
            //清空变量
            btn[id].lp_triged_fg = false;
            btn[id].t_press = 0;
            btn[id].t_release = 0;
            return true;
            break;
    	default:
    		break;
    }
    return false;
}

//核心代码,遍历状态表进行状态切换
void FSM_Handler(uint8_t id){
    //遍历状态表
    for(uint8_t fsm_num = 0; fsm_num < (sizeof(fsm) / sizeof(fsm[0])); fsm_num++){
        //满足上态和条件才能切换
        if((btn[id].fsm_state == fsm[fsm_num].state_prev) && cdtn_check(id,fsm[fsm_num].cdtn)){
            //切换状态
            btn[id].fsm_state = fsm[fsm_num].state_tran;
            //如果有动作要触发
            if(fsm[fsm_num].trig_ev != ev_null){
                btn[id].ev_flag = fsm[fsm_num].trig_ev;
                btn_trig_f(id,btn[id].ev_flag);
            }
            btn[id].ev_flag = ev_null;
        }
    }
}

/*@brief  处理函数,用户周期性调用
*
* @param  无
*
* @return 无
*/
void sGBD_Handler(){
    for(uint8_t id = 0; id < BTN_NUM; id++){
        if(btn[id].en){
            Level_Handler(id);
            FSM_Handler(id);
        }
    }
}

