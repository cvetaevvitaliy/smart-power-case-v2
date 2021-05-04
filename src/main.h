#ifndef SMART_POWER_CASE_V2_MAIN_H
#define SMART_POWER_CASE_V2_MAIN_H
#include "hw_config.h"


void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)

#endif //SMART_POWER_CASE_V2_MAIN_H
