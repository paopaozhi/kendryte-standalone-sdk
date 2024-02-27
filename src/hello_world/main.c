/* Copyright 2018 Canaan Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <bsp.h>
#include <sysctl.h>
#include "lcd.h"
#include "fpioa.h"

lcd_t *lcdPara = NULL;
lcd_para_t lcd_para = {
        .freq = 150000,
        .height = 240,
        .width = 320,
        .offset_h0 = 0,
        .offset_w0 = 0,
        .offset_h1 = 0,
        .offset_w1 = 0,
        .lcd_type = LCD_TYPE_ST7789,
        .oct = true,
        .invert = 0,
        .extra_para = NULL,
        .rst_pin = 37,
        .cs_pin = 36,
        .dcx_pin = 38,
        .clk_pin = 39,
};

int core1_function(void *ctx)
{
    uint64_t core = current_coreid();
    printf("Core %ld Hello world\n", core);

    fpioa_set_function(lcd_para.rst_pin, FUNC_GPIOHS0 + RST_GPIONUM);
    fpioa_set_function(lcd_para.dcx_pin, FUNC_GPIOHS0 + DCX_GPIONUM);
    fpioa_set_function(lcd_para.cs_pin, FUNC_SPI0_SS0 + LCD_SPI_SLAVE_SELECT);
    fpioa_set_function(lcd_para.clk_pin, FUNC_SPI0_SCLK);

    lcdPara = &lcd_mcu;
//    lcdPara->lcd_para = &lcd_para;
    lcdPara->lcd_para->rst_pin = 37;
    lcdPara->lcd_para->cs_pin = 36;
    lcdPara->lcd_para->dcx_pin = 38;
    lcdPara->lcd_para->clk_pin = 39;

    lcdPara->init(&lcd_para);
    lcdPara->clear(YELLOW);

    while(1);
}

int main(void)
{
    sysctl_pll_set_freq(SYSCTL_PLL0, 800000000);
    uint64_t core = current_coreid();
    int data;
    printf("Core %ld Hello world\n", core);
    register_core1(core1_function, NULL);

    /* Clear stdin buffer before scanf */
    sys_stdin_flush();

    scanf("%d", &data);
    printf("\nData is %d\n", data);
    while(1)
        continue;
    return 0;
}
