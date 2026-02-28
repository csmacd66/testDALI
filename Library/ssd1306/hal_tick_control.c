#include <stdio.h>
#include "NuMicro.h"
#include "hal_tick_control.h"

volatile uint32_t uwTick = 0;
volatile uint8_t F_OLEDStart = FALSE;

uint32_t HAL_GetTick(void)
{
    return uwTick;
}

void HAL_Delay(uint32_t Delay)
{
    uint32_t tickstart = 0U;
    tickstart = HAL_GetTick();

    while ((HAL_GetTick() - tickstart) < Delay)
    {
    }
}

