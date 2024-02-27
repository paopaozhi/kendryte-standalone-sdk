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
#include <src/drivers/display/st7789/lv_st7789.h>
#include <sysctl.h>
#include "lvgl.h"
#include "st7789.h"
#include "lv_port_disp.h"

lv_display_t *st7789_display;

void lv_example_get_started_1(void)
{
    /*Change the active screen's background color*/
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x003a57), LV_PART_MAIN);

    /*Create a white label, set its text and align it to the center*/
    lv_obj_t * label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "Hello world");
    lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
}


/* Send short command to the LCD. This function shall wait until the transaction finishes. */
void my_lcd_send_cmd(lv_display_t *disp, const uint8_t *cmd, size_t cmd_size, const uint8_t *param, size_t param_size)
{
    for(int i = 0; i < cmd_size; i++)
    {
        tft_write_command(*(cmd + i));
    }
}

/* Send large array of pixel data to the LCD. If necessary, this function has to do the byte-swapping. This function can do the transfer in the background. */
void my_lcd_send_color(lv_display_t *disp, const uint8_t *cmd, size_t cmd_size, uint8_t *param, size_t param_size)
{
    tft_write_byte(cmd, cmd_size);
}

int core1_function(void *ctx)
{
    uint64_t core = current_coreid();
    printf("Core %ld Hello world\n", core);

    tft_hard_init(15000000, true);


    st7789_display = lv_st7789_create(320, 280, 0, my_lcd_send_cmd, my_lcd_send_color);

    lv_init();
    lv_port_disp_init();

    lv_example_get_started_1();
    while(1){
        lv_tick_inc(5);
        lv_task_handler();
        msleep(5);
    }

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
