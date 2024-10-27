#include "sAPP_Btns.h"




#define KEY_UP_GPIO_CLK_EN __GPIOC_CLK_ENABLE
#define KEY_UP_GPIO        GPIOC
#define KEY_UP_GPIO_PIN    GPIO_PIN_5

#define KEY_DN_GPIO_CLK_EN __GPIOB_CLK_ENABLE
#define KEY_DN_GPIO        GPIOB
#define KEY_DN_GPIO_PIN    GPIO_PIN_0

#define KEY_ET_GPIO_CLK_EN __GPIOB_CLK_ENABLE
#define KEY_ET_GPIO        GPIOB
#define KEY_ET_GPIO_PIN    GPIO_PIN_1

#define KEY_BK_GPIO_CLK_EN __GPIOB_CLK_ENABLE
#define KEY_BK_GPIO        GPIOB
#define KEY_BK_GPIO_PIN    GPIO_PIN_2



static bool get_lv(uint8_t btn_id){
    if(btn_id == SGBD_KEY_UP_ID){
        return !!HAL_GPIO_ReadPin(KEY_UP_GPIO,KEY_UP_GPIO_PIN);
    }
    else if(btn_id == SGBD_KEY_DN_ID){
        return !!HAL_GPIO_ReadPin(KEY_DN_GPIO,KEY_DN_GPIO_PIN);
    }
    else if(btn_id == SGBD_KEY_ET_ID){
        return !!HAL_GPIO_ReadPin(KEY_ET_GPIO,KEY_ET_GPIO_PIN);
    }
    else if(btn_id == SGBD_KEY_BK_ID){
        return !!HAL_GPIO_ReadPin(KEY_BK_GPIO,KEY_BK_GPIO_PIN);
    }
    return false;
}

#include "main.h"

static void trig(uint8_t btn_id,ev_flag_t btn_ev){
    if(btn_id == SGBD_KEY_UP_ID){
        if(btn_ev == ev_dp){
            
        }
        else if(btn_ev == ev_pres){
            g_ctrl.blc_en = !g_ctrl.blc_en;
        }
    }


    //打印按键id的事件
    if(btn_ev == ev_pres){
        //sHMI_BUZZER_StartSinglePulse();
        //dbg.printf("KEY%d:按键按下\n",btn_id + 1);
    }
    else if(btn_ev == ev_rlsd){
        //dbg.printf("KEY%d:按键松手\n",btn_id + 1);
    }
    else if(btn_ev == ev_dp){
        //sHMI_BUZZER_StartSinglePulse();
        //dbg.printf("KEY%d:双击按下\n",btn_id + 1);
    }
    else if(btn_ev == ev_dp_rlsd){
        //dbg.printf("KEY%d:双击松手\n",btn_id + 1);
    }
    else if(btn_ev == ev_lp){
        //sHMI_BUZZER_StartSinglePulse();
        //dbg.printf("KEY%d:长按触发\n",btn_id + 1);
    }
    else if(btn_ev == ev_lp_rlsd){
        //dbg.printf("KEY%d:长按松手\n",btn_id + 1);
    }
    else if(btn_ev == ev_lp_loop){
        //dbg.printf("KEY%d:长按循环触发\n",btn_id + 1);
    }
    //sHMI_Debug_Printf("btn_id:%d,btn_ev:%d\n",btn_id,btn_ev);
}


void sAPP_Btns_Init(){
    KEY_UP_GPIO_CLK_EN();
    KEY_DN_GPIO_CLK_EN();
    KEY_ET_GPIO_CLK_EN();
    KEY_BK_GPIO_CLK_EN();

    GPIO_InitTypeDef gpio = {0};
    gpio.Pin       = KEY_UP_GPIO_PIN;
    gpio.Mode      = GPIO_MODE_INPUT;
    gpio.Pull      = GPIO_PULLUP;
    gpio.Speed     = GPIO_SPEED_FREQ_LOW;
    gpio.Alternate = 0;
    HAL_GPIO_Init(KEY_UP_GPIO, &gpio);
    gpio.Pin       = KEY_DN_GPIO_PIN;
    HAL_GPIO_Init(KEY_DN_GPIO, &gpio);
    gpio.Pin       = KEY_ET_GPIO_PIN;
    HAL_GPIO_Init(KEY_ET_GPIO, &gpio);
    gpio.Pin       = KEY_BK_GPIO_PIN;
    HAL_GPIO_Init(KEY_BK_GPIO, &gpio);


    btn_init_t btn_init;
    memset(&btn_init, 0, sizeof(btn_init));

    btn_init.en = 1;                //使能此按键
    btn_init.lv_rev = lv_non_reverse;   //空闲时的电平反转
    btn_init.dp_mode = dp_disable;   //禁用双击,可提高连续单击速度
    btn_init.lp_loop_pridt = 300;   //设置长按循环触发间隔每500ms触发一次
    btn_init.lp_trig_waitt = 1000;  //设置长按触发时间2000ms
    btn_init.dp_prid_waitt = 200;   //设置最大等待双击时间
    sGBD_SetAllBtnEnable(1);        //设置所有按键使能
    sGBD_SetAllBtnMode(&btn_init);  //装载btn_init的配置参数
    sGBD_Init(get_lv,trig,HAL_GetTick);
}


