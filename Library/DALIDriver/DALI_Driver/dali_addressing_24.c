#include <stdlib.h>
#include <string.h>
#include "NuMicro.h"
#include "dali_cd_app.h"
#if(APPLICATION_CONTROLLER_PRESENT)
#include "dali_addressing_24.h"

volatile uint8_t DALI_deviceType_shortAddress[4];//301 ~ 304
volatile uint8_t DALI_shortAddress24_amount = 0;

extern volatile int test_F;
extern volatile uint16_t Time_out_f;
extern volatile uint16_t Time_out_c;
extern volatile int8_t ACK_STATE;
extern volatile uint8_t bus_ACK;
extern void DALI_Addressing_16(void);

void dtr0_24(uint8_t data)
{
    sendData(CMD) = (SPECIAL_COMMAND << 16) | (DTR0 << 8) | data;
    setSendSetting(CMD, 24, 3, 0, 0);
    test_F = 1;
    while(test_F != 0){}
}

void dtr1_24(uint8_t data)
{
    sendData(CMD) = (SPECIAL_COMMAND << 16) | (DTR1 << 8) | data;
    setSendSetting(CMD, 24, 3, 0, 0);
    test_F = 1;
    while(test_F != 0){}
}

void dtr2_24(uint8_t data)
{
    sendData(CMD) = (SPECIAL_COMMAND << 16) | (DTR2 << 8) | data;
    setSendSetting(CMD, 24, 3, 0, 0);
    test_F = 1;
    while(test_F != 0){}
}

void dtr1_dtr0(uint8_t data1, uint8_t data0)
{
    sendData(CMD) = (DTR1_DTR0 << 16) | (data1 << 8) | data0;
    setSendSetting(CMD, 24, 3, 0, 0);
    test_F = 1;
    while(test_F != 0){}
}

void dtr2_dtr1(uint8_t data2, uint8_t data1)
{
    sendData(CMD) = (DTR2_DTR1 << 16) | (data2 << 8) | data1;
    setSendSetting(CMD, 24, 3, 0, 0);
    test_F = 1;
    while(test_F != 0){}
}

int8_t direct_write_momory(uint8_t offset, uint8_t data)
{
    sendData(CMD) = (DIRCT_WRITE_MEMORY << 16) | (offset << 8) | data;
    setSendSetting(CMD, 24, 3, 0, 0);
    Time_out_f = TRUE;
    Time_out_c = 0;
    test_F = 2;
    while(test_F != 0 && Time_out_c <= 100){}
    Time_out_f = FALSE;
    return test_F;
}

void setStandardCMD24(uint8_t address, uint8_t opcode)
{
    if ((address != BROADCAST) || (address != BROADCAST_UNADDRESSED))
        address = (address << 1) | 1;
    sendData(CMD) = (address << 16) | (DEVICE << 8) | opcode;
    setSendSetting(CMD, 24, 3, 1, 1);
    test_F = 1;
    while(test_F != 0){}
}

int8_t queryStandardCMD24(uint8_t address, uint8_t opcode)
{
    if ((address != BROADCAST) || (address != BROADCAST_UNADDRESSED))
        address = (address << 1) | 1;
    sendData(CMD) = (address << 16) | (DEVICE << 8) | opcode;
    setSendSetting(CMD, 24, 5, 0, 0);
    Time_out_f = TRUE;
    Time_out_c = 0;
    test_F = 2;
    while(test_F > 0 && Time_out_c <= 100){}
    Time_out_f = FALSE;
    return test_F;
}

void setInstanceCMD24(uint8_t address, uint8_t instance, uint8_t opcode)
{
    if ((address != BROADCAST) || (address != BROADCAST_UNADDRESSED))
        address = (address << 1) | 1;
    sendData(CMD) = (address << 16) | (instance << 8) | opcode;
    setSendSetting(CMD, 24, 3, 1, 1);
    test_F = 1;
    while(test_F != 0){}
}

int8_t queryInstanceCMD24(uint8_t address, uint8_t instance, uint8_t opcode)
{
    if ((address != BROADCAST) || (address != BROADCAST_UNADDRESSED))
        address = (address << 1) | 1;
    sendData(CMD) = (address << 16) | (instance << 8) | opcode;
    setSendSetting(CMD, 24, 5, 0, 0);
    Time_out_f = TRUE;
    Time_out_c = 0;
    test_F = 2;
    while(test_F > 0 && Time_out_c <= 100){}
    Time_out_f = FALSE;
    return test_F;
}

void setSpecialCMD24(uint8_t instance, uint8_t opcode)
{
    sendData(CMD) = (SPECIAL_COMMAND << 16) | (instance << 8) | opcode;
    setSendSetting(CMD, 24, 2, 0, 0);
    test_F = 1;
    while(test_F != 0){}
}

void setSpecialCMD24_2(uint8_t instance, uint8_t opcode)
{
    sendData(CMD) = (SPECIAL_COMMAND << 16) | (instance << 8) | opcode;
    setSendSetting(CMD, 24, 2, 1, 1);
    test_F = 1;
    while(test_F != 0){}
}

int8_t querySpecialCMD24(uint8_t instance, uint8_t opcode)
{
    sendData(CMD) = (SPECIAL_COMMAND << 16) | (instance << 8) | opcode;
    setSendSetting(CMD, 24, 5, 0, 0);
    Time_out_f = TRUE;
    Time_out_c = 0;
    test_F = 2;
    while(test_F != 0 && Time_out_c <= 100){}
    Time_out_f = FALSE;
    return test_F;
}

void removeFromGroup24(uint8_t address)
{
    setStandardCMD24(address, REMOVE_FROM_DEVICE_GROUPS_0_15);
    setStandardCMD24(address, REMOVE_FROM_DEVICE_GROUPS_16_31);
}

void removeAddress24(uint8_t address)
{
    dtr0_24(0xFF);
    setStandardCMD24(address, SET_SHORT_ADDRESS);
}

void searchAddr24(uint8_t cmd)
{
    uint16_t high = 0xFF, low = 0, mid = 0;

    while (1)
    {
        mid = (high + low) >> 1;
        setSpecialCMD24(cmd, (uint8_t)mid);
        ACK_STATE = querySpecialCMD24(COMPARE, 0);
        
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

uint8_t Search_Address_24(void)
{
    uint8_t searching = 1;
    
    setSpecialCMD24_2(INITIALISE, BROADCAST);//INITAILIZE all
    setSpecialCMD24_2(RANDOMISE, 0);

    while(searching)
    {
        setSpecialCMD24(SEARCH_ADDR_L, 0xFF);
        setSpecialCMD24(SEARCH_ADDR_M, 0xFF);
        setSpecialCMD24(SEARCH_ADDR_H, 0xFF);
        ACK_STATE = querySpecialCMD24(COMPARE, 0);

        if (ACK_STATE <= 0)
        {
            setSpecialCMD24(SEARCH_ADDR_L, 0);
            setSpecialCMD24(SEARCH_ADDR_M, 0);
            searchAddr24(SEARCH_ADDR_H);
            searchAddr24(SEARCH_ADDR_M);
            searchAddr24(SEARCH_ADDR_L);
            setSpecialCMD24(PROGRAM_SHORT_ADDRESS, DALI_shortAddress24_amount);
            ACK_STATE = querySpecialCMD24(QUERY_SHORT_ADDRESS, 0);

            if (ACK_STATE == 0) //only one valid frame is received, the address is mapping to one device
            {
                setSpecialCMD24(WITHDRAW, 0);
                //can add other query cmds or read bank function here to check if the address is really mapping to only one device.
                ACK_STATE = queryStandardCMD24(DALI_shortAddress24_amount, QUERY_DEVICE_CAPABILITIES);
                ACK_STATE = queryStandardCMD24(DALI_shortAddress24_amount, QUERY_NUMBER_OF_INSTANCES);

                if (bus_ACK > 0)
                {

                }
                
                DALI_shortAddress24_amount++;
            }
            else if (ACK_STATE < 0) //more than one device response to QUERY_SHORT_ADDRESS and cause frame error
            {
                setSpecialCMD24(PROGRAM_SHORT_ADDRESS, 0xFF); //set these device short address to 0xFF and do RANDOMISE for unaddressed devices again.
                setSpecialCMD24_2(RANDOMISE, 0xFF);
            }
        }
        else
        {
            searching = 0;
            setSpecialCMD24_2(TERMINATE, 0);
        }
    }

    return 0;
}

void DALI_Addressing_24(void)
{
    setStandardCMD24(BROADCAST, START_QUIESCENT_MODE);
    DALI_Addressing_16();
    ACK_STATE = queryStandardCMD24(BROADCAST, QUERY_DEVICE_STATUS);

    if(ACK_STATE > 0)//Time out no device ACK
    {
        DALI_shortAddress24_amount = 0;
    }
    else //receive valid or broken frame
    {
        removeFromGroup24(BROADCAST);
        removeAddress24(BROADCAST);
        Search_Address_24();
    }

    setStandardCMD24(BROADCAST, STOP_QUIESCENT_MODE);
}
#endif
