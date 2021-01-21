#include "main.h"
#include "service.h"


int main(void)
{
    Services_Init();

    while (1){

        Services_Loop();

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
