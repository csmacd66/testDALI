#include <stdlib.h>
#include <string.h>
#include "NuMicro.h"
#include "dali_cd_app.h"
#if(APPLICATION_CONTROLLER_PRESENT)
#include "dali_addressing_16.h"

volatile uint8_t DALI_gearType_shortAddress[4];//type 5~8
volatile uint8_t DALI_shortAddress16_amount = 0;

extern volatile int test_F;
extern volatile uint16_t Time_out_f;
extern volatile uint16_t Time_out_c;
extern volatile int8_t ACK_STATE;
extern volatile uint8_t bus_ACK;

void dtr0_16(uint8_t data)
{
    sendData(CMD) = (DTR0 << 8) | data;
    setSendSetting(CMD, 16, 3, 0, 0);
    test_F = 1;
    while(test_F != 0){}
}

void dtr1_16(uint8_t data)
{
    sendData(CMD) = (DTR1 << 8) | data;
    setSendSetting(CMD, 16, 3, 0, 0);
    test_F = 1;
    while(test_F != 0){}
}

void dtr2_16(uint8_t data)
{
    sendData(CMD) = (DTR2 << 8) | data;
    setSendSetting(CMD, 16, 3, 0, 0);
    test_F = 1;
    while(test_F != 0){}
}

void DAPC(uint8_t address, uint8_t opcode)
{
    if ((address != BROADCAST) || (address != BROADCAST_UNADDRESSED))
        address = (address << 1) | 0;
    sendData(CMD) = (address << 8) | opcode;
    setSendSetting(CMD, 16, 2, 0, 0);
    test_F = 1;
    while(test_F != 0){}
}

void enableDeviceType(uint8_t type)
{
    sendData(CMD) = 0xC100 + type;
    setSendSetting(CMD, 16, 3, 0, 0);
    test_F = 1;
    while(test_F != 0){}
}

void setCMD16(uint8_t address, uint8_t opcode)
{
    if ((address != BROADCAST) || (address != BROADCAST_UNADDRESSED))
        address = (address << 1) | 1;
    sendData(CMD) = (address << 8) | opcode;
    setSendSetting(CMD, 16, 3, 0, 0);
    test_F = 1;
    while(test_F != 0){}
}

void setCMD16_2(uint8_t address, uint8_t opcode)
{
    if ((address != BROADCAST) || (address != BROADCAST_UNADDRESSED))
        address = (address << 1) | 1;
    sendData(CMD) = (address << 8) | opcode;
    setSendSetting(CMD, 16, 3, 1, 1);
    test_F = 1;
    while(test_F != 0){}
}

int8_t queryCMD16(uint8_t address, uint8_t opcode)
{
    if ((address != BROADCAST) || (address != BROADCAST_UNADDRESSED))
        address = (address << 1) | 1;
    sendData(CMD) = (address << 8) | opcode;
    setSendSetting(CMD, 16, 5, 0, 0);
    Time_out_f = TRUE;
    Time_out_c = 0;
    test_F = 2;
    while(test_F > 0 && Time_out_c <= 100){}
    Time_out_f = FALSE;
    return test_F;
}

void setSpecialCMD16(uint8_t address, uint8_t opcode)
{
    sendData(CMD) = (address << 8) | opcode;
    setSendSetting(CMD, 16, 2, 0, 0);
    test_F = 1;
    while(test_F != 0){}
}

void setSpecialCMD16_2(uint8_t address, uint8_t opcode)
{
    sendData(CMD) = (address << 8) | opcode;
    setSendSetting(CMD, 16, 2, 1, 1);
    test_F = 1;
    while(test_F != 0){}
}

int8_t querySpecialCMD16(uint8_t address, uint8_t opcode)
{
    sendData(CMD) = (address << 8) | opcode;
    setSendSetting(CMD, 16, 5, 0, 0);
    Time_out_f = TRUE;
    Time_out_c = 0;
    test_F = 2;
    while(test_F != 0 && Time_out_c <= 100){}
    Time_out_f = FALSE;
    return test_F;
}

void removeFromGroup16(uint8_t address)
{
    uint8_t N;

    for(N = 0; N < 16; N++)
    {
        setCMD16_2(address, REMOVE_FROM_GROUP_0 + N);
    }
}

void removeAddress16(uint8_t address)
{
    dtr0_16(0xFF);
    setCMD16_2(address, SET_SHORT_ADDRESS);
}

void searchAddr16(uint8_t cmd)
{
    uint16_t high = 0xFF, low = 0, mid = 0;

    while (1)
    {
        mid = (high + low) >> 1;
        setSpecialCMD16(cmd, (uint8_t)mid);
        ACK_STATE = querySpecialCMD16(COMPARE, 0);
        
        if(ACK_STATE == 2) //NACK
        {
            if (cmd == SEARCH_ADDR_L)
            {
                if ((high == low + 1) && (low == mid))
                    low = high;
                else if (high == low)
                    break;
                else
                    low = mid;
            }
            else
            {
                if ((high == low + 1) && (low == mid))
                    break;

                low = mid;
            }
        }
        else //ACK
        {
            if (cmd == SEARCH_ADDR_L)
            {
                if (((high == low + 1) && (high == mid)) || (high == low))
                    break;
            }
            else
            {
                if ((high == low + 1) && (high == mid))
                    break;
            }

            high = mid;
        }
     }
}

uint8_t Search_Address_16(void)
{
    uint8_t searching = 1;
    
    setSpecialCMD16_2(INITIALISE, 0);//INITAILIZE all
    setSpecialCMD16_2(RANDOMISE, 0);

    while(searching)
    {
        setSpecialCMD16(SEARCH_ADDR_L, 0xFF);
        setSpecialCMD16(SEARCH_ADDR_M, 0xFF);
        setSpecialCMD16(SEARCH_ADDR_H, 0xFF);
        ACK_STATE = querySpecialCMD16(COMPARE, 0);

        if (ACK_STATE <= 0)
        {
            setSpecialCMD16(SEARCH_ADDR_L, 0);
            setSpecialCMD16(SEARCH_ADDR_M, 0);
            searchAddr16(SEARCH_ADDR_H);
            searchAddr16(SEARCH_ADDR_M);
            searchAddr16(SEARCH_ADDR_L);
            setSpecialCMD16(PROGRAM_SHORT_ADDRESS, (DALI_shortAddress16_amount << 1) + 1);
            ACK_STATE = querySpecialCMD16(QUERY_SHORT_ADDRESS, 0);

            if (ACK_STATE == 0) //only one valid frame is received, the address is mapping to one device
            {
                setSpecialCMD16(WITHDRAW, 0);
                //can add other query cmds or read bank function here to check if the address is really mapping to only one device.
                ACK_STATE = queryCMD16(DALI_shortAddress16_amount, QUERY_DEVICE_TYPE);

                if (bus_ACK == 0xFF)
                {
                    do
                    {
                        ACK_STATE = queryCMD16(DALI_shortAddress16_amount, QUERY_NEXT_DEVICE_TYPE);

                        if (ACK_STATE == 0)
                            DALI_gearType_shortAddress[bus_ACK - 5] = DALI_shortAddress16_amount;//DALI_shortAddress16_amount+1;
                        else
                            break;

                    }while(bus_ACK != 0xFE);
                }
                else if (bus_ACK < 0xFE)
                {
                    DALI_gearType_shortAddress[bus_ACK - 5] = DALI_shortAddress16_amount;//DALI_shortAddress16_amount+1;
                }
                
                DALI_shortAddress16_amount++;
            }
            else if (ACK_STATE < 0) //more than one device response to QUERY_SHORT_ADDRESS and cause frame error
            {
                setSpecialCMD16(PROGRAM_SHORT_ADDRESS, 0xFF); //set these device short address to 0xFF and do RANDOMISE for unaddressed devices again.
                setSpecialCMD16_2(RANDOMISE, 0xFF);
            }
        }
        else
        {
            searching = 0;
            setSpecialCMD16_2(TERMINATE, 0);
        }
    }

    return 0;
}

void DALI_Addressing_16(void)
{
    ACK_STATE = queryCMD16(BROADCAST, QUERY_CONTROL_GEAR_PRESENT);

    if(ACK_STATE > 0)//Time out nobody's here
    {
        DALI_shortAddress16_amount = 0;
    }
    else
    {
        removeFromGroup16(BROADCAST);
        removeAddress16(BROADCAST);
        Search_Address_16();
        readGearData();
    }
}
#endif
