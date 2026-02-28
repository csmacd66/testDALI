#include "ssd1306.h"
#include "ssd1306_tests.h"

extern volatile uint32_t uwTick;
extern volatile uint8_t F_OLEDStart;

void HAL_Delay(uint32_t Delay);
uint32_t HAL_GetTick(void);

