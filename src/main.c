#include "main.h"
#include "stm32f4xx_hal.h"
#include "service.h"
#include "cmsis_os.h"
#include "cli.h"
#include "cli.h"
#include "dfu.h"
#include "power.h"
#include "battery.h"
#include "display.h"
#include "lvgl.h"

osThreadId tick_task_handle;
osThreadId cli_task_handle;
osThreadId lvgl_task_handle;
osThreadId power_task_handle;
void tick_task(void const * argument);
void cli_task(void const * argument);
void lvgl_task(void const * argument);
void power_task(void const * argument);

int main(void)
{
    Services_Init();

    osThreadDef(tick_task, tick_task, osPriorityNormal, 0, 128);
    tick_task_handle = osThreadCreate(osThread(tick_task), NULL);

    osThreadDef(cli_task, cli_task, osPriorityNormal, 0, 256);
    cli_task_handle = osThreadCreate(osThread(cli_task), NULL);

    osThreadDef(lvgl_task, lvgl_task, osPriorityNormal, 0, 1024);
    lvgl_task_handle = osThreadCreate(osThread(lvgl_task), NULL);

    osThreadDef(power_task, power_task, osPriorityNormal, 0, 512);
    power_task_handle = osThreadCreate(osThread(power_task), NULL);

    osKernelStart();

    while (1){

        //Services_Loop();

    }

}

void tick_task(void const * argument)
{

    while (1)
    {
        osDelay(1);
    }
}

void cli_task(void const * argument)
{

    while (1)
    {
        cli_loop_service();
        osDelay(100);
    }

}

void lvgl_task(void const * argument)
{

    while (1)
    {
        lv_task_handler();
        osDelay(100);
    }

}

void power_task(void const * argument)
{

    while (1)
    {
        Power_LoopService();
        Battery_LoopService();
        osDelay(100);
    }

}


/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
    while(1){
    }
}
