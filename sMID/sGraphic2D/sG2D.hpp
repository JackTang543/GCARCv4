#pragma once

// #if defined(STM32F405xx) || defined(STM32F411xx) || defined(STM32F446xx)
//     #include "stm32f4xx_hal.h"
// #endif

// #if defined(STM32F103xx)
//     #include "stm32f1xx_hal.h"
// #endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>

#include "sAPP_Font.h"

#include "sDBG_Debug.h"



class sG2D{
public:
    sG2D();
    ~sG2D();

    enum FPS_ShowMode{
        NONE = 0,
        DIGITS2 = 2,        //2位数字
        DIGITS3 = 3,        //3位数字
    };

    void init(uint16_t scr_w = 128u,uint16_t scr_h = 64u);


    void setDot(uint16_t x, uint16_t y, bool dot_en);
    bool getDot(uint16_t x, uint16_t y);
    void inv_area(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void set_byte(uint16_t x, uint16_t y, uint8_t data);
    void write_char(uint16_t x,uint16_t y,sCGRAM_Char_t user_char);

    void write_number(uint8_t x,uint8_t y,uint32_t num);
    void write_string(uint16_t x,uint16_t y,const char* str);
    void printf(uint16_t x,uint16_t y,const char* fmt,...);


    void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, bool dot_en);
    void drawVLine(uint16_t x, uint16_t y0, uint16_t y1, bool dot_en);
    void drawHLine(uint16_t x0, uint16_t x1,uint16_t y, bool dot_en);
    void drawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, bool is_fill);


    void setFPSMode(FPS_ShowMode mode);
    


    void setAll(bool px_en);



    void handler();



private:
    //屏幕长宽
    uint16_t scr_w, scr_h;
    uint32_t buf_size;  //Byte
    //显示缓冲区和绘制缓冲区
    uint8_t* disp_buf = nullptr;
    uint8_t* draw_buf = nullptr;
    FPS_ShowMode fps_mode = NONE;

    void swap_buf();
    void attachFPSInfo();
    void updateScreen();



};






