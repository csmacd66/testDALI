#ifndef __BOARD_INIT_H__
#define __BOARD_INIT_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include <stdint.h>
#include "dali_cd_app.h"

#define LOW_INIT         0
#define HIGH_INIT        1

#define DTX              PC14
#define DTX_PORT         PC
#define DTX_BIT          BIT14
#define DTX_PIN          14

#define DRX              PB15
#define DRX_PORT         PB
#define DRX_BIT          BIT15
#define DRX_PIN          15
#define DRX_MFP          SYS->GPB_MFPH
#define DRX_GPIO         (SYS->GPB_MFPH & (~SYS_GPB_MFPH_PB15MFP_Msk)) | SYS_GPB_MFPH_PB15MFP_GPIO
#define DRX_ETM0         (SYS->GPB_MFPH & (~SYS_GPB_MFPH_PB15MFP_Msk)) | SYS_GPB_MFPH_PB15MFP_TM0_EXT

#if(DEMOBOARD==1)
#define KEY1             PA7
#define KEY2             PC2
#define LEDP             PA5
#define LED1             PA4
#define LED2             PA3
#define LED3             PA0
#define LED4             PA1
#endif

#define LEDR             PB14

#define SW1              PF3
#define SW2              PF4
#define SW3              PF5

#if(USE_PART302 && AMOUNT_OF_302_rotary)
#define CHECK_STEP_PIN_CW       PF2 
#define CHECK_STEP_PIN_CCW      PC7 

#define Rotary_302_type         0                               /* 1= 30 grid/cycle, 0= 20 grid/cycle */
#define PWM_PHASE_A             PC5 
#define PWM_PHASE_B             PC4 
#define PWM_PHASE_A_1           PC3 
#define PWM_PHASE_B_1           PC2 
#define AUTO_TEST_ROTARY_DETENT 1
#endif  
typedef struct
{
    uint8_t Int0: 1;
    uint8_t Int1: 1;
    uint8_t Int2: 1;
    uint8_t Int3: 1;
    uint8_t Reserved: 4;

} AdcIntFlag_t;
extern volatile AdcIntFlag_t AdcIntFlag;

void IO_Init(void);
void Timer_Init(void);

void DALI_IO_Init(void);
void function_IO_Init(void);

void DALI_TIMER_Init(void);
void TIMER1_Init(void);
#if (RANDADDR_GENERATE)
void TIMER2_Init(void);
void EADC_Init(void);
#endif
#if CHECK_COLLISION
void TIMER3_Init(void);
void TIMER3_Init_capture(void);
#endif
#if (USE_PART301)
void IO_301_Init(void);
#endif
#if (USE_PART302)
void IO_302_Init(void);
#endif
#if(USE_PART302 && AMOUNT_OF_302_rotary)
void PWM1_Capture_Init(void); 
#endif
#if(USE_PART303)
void IO_303_Init(void);
#endif
#if (USE_PART304)
void IO_304_Init(void);
#endif

#if(DEMOBOARD==1)
void OLED_Init(void);
#endif

#ifdef __cplusplus
}
#endif

#endif //__BOARD_INIT_H__

