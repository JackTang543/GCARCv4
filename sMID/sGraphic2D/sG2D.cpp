#include "sG2D.hpp"

#include "FreeRTOS.h"

#include "sDRV_GenOLED.h"
#include "sBSP_DMA.h"


static inline uint32_t portGetTick(){
    return HAL_GetTick();
}

static inline void* portMalloc(size_t size){
    return pvPortMalloc(size);
}

static inline void portFree(void* pv){
    vPortFree(pv);
}

static inline void portM2M_DMA_Init(){
    sBSP_DMA_MemSetByte_Init();
}

static inline void portM2M_DMA_MemSetByte(uint8_t val,uint8_t* pDst,uint16_t len){
    sBSP_DMA_MemSetByte(val,pDst,len);
}

//字体
extern sCGRAM_Char_t SymbolFont[];
extern sCGRAM_Char_t CharFont[];




sG2D::sG2D(){

}

void sG2D::init(uint16_t scr_w,uint16_t scr_h){
    //注册屏幕信息
    this->scr_w = scr_w; this->scr_h = scr_h;
    this->buf_size = (this->scr_h * this->scr_w) / 8;
    //创建缓冲区
    this->disp_buf = (uint8_t*)portMalloc((this->scr_h * this->scr_w) / 8);
    this->draw_buf = (uint8_t*)portMalloc((this->scr_h * this->scr_w) / 8);

    portM2M_DMA_Init();
}


sG2D::~sG2D(){
    portFree(this->disp_buf);
    portFree(this->draw_buf);

}


void sG2D::setDot(uint16_t x, uint16_t y, bool dot_en){
    if((x >= this->scr_w) || (y >= this->scr_h)) return;
    if(dot_en){
        //和排列方式有关,这里是从左到右,一行行连续排列的
        this->draw_buf[x + this->scr_w * (y / 8)] |=   1 << (y % 8);
    }else{
        this->draw_buf[x + this->scr_w * (y / 8)] &= ~(1 << (y % 8));
    }
}

bool sG2D::getDot(uint16_t x, uint16_t y){
    if((x >= this->scr_w) || (y >= this->scr_h)) return 0;
    return (this->draw_buf[x + this->scr_w * (y / 8)]& (1 << (y % 8))) != 0;
}

void sG2D::inv_area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1){
    uint8_t x, y;
    for (y = y0; y <= y1; y++) {
        for (x = x0; x <= x1; x++) {
            uint8_t currentDot = getDot(x, y);
            setDot(x, y, !currentDot);  //翻转像素状态
        }
    }
}

//写入一竖列(8个点)
void sG2D::set_byte(uint16_t x, uint16_t y, uint8_t data){
    for(uint8_t i = 0;i < 8;i++){
        if(data & (1 << i)){
            setDot(x,y + i,1);
        }
    }
}

void sG2D::write_char(uint16_t x,uint16_t y,sCGRAM_Char_t user_char){
    set_byte(x + 1,y,user_char.CharByte0);
    set_byte(x + 2,y,user_char.CharByte1);
    set_byte(x + 3,y,user_char.CharByte2);
    set_byte(x + 4,y,user_char.CharByte3);
    set_byte(x + 5,y,user_char.CharByte4);
}

void sG2D::write_number(uint8_t x,uint8_t y,uint32_t num){
    uint8_t length = (num == 0)? 1 : (uint8_t)(log10(num) + 1);
    for(uint8_t i = length;i > 0;i--){
        write_char(x + (i * 6) - 6,y,SymbolFont[num / (uint16_t)powf(10 , length - i) % 10]);
    }
}

void sG2D::write_string(uint16_t x,uint16_t y,const char* str){
    //遍历字符串
    while(*str != '\0'){
        if(x != 128){
            set_byte(x + 1,y,CharFont[*str].CharByte0);
            set_byte(x + 2,y,CharFont[*str].CharByte1);
            set_byte(x + 3,y,CharFont[*str].CharByte2);
            set_byte(x + 4,y,CharFont[*str].CharByte3);
            set_byte(x + 5,y,CharFont[*str].CharByte4);
            x+=6;
            str++; 
        }else{
            //已经达到所能显示的最大范围了
            return;
        }
    }
}

void sG2D::printf(uint16_t x,uint16_t y,const char* fmt,...){
    char buf[64] = {0};
    va_list args;
    va_start(args, fmt);
    vsprintf(buf,fmt,args);
    va_end(args);
    
    write_string(x,y,buf);
}

//绘制任意线段
void sG2D::drawLine(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,bool dot_en){
    //Bresenham直线绘制算法
    int16_t dx = abs(x1 - x0);
    int16_t dy = -abs(y1 - y0);
    int16_t sx = x0 < x1 ? 1 : -1;
    int16_t sy = y0 < y1 ? 1 : -1;
    int16_t err = dx + dy;

    while (1) {
        setDot(x0, y0, dot_en); //设置像素点
        if(x0 == x1 && y0 == y1) break;
        int16_t e2 = 2 * err;
        if(e2 >= dy){
            err += dy;
            x0 += sx;
        }
        if(e2 <= dx){
            err += dx;
            y0 += sy;
        }
    }
}


void sG2D::drawVLine(uint16_t x, uint16_t y0, uint16_t y1, bool dot_en){
    for(uint16_t i = y0; i < y1; i++){
        setDot(x,i,dot_en);
    }
}

void sG2D::drawHLine(uint16_t x0, uint16_t x1,uint16_t y, bool dot_en){
    for(uint16_t i = x0; i < x1; i++){
        setDot(i,y,dot_en);
    }
}

//绘制矩形
void sG2D::drawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, bool is_fill){
    if (is_fill) {
        for (uint16_t y = y0; y <= y1; y++) {
            drawHLine(x0, x1, y, 1);
        }
    } else {
        for (uint16_t y = y0 - 1; y < y1; y++) {
            drawHLine(x0 - 0, x1 - 0, y, 0);
        }
        drawHLine(x0, x1, y0, 1);
        drawHLine(x0, x1, y1, 1);
        drawVLine(x0, y0, y1, 1);
        drawVLine(x1, y0, y1 + 1, 1);
    }
}


void sG2D::setFPSMode(FPS_ShowMode mode){
    this->fps_mode = mode;
}


void sG2D::attachFPSInfo(){
    static uint32_t fps_ts_last;
    static uint32_t fps_ts_curr;
    static uint32_t fps_prid;
    static uint32_t fps_val;
    static uint8_t fps_x,fps_y;

    if(this->fps_mode == NONE){
        return;
    }
    else if(this->fps_mode == DIGITS2){
        fps_x = this->scr_w - 36u;
        fps_y = this->scr_h - 8u;
    }
    else if(this->fps_mode == DIGITS3){
        fps_x = this->scr_w - 42u;
        fps_y = this->scr_h - 8u;
    }
    
    fps_ts_last = fps_ts_curr;
    fps_ts_curr = portGetTick();
    fps_val = 1000 / (fps_ts_curr - fps_ts_last);

    drawRectangle(fps_x - 1,fps_y - 1 ,128,64,0);
    write_string(fps_x,fps_y,"FPS:");
    write_number(fps_x + 23,fps_y,fps_val);
    inv_area(fps_x - 0,fps_y - 0 ,128,64);
}

void sG2D::setAll(bool px_en){
    if(px_en){
        //memset(&this->draw_buf[0],0xFF,this->buf_size);
        portM2M_DMA_MemSetByte(0xFF,(uint8_t*)&this->draw_buf[0],buf_size);
    }else{
        //memset(&this->draw_buf[0],0x00,this->buf_size);
        portM2M_DMA_MemSetByte(0x00,(uint8_t*)&this->draw_buf[0],buf_size);
    }
}


void sG2D::updateScreen(){
    sDRV_GenOLED_SetShowEN(1);
    
    //附上FPS信息
    attachFPSInfo();
    

    sDRV_GenOLED_UpdateScreen(this->draw_buf);
    swap_buf();
    //HAL_Delay(10);
}


void sG2D::handler(){
    if(sDRV_GenOLED_IsIdle() == false){
        sDBG_Debug_Printf("false\n");
        return;
    }




    updateScreen();
    
}


/**
 * todo 1102 引入双缓冲区模式,检查上次刷新是否完成,如果完成
 * 在updateScreen里发送更新上一帧屏幕后切换缓冲区,否则return
 * 
 * 新更新:
 * 在用户写入数据时,使用链表add画面的改动,比如在某个地方画个什么东西/字符
 * 然后return表示完成.这个过程很快.
 * 然后在DMA发送过程中,pop链表里的改动,应用到屏幕上,然后检查上一帧是否完成,
 * 如果完成则发送新的DMA,否则return.
 * 
 * 注意帧率计算
 * 
 * 动画必须基于事件,也就是不能直接写GRAM
 * 
 * 
 * 动画,简单线性动画
 * 
 * 简单非线性动画
 * 
 */


void sG2D::swap_buf(){
    //交换缓冲区
    uint8_t* tmp = this->disp_buf;
    this->disp_buf = this->draw_buf;
    this->draw_buf = tmp;
}


