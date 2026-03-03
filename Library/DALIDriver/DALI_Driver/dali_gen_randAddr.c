#include <stdlib.h>
#include <stdio.h>
#include "NuMicro.h"
#include "dali_cd_app.h"

#if(RANDADDR_GENERATE)
volatile uint8_t u8dig = 0, seedADF0[8], seedADF1[8], seedBits[8];
extern volatile uint32_t ADCCH, AdcInt0CH, AdcInt1CH;
/*---------------------------------------------------------------------------------------------------------*/
/*              ADF                                                                                                                                              */ 
/*           -------------                                                                                                                                  */
/*          |{x, x, x, x}                                                                                                                                  */
/*     TEMP |{x, x, x, x}                                                                                                                                  */
/*          |{x, x, x, x}                                                                                                                                  */
/*          |{x, x, x, x}                                                                                                                                  */
/*                                                                                                                                                                                                               */
/*---------------------------------------------------------------------------------------------------------*/

uint8_t RandomArray[8][4][4] =
{
    {
        {11, 0, 8,12},
        { 9, 3,10,14},
        { 2,13, 6, 1},
        { 7, 5,15, 4}
    }, //DIG1

    {
        { 6, 2, 5,13},
        {12, 1, 4, 7},
        {15, 8,11, 0},
        {10,14, 3, 9}
    }, //DIG2

    {
        { 4, 9,14,10},
        {11, 2,15, 8},
        { 3, 7,12, 6},
        { 5, 0, 1,13}
    }, //DIG3

    {
        { 8,12, 3,11},
        { 1, 0, 7, 5},
        {13,14,10, 4},
        { 9,15, 2, 6}
    }, //DIG4

    {
        { 7,13, 4, 1},
        {10, 6, 2,15},
        { 5,11, 9,12},
        { 3, 8, 0,14}
    }, //DIG5

    {
        {15,10,11, 5},
        { 4, 9,14, 3},
        { 0,12,13, 8},
        { 2, 1, 6, 7}
    }, //DIG6

    {
        { 1, 8, 7, 6},
        { 3,10,13, 0},
        {14,15, 2,11},
        {12, 5, 9, 4}
    }, //DIG7

    {
        { 5,14,12, 2},
        { 6,11, 4, 9},
        { 7, 0, 8,15},
        { 1,13,10, 3}
    }  //DIG8
};

void seed_gen_ADC(void)
{
    if (u8dig < 8) 
    {
        if (AdcIntFlag.Int0)
            seedADF0[u8dig] = EADC_GET_CONV_DATA(EADC, AdcInt0CH) & 3; //Get last 2 bits of pull up pin sampling result

        if (AdcIntFlag.Int1)
            seedADF1[u8dig] = EADC_GET_CONV_DATA(EADC, AdcInt1CH) & 3; //Get last 2 bits of pull up pin sampling result

        if (AdcIntFlag.Int0 && AdcIntFlag.Int1)
        {  
            EADC_START_CONV(EADC, ADCCH);
            seedBits[u8dig] = RandomArray[u8dig][seedADF0[u8dig]][seedADF1[u8dig]]; //Get random bit dig1~8 value
            AdcIntFlag.Int0 = 0;
            AdcIntFlag.Int1 = 0;
            u8dig++;
        }
    }

    if (u8dig == 8)
    {
        TIMER_Stop(TIMER2);
        u8dig = 0;
        var_seed =    (seedBits[7] << 28)
                    | (seedBits[6] << 24)
                    | (seedBits[5] << 20)
                    | (seedBits[4] << 16)
                    | (seedBits[3] << 12)
                    | (seedBits[2] << 8)
                    | (seedBits[1] << 4)
                    | (seedBits[0]);
        srand(var_seed);
    }
}
#endif

uint32_t ADDRMAX = 0xFFFFFE, RANGE = 0xFFFFFF;

void getRandomAddress(void) //receive cmd RANDOMIZE will call this function
{
    uint32_t data;
#if(RANDADDR_GENERATE)
    u8dig = 0;
    EADC_START_CONV(EADC, ADCCH);
    TIMER_Start(TIMER2);
#endif
    do
    {
        data = rand() / (RAND_MAX/RANGE + 1);
    }while(data >= RANGE);

    var_randomAddress = data;
    write_variable_once((uint8_t *)&var_randomAddress, 4);
}

