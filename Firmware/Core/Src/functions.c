#include "main.h"
#include "functions.h"

void Sleep_us(uint16_t delay)
{
    TIM2->CNT = 0;
    while (TIM2->CNT < delay);
}

void Sleep_ms(uint16_t delay)
{
    while(delay > 0)
    {
			delay--;
			Sleep_us(1000);
    }
}

void Sleep_sec(uint16_t delay)
{
    while(delay > 0)
    {
			Sleep_ms(1000);
			delay--;
    }
}
