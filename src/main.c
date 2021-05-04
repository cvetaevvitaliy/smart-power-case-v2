#include "main.h"
#include "stm32f4xx_hal.h"
#include "stm32_init.h"
#include "hardware.h"
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

static osThreadId cli_task_handle;
static osThreadId lvgl_task_handle;
static osThreadId power_task_handle;
static osThreadId acc_task_handle;
static void CliTask(void const * argument);
static void LvglTask(void const * argument);
static void PowerTask(void const * argument);
static void AccTask(void const * argument);

#ifdef DEBUG
static CLI_Result_t i2c_scan(void);
#endif

#define UNUSED_ARG(X) (void)X      /* To avoid gcc warnings */

int main(void)
{
    Hardware_Init();

    osThreadDef(CliTask, CliTask, osPriorityNormal, 0, 256);
    cli_task_handle = osThreadCreate(osThread(CliTask), NULL);

    osThreadDef(LvglTask, LvglTask, osPriorityNormal, 0, 1024);
    lvgl_task_handle = osThreadCreate(osThread(LvglTask), NULL);

    osThreadDef(PowerTask, PowerTask, osPriorityNormal, 0, 512);
    power_task_handle = osThreadCreate(osThread(PowerTask), NULL);

    osThreadDef(AccTask, AccTask, osPriorityNormal, 0, 1024);
    acc_task_handle = osThreadCreate(osThread(AccTask), NULL);

    osKernelStart();

    while (1)
    {

    }

}

/**
 * @brief This task for check double click event from IMU sensor
 * @param argument
 */
void AccTask(void const * argument)
{
    UNUSED_ARG(argument);

    // @note if IMU not found = suspend execution of IMU task
    if (imu_Init() == -1) {
        osThreadSuspend(acc_task_handle);
    }

    while (1)
    {
        imu_Loop();
        osDelay(100);
    }

}

/**
 * @brief This task for Command line interface
 * @note Used USB virtual uart interface
 * @param argument
 */
void CliTask(void const * argument)
{
    UNUSED_ARG(argument);

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

/**
 * @brief This task for draw GUI interface
 * @note Used LVGL libraries
 * @param argument
 */
void LvglTask(void const * argument)
{
    UNUSED_ARG(argument);

    while (1)
    {
        lv_task_handler();
        gui_update_value();
        osDelay(5);
    }

}

/**
 * @brief This task for BQ25895 charger chip and BQ27441 battery fuel chip
 * @param argument
 */
void PowerTask(void const * argument)
{
    UNUSED_ARG(argument);

    while (1)
    {
        Power_Loop();
        Battery_Loop();
        osDelay(500);
    }

}

#ifdef DEBUG
/** Device scaner For debug i2c device */

/**
 * @brief Linux like i2c bus scanner
 */
CLI_Result_t i2c_scan(void)
{
    I2C_HandleTypeDef *i2c = STM32_GetI2CHandle();
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
        ret = HAL_I2C_IsDeviceReady(i2c, (uint16_t) (address << 1), 2, 5);
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
