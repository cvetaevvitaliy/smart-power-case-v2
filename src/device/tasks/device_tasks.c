#include "device_tasks.h"
#include "stm32_init.h"
#include "cli.h"
#include "dfu.h"
#include "power.h"
#include "battery.h"
#include "display.h"


void Services_Init(void)
{
    STM32_Init();
    Power_InitChargerChip();
    Battery_Init();
    Display_Init();

    cli_init();
    dfu_add_cli_cmd();

}


void Services_Loop(void)
{
    cli_loop_service();
    Power_Loop();
    Display_Loop();
    Battery_Loop();
}
