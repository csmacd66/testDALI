#include "NuMicro.h"
#include "board_init.h"
#include "dali_cd_app.h"

#if(USE_PART304)
#include "ALS.h"
extern volatile uint8_t F_ALSWarmUpEnd[AMOUNT_OF_304];
#endif
#if(DEMOBOARD==1)
#include "HAL_TICK_CONTROL.h"
#endif

void DALI_IO_Init(void)
{
    SET_DTX_INIT = LOW_INIT; //customer define. Depends on hardware
    SET_DRX_INIT = LOW_INIT; //customer define. Depends on hardware

    //DALI_TX use PB14(NDA102TD2), or PC14(others). Please check the setting in board_init.h.
    DTX_PORT->SMTEN |= (1 << DTX_PIN);
#if 1
    DTX = SET_DTX_INIT;
    GPIO_SetMode(DTX_PORT, DTX_BIT, GPIO_MODE_OUTPUT);
#else
    GPIO_SetMode(DTX_PORT, DTX_BIT, GPIO_MODE_INPUT);

    if (SET_DTX_INIT == HIGH_INIT)
        GPIO_SetPullCtl(DTX_PORT, DTX_BIT, GPIO_PUSEL_PULL_UP);
    else
        GPIO_SetPullCtl(DTX_PORT, DTX_BIT, GPIO_PUSEL_PULL_DOWN);

#endif

    //DALI_RX use PB15 TM0_EXT
    DRX_MFP = DRX_ETM0;
    DRX_PORT->SMTEN |= (1 << DRX_PIN);

    if (SET_DRX_INIT == HIGH_INIT)
        GPIO_SetPullCtl(DRX_PORT, DRX_BIT, GPIO_PUSEL_PULL_UP);
    else
        GPIO_SetPullCtl(DRX_PORT, DRX_BIT, GPIO_PUSEL_PULL_DOWN);
}

void function_IO_Init(void)
{
#if((USE_PART301 == 1) && (DEMOBOARD == 0)) //hardware switches define button amount //Optional //Customer define
//    GPIO_SET_DEBOUNCE_TIME(GPIO_DBCTL_DBCLKSRC_LIRC, GPIO_DBCTL_DBCLKSEL_1024);//26.9ms
//    // button number switch
//    GPIO_SetMode(PF, BIT3, GPIO_MODE_INPUT); // button number SW1
//    GPIO_SetMode(PF, BIT4, GPIO_MODE_INPUT); // button number SW2
//    GPIO_SetMode(PF, BIT5, GPIO_MODE_INPUT); // button number SW3
//    GPIO_SetPullCtl(PF, BIT3, GPIO_PUSEL_PULL_UP);
//    GPIO_SetPullCtl(PF, BIT4, GPIO_PUSEL_PULL_UP);
//    GPIO_SetPullCtl(PF, BIT5, GPIO_PUSEL_PULL_UP);
//    GPIO_ENABLE_DEBOUNCE(PF, BIT3);
//    GPIO_ENABLE_DEBOUNCE(PF, BIT4);
//    GPIO_ENABLE_DEBOUNCE(PF, BIT5);
#endif
#if(DEMOBOARD==1)
    GPIO_SetMode(PA, BIT7, GPIO_MODE_INPUT); //KEY1
    GPIO_SetMode(PC, BIT2, GPIO_MODE_INPUT); //KEY2
    GPIO_ENABLE_DEBOUNCE(PA, BIT7);
    GPIO_ENABLE_DEBOUNCE(PC, BIT2);

    LEDP = 1;  //LED on Sensor board
    LED1 = 1;  //LED on Sensor board
    LED2 = 1;  //LED on Sensor board
    LED3 = 1;  //LED on Sensor board
    LED4 = 1;  //LED on Sensor board
    GPIO_SetMode(PA, BIT5, GPIO_MODE_OUTPUT); //led Power
    GPIO_SetMode(PA, BIT4, GPIO_MODE_OUTPUT); //led
    GPIO_SetMode(PA, BIT3, GPIO_MODE_OUTPUT); //led
    GPIO_SetMode(PA, BIT0, GPIO_MODE_OUTPUT); //led
    GPIO_SetMode(PA, BIT1, GPIO_MODE_OUTPUT); //led
#endif

    LEDR = 1;  //LED on NuMaker board
    GPIO_SetMode(PB, BIT14, GPIO_MODE_OUTPUT); //LED on NuMaker board
}

void DALI_TIMER_Init(void)
{
    CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0SEL_PCLK0, NULL);
    CLK_EnableModuleClock(TMR0_MODULE);
    DALI_TIMER_Init_capture();
    NVIC_EnableIRQ(TMR0_IRQn);
}

void TIMER1_Init(void)
{
    CLK_SetModuleClock(TMR1_MODULE, CLK_CLKSEL1_TMR1SEL_PCLK0, NULL);
    CLK_EnableModuleClock(TMR1_MODULE);
    TIMER_ClearIntFlag(TIMER1);
    TIMER_Open(TIMER1, TIMER_PERIODIC_MODE, 10000);//0.1 ms
    TIMER_EnableInt(TIMER1);
    NVIC_EnableIRQ(TMR1_IRQn);
    TIMER_Start(TIMER1);
}

#if CHECK_COLLISION
void TIMER3_Init(void)
{
    SYS->GPA_MFPH = (SYS->GPA_MFPH & (~SYS_GPA_MFPH_PA8MFP_Msk)) | SYS_GPA_MFPH_PA8MFP_TM3_EXT;
    PA->SMTEN |= (1 << 8);

    if (SET_DRX_INIT == HIGH_INIT)
        GPIO_SetPullCtl(PA, BIT8, GPIO_PUSEL_PULL_UP);
    else
        GPIO_SetPullCtl(PA, BIT8, GPIO_PUSEL_PULL_DOWN);

    CLK_SetModuleClock(TMR3_MODULE, CLK_CLKSEL1_TMR3SEL_PCLK1, NULL);
    CLK_EnableModuleClock(TMR3_MODULE);
}

void TIMER3_Init_capture(void)
{
    TIMER3->CNT = 0;
    // Set compare value as large as possible
    TIMER3->CMP = 0xFFFFFF;
    TIMER3->CTL = TIMER_PERIODIC_MODE;

    // Configure Timer 0 trigger counting mode, capture TDR value on rising edge, enable capture interrupt
    if (SET_DRX_INIT == LOW_INIT)
        TIMER3->EXTCTL = TIMER_CAPTURE_FREE_COUNTING_MODE | TIMER_CAPTURE_EVENT_RISING_FALLING | TIMER_EXTCTL_CAPDBEN_Msk | TIMER_EXTCTL_CAPIEN_Msk | TIMER_EXTCTL_CAPEN_Msk;
    else
        TIMER3->EXTCTL = TIMER_CAPTURE_FREE_COUNTING_MODE | TIMER_CAPTURE_EVENT_FALLING_RISING | TIMER_EXTCTL_CAPDBEN_Msk | TIMER_EXTCTL_CAPIEN_Msk | TIMER_EXTCTL_CAPEN_Msk;

    TIMER3->CTL |= TIMER_CTL_CNTEN_Msk;
    NVIC_EnableIRQ(TMR3_IRQn);
}
#endif
#if(USE_PART302 && AMOUNT_OF_302_rotary)
volatile int16_t encoder_count = 0;        /* Store encoder count */
volatile uint8_t last_state = 0;           /* Store last encoder state */
volatile int8_t transition_sum = 0;        /* Sum of direction transitions */
volatile int8_t Rotary_302_repeat_check = 0;

const int8_t transition_table[16] =
{
     0,  /*  0: 00 -> 00 (no change) */
    -1,  /*  1: 00 -> 01 (CCW)       */
     1,  /*  2: 00 -> 10 (CW)        */
     0,  /*  3: 00 -> 11 (invalid)   */

     1,  /*  4: 01 -> 00 (CW)        */
     0,  /*  5: 01 -> 01 (no change) */
     0,  /*  6: 01 -> 10 (invalid)   */
    -1,  /*  7: 01 -> 11 (CCW)       */

    -1,  /*  8: 10 -> 00 (CCW)       */
     0,  /*  9: 10 -> 01 (invalid)   */
     0,  /* 10: 10 -> 10 (no change) */
     1,  /* 11: 10 -> 11 (CW)        */

     0,  /* 12: 11 -> 00 (invalid)   */
     1,  /* 13: 11 -> 01 (CW)        */
    -1,  /* 14: 11 -> 10 (CCW)       */
     0   /* 15: 11 -> 11 (no change) */
};

void HandleEncoderEvent(uint8_t id, uint8_t phase_A, uint8_t phase_B)
{
    uint8_t current_state = (phase_A << 1) | phase_B;
    uint8_t transition = (last_state << 2) | current_state;

    /* Lookup direction using transition table */
    int8_t dir = transition_table[transition];

#if(Rotary_302_type && AMOUNT_OF_302_rotary)            //motor->Rotary_302_repeat_check=2
    Rotary_302_repeat_check = 4;
#else
    Rotary_302_repeat_check = 2;
#endif
    /* If movement is valid */
    if (dir != 0)
    {
        transition_sum += dir;
        if (transition_sum >= Rotary_302_repeat_check)
        {
            ++encoder_count;
            transition_sum = 0;

            if(encoder_count == AUTO_TEST_ROTARY_DETENT)
            {
                CHECK_STEP_PIN_CCW = 1;
                CHECK_STEP_PIN_CW = 0;
            }
        }
        else if (transition_sum <= -Rotary_302_repeat_check)
        {
            encoder_count--;
            transition_sum = 0;
            if((encoder_count == 0) && (CHECK_STEP_PIN_CCW == 1))
            {
                CHECK_STEP_PIN_CW = 1;
                CHECK_STEP_PIN_CCW = 0;
            }
        }
    }

    /* Update last state */
    last_state = current_state;
}

void PWM1_P0_IRQHandler(void)
{
    uint32_t Rotary_1_Phase_A_State = PWM_GetCaptureIntFlag(PWM1, 0);
    uint32_t Rotary_1_Phase_B_State = PWM_GetCaptureIntFlag(PWM1, 1);

    if (Rotary_1_Phase_A_State & 0x01)   //Rotary 1 PHASE A Rising edge
    {
        HandleEncoderEvent(0, PWM_PHASE_A & 1, PWM_PHASE_B & 1);
        PWM_ClearCaptureIntFlag(PWM1, 0, PWM_CAPTURE_INT_RISING_LATCH);
    }

    if (Rotary_1_Phase_A_State & 0x02)   //Rotary 1 PHASE A Falling edge
    {
        HandleEncoderEvent(0, PWM_PHASE_A & 1, PWM_PHASE_B & 1);
        PWM_ClearCaptureIntFlag(PWM1, 0, PWM_CAPTURE_INT_FALLING_LATCH);
    }

    if (Rotary_1_Phase_B_State & 0x01)   //Rotary 1 PHASE B Rising edge
    {
        HandleEncoderEvent(0, PWM_PHASE_A & 1, PWM_PHASE_B & 1);
        PWM_ClearCaptureIntFlag(PWM1, 1, PWM_CAPTURE_INT_RISING_LATCH);
    }

    if (Rotary_1_Phase_B_State & 0x02)   //Rotary 1 PHASE B Falling edge
    {
        HandleEncoderEvent(0, PWM_PHASE_A & 1, PWM_PHASE_B & 1);
        PWM_ClearCaptureIntFlag(PWM1, 1, PWM_CAPTURE_INT_FALLING_LATCH);
    }
}

void PWM1_Capture_Init(void)
{
    CLK_EnableModuleClock(PWM1_MODULE);
    CLK_SetModuleClock(PWM1_MODULE, CLK_CLKSEL2_PWM1SEL_PCLK1, 0);

    SYS->GPC_MFPL &= ~(SYS_GPC_MFPL_PC4MFP_Msk | SYS_GPC_MFPL_PC5MFP_Msk);
    SYS->GPC_MFPL |= (SYS_GPC_MFPL_PC4MFP_PWM1_CH1 | SYS_GPC_MFPL_PC5MFP_PWM1_CH0);

    PWM_EnableCapture(PWM1, PWM_CH_0_MASK);
    PWM_EnableCaptureInt(PWM1, 0, PWM_CAPTURE_INT_RISING_LATCH | PWM_CAPTURE_INT_FALLING_LATCH);

    PWM_EnableCapture(PWM1, PWM_CH_1_MASK);
    PWM_EnableCaptureInt(PWM1, 1, PWM_CAPTURE_INT_RISING_LATCH | PWM_CAPTURE_INT_FALLING_LATCH);

    /* Enable PWM1 interrupt in NVIC */
    NVIC_EnableIRQ(PWM1_P0_IRQn);

    /* Set the interrupt priority for PWM1_IRQn to level 1
    (lower number means higher priority) */
    //NVIC_SetPriority(PWM1_IRQn, 1);

    /* Initialize last_state to the current state of the encoder */
    last_state = ((PWM_PHASE_A & 1) << 1) | (PWM_PHASE_B & 1);
}
#endif

#if (RANDADDR_GENERATE)
void TIMER2_Init(void)
{
    CLK_SetModuleClock(TMR2_MODULE, CLK_CLKSEL1_TMR2SEL_PCLK1, NULL);
    CLK_EnableModuleClock(TMR2_MODULE);
    TIMER_ClearIntFlag(TIMER2);
    TIMER_Open(TIMER2, TIMER_PERIODIC_MODE, 125); //8 ms
    TIMER_EnableInt(TIMER2);
    NVIC_EnableIRQ(TMR2_IRQn);
    NVIC_SetPriority(TMR2_IRQn, 3);
    TIMER_Start(TIMER2);
}

volatile AdcIntFlag_t AdcIntFlag;
volatile uint32_t ADCCH = 0, AdcInt0CH = 0, AdcInt1CH = 0, AdcInt2CH = 0;
void EADC_Init(void)
{
    CLK_EnableModuleClock(EADC_MODULE);
    CLK_SetModuleClock(EADC_MODULE, 0, CLK_CLKDIV0_EADC(8));
    EADC_Open(EADC, 0);

    //Customer define.
    //Choose 2 adc channel: (any floating adc pin x2) or (any floating adc pin x1 + adc tempurature channel x1)

#if 1
    //unused adc pin 1
    GPIO_SetMode(PB, BIT10, GPIO_MODE_INPUT);
    GPIO_SetPullCtl(PB, BIT10, GPIO_PUSEL_PULL_UP);
    SYS->GPB_MFPH = (SYS->GPB_MFPH & ~SYS_GPB_MFPH_PB10MFP_Msk) | SYS_GPB_MFPH_PB10MFP_EADC0_CH10;
    GPIO_DISABLE_DIGITAL_PATH(PB, BIT10);
    EADC_SetExtendSampleTime(EADC, 10, 0);
    EADC_ENABLE_SAMPLE_MODULE_INT(EADC, 0, BIT10);
    EADC_ConfigSampleModule(EADC, 10, EADC_SOFTWARE_TRIGGER, 10);
    ADCCH |= BIT10;
    AdcInt0CH = 10;
#endif
#if 0
    //unused adc pin 2
    GPIO_SetMode(PB, BIT11, GPIO_MODE_INPUT);
    GPIO_SetPullCtl(PB, BIT11, GPIO_PUSEL_PULL_UP);
    SYS->GPB_MFPH = (SYS->GPB_MFPH & ~SYS_GPB_MFPH_PB11MFP_Msk) | SYS_GPB_MFPH_PB11MFP_EADC0_CH11;
    GPIO_DISABLE_DIGITAL_PATH(PB, BIT11);
    EADC_SetExtendSampleTime(EADC, 11, 0);
    EADC_ENABLE_SAMPLE_MODULE_INT(EADC, 1, BIT11);
    EADC_ConfigSampleModule(EADC, 11, EADC_SOFTWARE_TRIGGER, 11);
    ADCCH |= BIT11;
    AdcInt1CH = 11;
#endif
#if 1
    //Enable Bandgap
//    SYS->IVSCTL |= SYS_IVSCTL_VTEMPEN_Msk;
    EADC_SetExtendSampleTime(EADC, 16, 0);
    EADC_ENABLE_SAMPLE_MODULE_INT(EADC, 1, BIT16);
    EADC_ConfigSampleModule(EADC, 16, EADC_SOFTWARE_TRIGGER, 16);
    ADCCH |= BIT16;
    AdcInt1CH = 16;
#endif
#if 0
    //Enable temperature sensor
//    SYS->IVSCTL |= SYS_IVSCTL_VTEMPEN_Msk;
    EADC_SetExtendSampleTime(EADC, 17, 0);
    EADC_ENABLE_SAMPLE_MODULE_INT(EADC, 1, BIT17);
    EADC_ConfigSampleModule(EADC, 17, EADC_SOFTWARE_TRIGGER, 17);
    ADCCH |= BIT17;
    AdcInt1CH = 17;
#endif

    AdcIntFlag.Int0 = 0;
    EADC_CLR_INT_FLAG(EADC, EADC_STATUS2_ADIF0_Msk);
    EADC_ENABLE_INT(EADC, BIT0);
    NVIC_EnableIRQ(EADC_INT0_IRQn);
    NVIC_SetPriority(EADC_INT0_IRQn, 2);

    AdcIntFlag.Int1 = 0;
    EADC_CLR_INT_FLAG(EADC, EADC_STATUS2_ADIF1_Msk);
    EADC_ENABLE_INT(EADC, BIT1);
    NVIC_EnableIRQ(EADC_INT1_IRQn);
    NVIC_SetPriority(EADC_INT1_IRQn, 2);

    EADC_START_CONV(EADC, ADCCH);
}
#endif

#if (USE_PART301)
void IO_301_Init(void)
{
    uint8_t i;
#if(DEMOBOARD)
    SW_AMOUNT_OF_301 = AMOUNT_OF_301;
    /* Enable interrupt de-bounce function and select de-bounce sampling cycle time is 1024 clocks of LIRC clock */
    GPIO_SET_DEBOUNCE_TIME(GPIO_DBCTL_DBCLKSRC_LIRC, GPIO_DBCTL_DBCLKSEL_1024);//26.9ms
    GPIO_SetMode(PC, BIT3, GPIO_MODE_INPUT);
    GPIO_ENABLE_DEBOUNCE(PC, BIT3);
#else
//    uint8_t numberOfKeys = (SW3 << 2) | (SW2 << 1) | (SW1 << 0);
//    SW_AMOUNT_OF_301 = numberOfKeys;
    SW_AMOUNT_OF_301 = AMOUNT_OF_301; //Customer defined
    /* Enable interrupt de-bounce function and select de-bounce sampling cycle time is 1024 clocks of LIRC clock */
    GPIO_SET_DEBOUNCE_TIME(GPIO_DBCTL_DBCLKSRC_LIRC, GPIO_DBCTL_DBCLKSEL_1024);//26.9ms

    for (i = 0; i < SW_AMOUNT_OF_301; i++)
    {
        GPIO_SetMode(PD, 1 << i, GPIO_MODE_INPUT);
        GPIO_ENABLE_DEBOUNCE(PD, 1 << i);
    }
#endif
}
#endif

#if (USE_PART302)
void IO_302_Init(void)
{
#if(AMOUNT_OF_302_switch)
    GPIO_SetMode(PB, BIT7, GPIO_MODE_INPUT); //302_1
    GPIO_EnableInt(PB, 7, GPIO_INT_BOTH_EDGE);
    NVIC_EnableIRQ(GPB_IRQn);
    GPIO_ENABLE_DEBOUNCE(PB, BIT7);
#endif
#if(AMOUNT_OF_302_slider)
    GPIO_SetMode(PB, BIT6, GPIO_MODE_INPUT);//302_2
    SYS->GPB_MFPL = (SYS->GPB_MFPL & ~SYS_GPB_MFPL_PB6MFP_Msk) | SYS_GPB_MFPL_PB6MFP_EADC0_CH6;
    GPIO_DISABLE_DIGITAL_PATH(PB, BIT6);
    CLK_EnableModuleClock(EADC_MODULE);
    CLK_SetModuleClock(EADC_MODULE, 0, CLK_CLKDIV0_EADC(8));
    EADC_Open(EADC, 0);
    EADC_SetExtendSampleTime(EADC, 6, 0);
    EADC_ENABLE_SAMPLE_MODULE_INT(EADC, 2, BIT6);
    EADC_ConfigSampleModule(EADC, 6, EADC_SOFTWARE_TRIGGER, 6);
    AdcIntFlag.Int2 = 0;
    EADC_CLR_INT_FLAG(EADC, EADC_STATUS2_ADIF2_Msk);
    EADC_ENABLE_INT(EADC, BIT2);
    NVIC_EnableIRQ(EADC_INT2_IRQn);
    NVIC_SetPriority(EADC_INT2_IRQn, 2);
#endif
#if(AMOUNT_OF_302_rotary)

	GPIO_SetMode(PF, BIT2, GPIO_MODE_OUTPUT); //Check whether the stepper motor's step count has reached 2^resolution -1 output high or low
    GPIO_SetMode(PC, BIT7, GPIO_MODE_OUTPUT);

    CLK_EnableModuleClock(PWM1_MODULE);
    CLK_SetModuleClock(PWM1_MODULE, CLK_CLKSEL2_PWM1SEL_PCLK1, 0);
    SYS_ResetModule(PWM1_RST);
#endif
}
#endif

#if(USE_PART303)
void IO_303_Init(void)
{
    //PIR input
    PB5 = 0;
    GPIO_SetPullCtl(PB, BIT5, GPIO_PUSEL_PULL_DOWN);
    GPIO_SetMode(PB, BIT5, GPIO_MODE_INPUT);

    //Physical sensor failure emulate pin
    PA14 = 0;
    GPIO_SetPullCtl(PA, BIT14, GPIO_PUSEL_PULL_DOWN);
    GPIO_SetMode(PA, BIT14, GPIO_MODE_INPUT);
}
#endif

#if(USE_PART304)
void IO_304_Init(void)
{
    //light sensor I2C setting
    SYS->GPB_MFPL = (SYS->GPB_MFPL & ~(SYS_GPB_MFPL_PB0MFP_Msk | SYS_GPB_MFPL_PB1MFP_Msk)) |
                    (SYS_GPB_MFPL_PB0MFP_I2C1_SDA | SYS_GPB_MFPL_PB1MFP_I2C1_SCL);
    GPIO_SetMode(PA, BIT2, GPIO_MODE_INPUT);
    CLK_EnableModuleClock(I2C1_MODULE);
    I2C_Open(I2C1, 100000);
    /* Set I2C 4 Slave Addresses */
    I2C_SetSlaveAddr(I2C1, 0, 0x39, 0);   /* Slave Address : 0x15 */
    //ALS initial
    I2C_WriteByteOneReg(I2C1, 0x39, 0x80, 0x03);
    I2C_WriteByteOneReg(I2C1, 0x39, 0x81, 0xf6);

    F_ALSWarmUpEnd[0] = 1;
    //Physical sensor failure emulate pin
    PA15 = 0;
    GPIO_SetPullCtl(PA, BIT15, GPIO_PUSEL_PULL_DOWN);
    GPIO_SetMode(PA, BIT15, GPIO_MODE_INPUT);
}
#endif

#if(DEMOBOARD==1)
void OLED_Init(void)
{
    /* Set I2C0 multi-function pins */
    SYS->GPC_MFPL = (SYS->GPC_MFPL & ~(SYS_GPC_MFPL_PC1MFP_Msk | SYS_GPC_MFPL_PC1MFP_Msk)) |
                    (SYS_GPC_MFPL_PC0MFP_I2C0_SDA | SYS_GPC_MFPL_PC1MFP_I2C0_SCL);

    if ((PC0 == 0) || (PC1 == 0))
        F_OLEDStart = FALSE;
    else
        F_OLEDStart = TRUE;

    if (F_OLEDStart)
    {
        CLK_EnableModuleClock(I2C0_MODULE);
        I2C_Open(I2C0, 100000);
        ssd1306_Init();
    }
}
#endif

void IO_Init(void)
{
    DALI_IO_Init();
    function_IO_Init();
#if RANDADDR_GENERATE
    EADC_Init();
#endif

#if(USE_PART301)
    IO_301_Init();
#endif
#if(USE_PART302)
    IO_302_Init();
#endif
#if(USE_PART303)
    IO_303_Init();
#endif
#if(USE_PART304)
    IO_304_Init();
#endif
#if(DEMOBOARD==1)
    OLED_Init();//I2C1
#endif
}

void Timer_Init(void)
{
    SysTick->CTRL = 0;
    DALI_TIMER_Init();
    TIMER1_Init();
#if RANDADDR_GENERATE
    TIMER2_Init();
#endif
#if CHECK_COLLISION
    TIMER3_Init();
#endif
    SYS_UnlockReg();
    NVIC_SetPriority(SysTick_IRQn, 0);
    NVIC_SetPriority(TMR0_IRQn, 0);           //TIMER0's priority need to be '0', the highest one.
    NVIC_SetPriority(TMR1_IRQn, 2);           //TIMER1's priority is '1'.
#if CHECK_COLLISION
    NVIC_SetPriority(TMR3_IRQn, 1);
#endif
    SYS_LockReg();
}

