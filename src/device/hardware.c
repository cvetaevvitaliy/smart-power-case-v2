#include "hardware.h"
#include "stm32_init.h"
#include "cli.h"
#include "dfu.h"
#include "power.h"
#include "battery.h"
#include "display.h"


void Hardware_Init(void)
{
    STM32_Init();
    Power_InitChargerChip();
    Battery_Init();
    Display_Init();

    cli_init();
    dfu_add_cli_cmd();

}

