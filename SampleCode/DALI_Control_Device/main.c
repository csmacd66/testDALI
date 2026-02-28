/****************************************************************************
 * @file     main.c
 * @version  V1.00
 * @brief    Demonstrate the DALI protocol.
 *
 * SPDX-License-Identifier: Apache-2.0
 * @copyright (C) 2019 Nuvoton Technology Corp. All rights reserved.
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "NuMicro.h"

/* DALI Library */
#include "dali_cd_app.h"
#include "board_init.h"

/* Message */
#if(DEMOBOARD)
#include "HAL_TICK_CONTROL.h"
extern void ssd1306_update(uint8_t);
volatile uint8_t F_updateOLED = 0;
volatile uint8_t SSD1306_page = 0;
#endif

volatile uint8_t frameType = CMD;
volatile uint8_t counter_1ms = 0, counter_5ms = 0;

/* Sensor */
#if(USE_PART301)
#define PRESS_LEVEL 1
volatile uint8_t F_buttonStatus = 0;
#endif

#if(USE_PART302 && AMOUNT_OF_302_switch)
volatile uint8_t F_switchStatus = 0;
#endif
#if(USE_PART302 && AMOUNT_OF_302_rotary)
extern int16_t encoder_count;
#endif

#if(USE_PART303)
volatile uint32_t PIR_Lock_Time_counter = 0, PIRcounter = 0, PIRTIME = 150;
volatile uint8_t F_PIRWarmUpEnd[AMOUNT_OF_303] = {0}, F_PIRWarmUpCheck = AMOUNT_OF_303;
volatile uint8_t F_PIRError = 0, F_PIRtrigger = 0;
#endif

#if(USE_PART304)
#include "ALS.h"
volatile uint16_t Lux_Level_Old = 0;
volatile uint8_t F_ALSWarmUpEnd[AMOUNT_OF_304] = {0}, F_ALSWarmUpCheck = AMOUNT_OF_304;
volatile uint8_t F_ALS_Ready = 0, F_ALSError = 0;
#endif


#if(APPLICATION_CONTROLLER_PRESENT)
#include "dali_addressing_24.h"
#include "dali_addressing_16.h"

volatile uint16_t Time_out_f = 0;
volatile uint16_t Time_out_c = 0;
volatile int8_t ACK_STATE = 0;

extern volatile uint8_t DALI_gearType_shortAddress[4];
extern volatile uint8_t DALI_shortAddress16_amount;
extern volatile uint8_t DALI_deviceType_shortAddress[4];
extern volatile uint8_t DALI_shortAddress24_amount;
#endif

void SYS_Init(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();
    /* Enable HIRC clock (Internal RC 22.1184MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);
    /* Wait for HIRC clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);
    /* Select HCLK clock source as HIRC and and HCLK source divider as 1 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));
    CLK_SetCoreClock(48000000/HCLK_DIV);
    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate SystemCoreClock and CyclesPerUs automatically. */
    SystemCoreClockUpdate();
    setVECTORtoSRAM();
    SYS_LockReg();
}

#if(CHECK_DALI_BUS)
uint16_t g_u16BusTime = 0;

void checkBusOff(void)
{
    if (DRX != SET_DRX_INIT)
        g_u16BusTime++;
    else
        g_u16BusTime = 0;

    if ((g_u16BusTime > 50) && (F_writeFlash)) //50ms
    {
        write_variable();
        F_writeFlash = FALSE;
    }
    else if (g_u16BusTime > 250) //450ms
    {
        SYS_UnlockReg();
        __NVIC_SystemReset();
    }
}

void GPB_IRQHandler(void)
{
    PB->INTSRC = PB->INTSRC;
}
#endif

#if(RANDADDR_GENERATE)
void TMR2_IRQHandler(void)
{
    TIMER_ClearIntFlag(TIMER2);
    seed_gen_ADC();
}

void EADC_INT0_IRQHandler(void)
{
    AdcIntFlag.Int0 = 1;
    /* Clear the A/D ADINT0 interrupt flag */
    EADC_CLR_INT_FLAG(EADC, EADC_STATUS2_ADIF0_Msk);
}

void EADC_INT1_IRQHandler(void)
{
    AdcIntFlag.Int1 = 1;
    /* Clear the A/D ADINT1 interrupt flag */
    EADC_CLR_INT_FLAG(EADC, EADC_STATUS2_ADIF1_Msk);
}
#endif

void SysTick_Handler(void)//Set by checkCollision(), to check if there is a collision that needs Tbreak
{
    SysTick->CTRL = 0;//Reset systick
    SysTick->VAL = 0;

    if ((g_parameter.send_rcv == RECEIVING) && (TIMER0->EXTCTL & TIMER_EXTCTL_CAPIEN_Msk))
    {
        F_receiveFinish = TRUE;
        checkReceiveFinish(); //700 us
        F_receiveFinish = FALSE;
    }

#if CHECK_COLLISION

    else if (F_statusCollision == BREAK_STATE)
    {
        DTX = SET_DTX_INIT;
        checkResend();
        DALI_IO_Init();
        DALI_TIMER_Init_capture();
        g_parameter.send_rcv = IDLE;
        F_statusCollision = INIT_STATE;
    }

#endif
}

void pollingInstance(uint8_t instanceNumber) // instanceNumber update sensor status & event
{
    uint8_t type = instanceType(instanceNumber);

    switch (type)
    {
#if(USE_PART301)

        case 1:
            checkEvent301(instanceNumber);
            break;
#endif
#if(USE_PART302)

        case 2:
            checkEvent302(instanceNumber);
            break;
#endif
#if(USE_PART303)

        case 3:

            /* Please implement your sensor failure detection and set error flag.*/
            /* if there's multiple PIR sensors, you have to set independent error flags to each sensor. */
            /* The checkEvent303 will set corresponding instanceError and instanceErrorByte bit depends on the error flag. */
            /* No event can be sent by the error sensor.*/
            if (PA14) //Physical sensor failure emulate pin.
                F_PIRError = 1;
            else
                F_PIRError = 0;

            checkEvent303(instanceNumber, F_PIRError);
            break;
#endif
#if(USE_PART304)

        case 4:

            /* Please implement your sensor failure detection and set error flag.*/
            /* if there's multiple PIR sensors, you have to set independent error flags to each sensor. */
            /* The checkEvent304 will set corresponding instanceError and instanceErrorByte bit depends on the error flag. */
            /* No event can be sent by the error sensor.*/
            if (F_ALSWarmUpEnd[instanceNumber - startInstanceNo(instanceTypeIndex(instanceNumber))])
            {
                if (PA15) //Physical sensor failure emulate pin
                    F_ALSError = 1;
                else
                    F_ALSError = 0;

                checkEvent304(instanceNumber, F_ALSError);
            }
            break;
#endif
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/*  TIMER1 IRQ Handler                                                                                     */
/*---------------------------------------------------------------------------------------------------------*/
void TMR1_IRQHandler(void) //0.1 ms
{
    uint8_t i, tmp, currentInNo;
    // clear timer interrupt flag
    TIMER_ClearIntFlag(TIMER1);

    switch (counter_1ms)
    {
        case 0:
#if CHECK_DALI_BUS
            checkBusOff();
#endif
#if(APPLICATION_CONTROLLER_PRESENT)
            if (Time_out_f)
                Time_out_c++;
#endif
            break;

        case 3:
#if(USE_PART303)
            if (F_PIRWarmUpCheck)
            {
                for (i = 0; i < g_parameter.numberOfInstances; i++)
                {
                    if (instanceType(i) == 3)
                    {
                        tmp = i - startInstanceNo(instanceTypeIndex(i));
                        F_PIRWarmUpEnd[tmp] = checkWarmUp(i);

                        if (F_PIRWarmUpEnd[tmp])
                            F_PIRWarmUpCheck--;
                    }
                    else if (instanceType(i) > 3)
                        break;
                }
            }

            if (PIR_Lock_Time_counter)
                PIR_Lock_Time_counter--;

            if (PB5 == 1)
            {
                PIRcounter++;
                if (PIRcounter >= PIRTIME)
                    F_PIRtrigger = 1;
                else
                    F_PIRtrigger = 0;
            }
            else
            {
                PIRcounter = 0;
                F_PIRtrigger = 0;
            }
#endif
            break;

        case 4:
#if(USE_PART304)
            for (i = 0; i < g_parameter.numberOfInstances; i++)
            {
                if (instanceType(i) == 4)
                {
                    tmp = i - startInstanceNo(instanceTypeIndex(i));

                    if (F_ALSWarmUpEnd[tmp])
                    {
                        if (ALS_Counter >= 200) //200 ms
                        {
                            ALS_Counter = 0;
                            F_Get_ALS = TRUE;
#if(DEMOBOARD)
                            LED4 = 0;
#endif
                        }
                        else if (F_Get_ALS == 0)
                            ALS_Counter++;
                    }
                }
            }
#endif
            break;

        case 5: // 780 ns
            if (F_newPowerCycleNotice) //1.3 sec, 5 sec
            {
                if (g_parameter.newPowerOnCounter >= NEW_POWER_ON_TIME[var_powerEventTime])
                {
                    g_parameter.newPowerOnCounter++;
                    F_sendPowerCycleNotification = TRUE;
                    var_powerEventTime++;

                    if (var_powerEventTime == 2)
                        F_newPowerCycleNotice = FALSE;
                }
                else
                    g_parameter.newPowerOnCounter++;
            }

            if (F_sendPowerCycleNotification)
            {
                F_sendPowerCycleNotification = FALSE;
                sendPowerCycleNotification();
            }

            break;

        case 6: //420 ns
            if (F_checkSendTwice) //send twice
            {
                if (g_parameter.sendTwiceCounter >= SEND_TWICE_TIME)
                {
                    g_parameter.sendTwiceCounter = 0;
                    F_checkSendTwice = FALSE;
                    g_parameter.twice = 0;
                    g_parameter.twice_cmd[0] = 0;
                    g_parameter.twice_cmd[1] = 0;
                    g_parameter.twice_cmd[2] = 0;
                }
                else
                    g_parameter.sendTwiceCounter++;
            }

            break;

        case 7: //440 ns
            if (F_identification)
            {
                LEDR = 0;
#if(DEMOBOARD)
                LEDP = 0;
#endif
                if (g_parameter.idCounter >= IDENTIFICATION_TIME)
                {
                    LEDR = 1;
#if(DEMOBOARD)
                    LEDP = 1;
#endif
                    F_identification = FALSE;
                    g_parameter.idCounter = 0;
                    g_parameter.idStepCounter = 0;
                }
                else
                {
                    g_parameter.idCounter++;
                    g_parameter.idStepCounter++;
                }
            }
            else
            {
                LEDR = 1;
#if(DEMOBOARD)
                LEDP = 1;
#endif
            }

            break;

        case 8: // 700 ns when 2 flags are 0, 1.24 us when at least one of the flag is 1
            if (var_initialisationState)
            {
                if (g_parameter.initCounter >= INIT_TIME) //15 mins
                {
                    g_parameter.initCounter = 0;
                    var_initialisationState = DISABLE;
                }
                else
                    g_parameter.initCounter++;
            }

            if (sta_quiescentMode)
            {
                if (g_parameter.quiescentmodeCounter >= INIT_TIME) //15 mins
                {
                    g_parameter.quiescentmodeCounter = 0;
                    sta_quiescentMode = DISABLE;
                }
                else
                    g_parameter.quiescentmodeCounter++;
            }

            break;

        case 9:

#if(DEMOBOARD)

            if (F_OLEDStart)
                uwTick++;

#endif
            break;
    }

#if(NUMBER_OF_INSTANCES)

    for (i = 0; i <= 3; i++)
    {
        currentInNo = i * 10 + counter_1ms;

        if ((currentInNo <= g_parameter.numberOfInstances) && (currentInNo < 32))
            pollingInstance(currentInNo);
        else
            break;
    }

#endif

    for (frameType = CMD; frameType < g_parameter.numberOfInstances + 2; frameType++)
    {
        if (F_send(frameType) && (F_sendFrame(frameType) == FALSE) && (g_parameter.send_rcv == IDLE) && ((frameType == CMD) || ((frameType > CMD) && (sta_quiescentMode == DISABLE))))
        {
            if (settlingCounter(frameType) >= settlingTime(frameType))
            {
                F_send(frameType) = FALSE;
                F_sendFrame(frameType) = TRUE;
                sendFrame(frameType);
                F_sendFrame(frameType) = FALSE;
                g_parameter.send_rcv = SENDING;
                settlingCounter(frameType) = 0;
            }
            else
                settlingCounter(frameType)++;
        }
        else
            settlingCounter(frameType) = 0;
    }

    counter_1ms = (counter_1ms < 9) ? (counter_1ms+1) : 0; //1 ms counter

#if(AMOUNT_OF_302_rotary)
    for (i = 0; i < g_parameter.numberOfInstances; i++)
    {
        if (instanceType(i) == 2)
        {
             tmp = i - startInstanceNo(instanceTypeIndex(i));
             SensorValue[tmp] = encoder_count;
        }
    }
#endif

#if(USE_PART302)
    if (counter_5ms == 25)
    {
        counter_5ms++;
#if(AMOUNT_OF_302_switch)
        F_switchStatus = 1;
#endif
#if(AMOUNT_OF_302_slider)
        EADC_START_CONV(EADC, 6);
#endif
    }
    else if (counter_5ms < 49) //5 ms
#else
    if (counter_5ms < 49) //5 ms
#endif
        counter_5ms++;
    else
    {
        counter_5ms = 0;
#if(USE_PART301)
        F_buttonStatus = 1;
#endif
    }
}

#if(USE_PART301)
extern uint32_t Tincr_button;
extern uint32_t Tincr_stuck;

uint8_t buttonValue(uint8_t instanceNumber)
{
    uint8_t state = RELEASE;
#if(DEMOBOARD)
    state = (PC3 == PRESS_LEVEL) ? PRESS : RELEASE;
#else
    state = (GPIO_PIN_DATA(3, instanceNumber) == PRESS_LEVEL) ? PRESS : RELEASE; // PA0~PA7
#endif
    return state;
}
#endif

#if(USE_PART302&&AMOUNT_OF_302_slider)
void EADC_INT2_IRQHandler(void)
{
    AdcIntFlag.Int2 = 1;
    /* Clear the A/D ADINT1 interrupt flag */
    EADC_CLR_INT_FLAG(EADC, EADC_STATUS2_ADIF2_Msk);
}
#endif

int main(void)
{
    uint8_t instanceNumber = 0, type = 0, state_change = 0, count = 0, swBuf[5] = {0}, swState, ch = 6;
    uint32_t sliderRawData = 0, sliderStep = 0, tmp;
    SYS_Init();
#if(USE_PART302 && AMOUNT_OF_302_rotary)
    GPIO_SetMode(PF, BIT2, GPIO_MODE_OUTPUT); //Check whether the stepper motor's step count has reached 2^resolution -1 output high or low
    GPIO_SetMode(PC, BIT7, GPIO_MODE_OUTPUT);
#endif
#if CHECK_DALI_BUS

    if (SYS_IS_SYSTEM_RST()) //Power down > 450ms reset flow
    {
        DALI_IO_Init();

        if (DRX != SET_DRX_INIT)
        {
            GPIO_EnableInt(DRX_PORT, DRX_PIN, GPIO_INT_BOTH_EDGE);
            NVIC_EnableIRQ(GPB_IRQn);
            SYS_UnlockReg();
            CLK_PowerDown();
            SYS_LockReg();
            GPIO_DisableInt(DRX_PORT, DRX_PIN);
            NVIC_DisableIRQ(GPB_IRQn);
        }
    }

#endif

    IO_Init();
    DALI_Init();
    Timer_Init();
    g_parameter.send_rcv = IDLE;

#if(APPLICATION_CONTROLLER_PRESENT)
    DALI_Addressing_24();//comment this line before running test sequences with application function enabled!
#endif
    while (1)
    {
        type = 0;
#if(USE_PART301)

        if (F_buttonStatus)
        {
            for (instanceNumber = startInstanceNo(type); instanceNumber < startInstanceNo(type) + SW_AMOUNT_OF_301; instanceNumber++)
            {
                //SensorValue[instanceNumber] = (buttonValue(instanceNumber) == 0) ? 0xFF : 0;
                state_change = checkButtonStatus(instanceNumber);
#if(DEMOBOARD)
                F_updateOLED = state_change;
                //S_PRESS_DBCK takes 20 ms before enter S_PRESS after button pressed
                if ((state_change) && (S_buttonStatus(instanceNumber) == S_PRESS))
                    LED1 = 0;
                else
                    LED1 = 1;
#endif
            }

            F_buttonStatus = 0;
        }

        type++;
#endif

#if(USE_PART302)
#if(AMOUNT_OF_302_switch)
        if (F_switchStatus)
        {
            for (instanceNumber = startInstanceNo(type); instanceNumber < startInstanceNo(type) + AMOUNT_OF_302_switch; instanceNumber++)
            {
                if (count < 5)
                    swBuf[count++] = PB7;
                else
                {
                    for (uint8_t i = 0; i < 5; i++)
                        swState = (swBuf[i] == 1) ? (swState+1) : swState;

                    if (swState == 5)
                        SensorValue[instanceNumber] = 1;
                    else if (swState == 0)
                        SensorValue[instanceNumber] = 0;

                    swState = 0;
                    count = 0;
                }
            }

            F_switchStatus = 0;
        }
#endif
#if(AMOUNT_OF_302_slider)
        if (AdcIntFlag.Int2)
        {
            
            for (instanceNumber = startInstanceNo(type) + AMOUNT_OF_302_switch; instanceNumber < startInstanceNo(type) + AMOUNT_OF_302_switch + AMOUNT_OF_302_slider; instanceNumber++)
            {
                SensorValue[instanceNumber] = EADC_GET_CONV_DATA(EADC, ch);
                instanceNumber++; ch++;
                AdcIntFlag.Int2 = 0;
            }
        }
#endif

        type++;
#endif

#if(USE_PART303)

        for (instanceNumber = startInstanceNo(type); instanceNumber < startInstanceNo(type) + AMOUNT_OF_303; instanceNumber++)
        {
            //303 movement sensor value update
            if (F_PIRWarmUpEnd[instanceNumber - startInstanceNo(type)])
            {
                if ((F_PIRtrigger == 1) && (PIR_Lock_Time_counter == 0)) //Get Occupied sensor status
                {
                    SensorValue[instanceNumber] = OCCUPIED_MOVEMENT;//0xFF
                    PIR_Lock_Time_counter = REMAIN_PIR_STATE_TIME(calInstanceIndex(instanceNumber));
#if(DEMOBOARD)
                    LED3 = 0;
                    F_updateOLED = 1;
#endif
                }
                else if ((PIR_Lock_Time_counter == 0) && (SensorValue[instanceNumber] == OCCUPIED_MOVEMENT/*0xFF*/))
                {
                    SensorValue[instanceNumber] = OCCUPIED_NOMOVEMENT;//0xAA
#if(DEMOBOARD)
                    LED3 = 0;
                    F_updateOLED = 1;
#endif
                }
                else if ((SensorValue[instanceNumber] == OCCUPIED_NOMOVEMENT/*0xAA*/) && (inputValue(instanceNumber) == 0))
                {
                    SensorValue[instanceNumber] = VACANT_NOMOVEMENT;//0x00
#if(DEMOBOARD)
                    LED3 = 1;
                    F_updateOLED = 1;
#endif
                }
            }
        }

        type++;
#endif
#if(USE_PART304)

        for (instanceNumber = startInstanceNo(type); instanceNumber < startInstanceNo(type) + AMOUNT_OF_304; instanceNumber++)
        {
            //304 sensor value update
            if (F_Get_ALS == 1)
            {
                F_Get_ALS = 0;
                Lux_Level = Get_ALS();//Get Lux Level

                if (SensorValue[instanceNumber] != Lux_Level)
                {
                    SensorValue[instanceNumber] = Lux_Level;
                    F_ALS_Ready = 1;
                }
            }
        }

        type++;
#endif

#if(DEMOBOARD)
        if(PA7 == 0)//KEY1,KEY2 page select
        {
            SSD1306_page = (SSD1306_page == 0) ? (NUMBER_OF_INSTANCES-1) : (SSD1306_page-1);
            F_updateOLED = 1;
        }
        else if(PC2 == 0)
        {
            SSD1306_page = (SSD1306_page == NUMBER_OF_INSTANCES-1) ? 0 : (SSD1306_page+1);
            F_updateOLED = 1;
        }

        if ((F_updateOLED) && (g_parameter.send_rcv == IDLE))
        {
            ssd1306_update(SSD1306_page);
            F_updateOLED = 0;
        }
#endif
    }
}
/*** (C) COPYRIGHT 2019 Nuvoton Technology Corp. ***/

