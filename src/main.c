#include "main.h"
#include "stm32_init.h"



int main(void)
{
    Init_Hardware();

    while (1){


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
