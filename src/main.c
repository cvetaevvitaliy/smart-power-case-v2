#include "main.h"
#include "stm32f4xx_hal.h"
#include "device_tasks.h"
#include "cmsis_os.h"
#include "cli.h"
#include "cli.h"
#include "dfu.h"
#include "power.h"
#include "battery.h"
#include "display.h"
#include "lvgl.h"
#include "gui_main_screen.h"
#include "imu.h"
#include "eeprom.h"

osThreadId cli_task_handle;
osThreadId lvgl_task_handle;
osThreadId power_task_handle;
osThreadId acc_task_handle;
void cli_task(void const * argument);
void lvgl_task(void const * argument);
void power_task(void const * argument);
void acc_task(void const * argument);

#ifdef DEBUG
static CLI_Result_t i2c_scan(void);
extern I2C_HandleTypeDef hi2c1;
#endif

int main(void)
{
    Services_Init();

    osThreadDef(cli_task, cli_task, osPriorityNormal, 0, 256);
    cli_task_handle = osThreadCreate(osThread(cli_task), NULL);

    osThreadDef(lvgl_task, lvgl_task, osPriorityNormal, 0, 1024);
    lvgl_task_handle = osThreadCreate(osThread(lvgl_task), NULL);

    osThreadDef(power_task, power_task, osPriorityNormal, 0, 512);
    power_task_handle = osThreadCreate(osThread(power_task), NULL);

    osThreadDef(acc_task, acc_task, osPriorityNormal, 0, 1024);
    acc_task_handle = osThreadCreate(osThread(acc_task), NULL);

    osKernelStart();

    while (1)
    {

    }

}

void acc_task(void const * argument)
{

    if (imu_Init() == -1) {
        osThreadSuspend(acc_task_handle);
    }

    while (1)
    {
        imu_Loop();

        osDelay(100);
    }

}

void cli_task(void const * argument)
{
#ifdef DEBUG
    cli_add_new_cmd("i2c_scan", i2c_scan, 0, CLI_PrintNone, "i2c scan bus");
#endif
    eeprom_CliCommand();

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
        gui_update_value();
        osDelay(5);
    }

}

void power_task(void const * argument)
{

    while (1)
    {
        Power_Loop();
        Battery_Loop();
        osDelay(500);
    }

}

#ifdef DEBUG
/** Device scaner For debug i2c device */
CLI_Result_t i2c_scan(void)
{
    HAL_StatusTypeDef ret;
    CLI_PRINTF("\nStart scanning i2c bus:\n");

    // table header
    CLI_PRINTF("  ");
    for (uint8_t i = 0; i < 16; i++) {
        CLI_PRINTF("%3x", i);
    }

    // table body
    for (uint8_t address = 0; address <= 119; address++) {
        if (address % 16 == 0) {
            CLI_PRINTF("\n%02x:", address & 0xF0);
        }
        ret = HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t) (address << 1), 2, 5);
        if (ret == HAL_OK) {
            CLI_PRINTF(" %02x", address); // device found
        } else {
            CLI_PRINTF(" --");
        }
    }
    CLI_PRINTF("\n");

    return CLI_OK;
}
#endif

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
    ULOG_ERROR("%s(): %d\r\n", file, line);
    while(1){
    }
}
