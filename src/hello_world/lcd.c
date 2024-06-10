#include <bsp.h>
#include "fpioa.h"
#include "gpiohs.h"
#include "lcd.h"

static uint16_t width_curr = 0;
static uint16_t height_curr = 0;

static lcd_para_t lcd_para = {
    .freq = 15000000,
    .height = 320,
    .width = 240,
    .offset_h0 = 0,
    .offset_w0 = 0,
    .offset_h1 = 0,
    .offset_w1 = 0,
    .lcd_type = LCD_TYPE_ST7789,
    .oct = true,
    .dir = DIR_YX_RLDU,
    .invert = 0,
    .extra_para = NULL,
    // port pin
    .rst_pin = 37,
    .dcx_pin = 38,
    .cs_pin = 36,
    .clk_pin = 39,
};
lcd_t *lcd = NULL;

void my_lcd_init(void)
{
    fpioa_set_function(lcd_para.rst_pin, FUNC_GPIOHS0 + RST_GPIONUM);
    fpioa_set_function(lcd_para.dcx_pin, FUNC_GPIOHS0 + DCX_GPIONUM);
    fpioa_set_function(lcd_para.cs_pin, FUNC_SPI0_SS0 + LCD_SPI_SLAVE_SELECT);
    fpioa_set_function(lcd_para.clk_pin, FUNC_SPI0_SCLK);

    lcd = &lcd_mcu;

    int ret = 0;
    lcd->lcd_para = &lcd_para;
    ret = lcd->init(&lcd_para);
    // 为测试使用黄色
    lcd->clear(YELLOW);

    if(ret != 0)
    {
        lcd_para.width = 0;
        lcd_para.width = 0;
        width_curr = 0;
        height_curr = 0;
        // mp_raise_OSError(ret);
    } else
    {
        lcd_para.width = lcd->get_width();
        lcd_para.height = lcd->get_height();
        height_curr = lcd_para.height;
        width_curr = lcd_para.width;
    }

    // lcd background
    // lcd_clear();
}