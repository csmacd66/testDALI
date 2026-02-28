#include <stdlib.h>
#include "dali_cd_app.h"

volatile uint8_t DALI_event = 0;
volatile uint8_t ID301_State;
volatile uint8_t ID302_State;
volatile uint8_t ID303_State;
volatile uint16_t ID304_Value;	 //lux
/*receive event*/
uint8_t Event(uint8_t address, uint8_t instance, uint8_t opcode)
{
    uint16_t eventSource = (address << 6) | (instance >> 2);
    uint16_t eventInfo = ((instance & 0x3) << 8) | opcode;
    uint8_t addrInfo = opcode & 0x7F;
    uint8_t groupInfo = ((instance & 0x1F) << 1) | (opcode >> 7);
    uint8_t addr, group, instype;


    if ((eventSource >> 3) == 0x7F7) //Device power cycle
    {
        if (groupInfo & BIT0) //device group valid
        {
            group = groupInfo & 0x1F; ////Device power cycle from groupAddr
        }
        else if (eventInfo & BIT0)
        {
            addr = addrInfo & 0x3F; //Device power cycle from shortAddr
        }
        else
            return 0; //invalid msg
    }
    else //Input Notification Event
    {
        addr = eventSource >> 7;
        instype = eventSource & 0x1F;

        if (addr & BIT6)
        {
            addr = addr & 0x1F;

            if (addr & BIT5)
            {
                //instance group + instance type
            }
            else
            {
                if (instype & BIT5)
                {
                    //device group + instance type
                }
                else
                {
                    //instance type + instance number
                    DALI_event |= 1 << (addr-1);

                    if(addr == 1) // 301
                    {
                        ID301_State = eventInfo;
                    }
                    else if(addr == 2) //302
                    {
                        ID302_State = eventInfo;
                    }
                    else if(addr == 3) //303
                    {
                        ID303_State = eventInfo;
                    }
                    else if(addr == 4) //304
                    {
                        ID304_Value = eventInfo;
                    }
                }
            }
        }
        else
        {
            addr = addr & 0x3F;

            if (instype & BIT5)
            {
                //short address + instance number
            }
            else
            {
                //short address + instance type
            }
        }
    }
    return 0;
}

