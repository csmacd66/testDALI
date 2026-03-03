#ifndef __CRITICAL_ISR_H__
#define __CRITICAL_ISR_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include <stdint.h>

void SysTick_Init(uint32_t);
void SysTick_Init_ISR(uint32_t ticks);
void DALI_TIMER_Init_capture(void);
void DALI_TIMER_Init_count(uint32_t);
void sendFrame_ISR(int8_t, uint8_t);
void DALI_TIMER_SRAM_ISR(void);
void checkResend(void);

#ifdef __cplusplus
}
#endif

#endif //__CRITICAL_ISR_H__

