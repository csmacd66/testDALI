#include <stdlib.h>
#include <string.h>
#include "NuMicro.h"
#include "dali_cd_app.h"
#if(APPLICATION_CONTROLLER_PRESENT)
#include "dali_addressing_16.h"

extern volatile uint8_t DALI_gearType_shortAddress[4];//type 5~8
extern volatile uint8_t DALI_shortAddress16_amount;

extern volatile int test_F;
extern volatile uint16_t Time_out_f;
extern volatile uint16_t Time_out_c;
extern volatile int8_t ACK_STATE;
extern volatile uint8_t bus_ACK;

volatile uint8_t CG206_Value;
volatile uint8_t CG207_Value;
volatile uint8_t CG208_Value;
volatile uint8_t CG208_up_on;
volatile uint8_t CG208_up_off;
volatile uint8_t CG208_down_on;
volatile uint8_t CG208_down_off;
volatile uint8_t CG209_type;
volatile uint16_t CG209_tc;
volatile uint16_t CG209_xy[2];
volatile uint8_t CG209_rgb[6];

void read206Value(void)
{
    uint8_t addr = DALI_gearType_shortAddress[0];
    enableDeviceType(5);
    ACK_STATE = queryCMD16(addr, 0xEF);//QUERY OUTPUT LEVEL
    CG206_Value = (bus_ACK * 2650 >> 16) + 1;
}

void read207Value(void)
{
    uint8_t addr = DALI_gearType_shortAddress[1];
    uint16_t outputLevel = 0;
    ACK_STATE = queryCMD16(addr, QUERY_ACTUAL_LEVEL);//QUERY ACTUAL LEVEL
    CG207_Value = bus_ACK;
}

void read208Value(void)
{
    uint8_t addr = DALI_gearType_shortAddress[2];
    uint16_t outputLevel = 0;
    enableDeviceType(7);
    ACK_STATE = queryCMD16(addr, 242);//QUERY_UP_SWITCH_ON
    CG208_up_on = bus_ACK;
    ACK_STATE = queryCMD16(addr, 243);//QUERY_UP_SWITCH_OFF
    CG208_up_off = bus_ACK;
    ACK_STATE = queryCMD16(addr, 244);//QUERY_DOWN_SWITCH_ON
    CG208_down_on = bus_ACK;
    ACK_STATE = queryCMD16(addr, 245);//QUERY_DOWN_SWITCH_ON
    CG208_down_on = bus_ACK;
    ACK_STATE = queryCMD16(addr, QUERY_ACTUAL_LEVEL);//QUERY ACTUAL LEVEL
    CG208_Value = bus_ACK;
}

void read209Value(void)
{
    uint8_t i, dtr0ACK, dtr1ACK, dtr2ACK, addr = DALI_gearType_shortAddress[3];
    enableDeviceType(8);
    ACK_STATE = queryCMD16(addr, 248);//QUERY_COLOUR_STATUS
    CG209_type = bus_ACK & 0xF0;

    switch (CG209_type)
    {
        case 0x10: //xy
            for (i = 0; i < 2; i++)
            {
                dtr0_16(i);
                enableDeviceType(8);
                ACK_STATE = queryCMD16(addr, 0xFA);//query colour value
                ACK_STATE = queryCMD16(addr, 0x9C);//query dtr1
                dtr1ACK = bus_ACK;
                ACK_STATE = queryCMD16(addr, 0x98);//query dtr0
                dtr0ACK = bus_ACK;
                CG209_xy[i] = (dtr1ACK << 8) | dtr0ACK;
            }
            break;

        case 0x20: //tc
            dtr0_16(2);
            enableDeviceType(8);
            ACK_STATE = queryCMD16(addr, 0xFA);//query colour value
            ACK_STATE = queryCMD16(addr, 0x9C);//query dtr1
            dtr1ACK = bus_ACK;
            ACK_STATE = queryCMD16(addr, 0x98);//query dtr0
            dtr0ACK = bus_ACK;
            CG209_tc = (dtr1ACK << 8) | dtr0ACK;
            break;

        case 0x80: //rgbwaf
            for (i = 9; i < 15; i++)
            {
                dtr0_16(i);
                enableDeviceType(8);
                ACK_STATE = queryCMD16(addr, 0xFA);//query colour value
                CG209_rgb[i-9] = bus_ACK;
            }
            break;
    }
}

void readGearData(void)
{
    read206Value();
    read207Value();
    read208Value();
    read209Value();
}
#endif
