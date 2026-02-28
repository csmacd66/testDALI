#include <stdlib.h>
#include "dali_cd_app.h"
#include "board_init.h"
#if(APPLICATION_CONTROLLER_PRESENT)
volatile uint8_t bus_ACK;
#endif
volatile int test_F = 0;

void checkReceiveFinish(void)
{
    uint8_t receiveByte = 0;
#if(APPLICATION_CONTROLLER_PRESENT)
    bus_ACK = 0;
#endif
    //The system will automatically receive and decode any signal input from DRx pin. DALI signal
    //reception action has the highest priority in the system. DALI signal reception use GPIO
    //interrupt and DALI_TIMER capture function. Please do not use DALI_TIMER for other usage.
    //The received data will be decoded in ParseCapData(), and shown in main().
    //NOTE: NDA103 Series use TIMER0 as DALI_TIMER.
    receiveByte = ParseCapData();   //Decode a received data in main().

    if (receiveByte == 3) // Part 103 and Part 3XX command
    {
        if (isCommand(ADDRESS) == TRUE)
            Command(ADDRESS, INSTANCE, OPCODE);
        else
            Event(ADDRESS, INSTANCE, OPCODE);
    }
    else if (receiveByte == 2) // Part 102 and Part 2XX command
    {
    }
    else if (receiveByte == 1) // 8-bit Response
    {
#if(APPLICATION_CONTROLLER_PRESENT)
        if(test_F == 2) //wait for query responce
        {
            test_F = 0;
            bus_ACK = ACK;
        }
#endif
    }
    else //error
    {
#if(APPLICATION_CONTROLLER_PRESENT)
        if (test_F == 2)
            test_F = -1;
#endif
        g_parameter.twice = 0;
    }

    startReceive();
}

