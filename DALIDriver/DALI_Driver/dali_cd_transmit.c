#include <stdlib.h>
#include <time.h>
#include "dali_cd_app.h"

/**
  * @brief      set the settling time between two forward frames depends on priority
  *
  * @param[in]  priority      0: between forward frame and backward frame
  *                           1~5: use priority level 1~5
  *                           others: send in a transaction
  *
  * @return     settlingTime  Bus IDLE time between two forward frames.
  *
  * @details    settlingTime = (rand() % x) + y - z
  *             y = minimum. x = maximum - minimum + 1. z = stop condition time.
  *             settlingTime is a random point of time within the minimum and maximum settling time to avoid collisions for multi-master transmitters.
  *             The settling time between two frames is start from stop condition and end at the next start bit defined in Part 101 Figure 13.
  *             But in this project, the settlingTime counter will start counting after stop condition.
  *             The stop condition is 2.45 ms, so settlingTime - 2.45 to meet the Part 101 spec.
  */
uint32_t setSettlingTime(uint8_t priority)
{
    switch (priority)
    {
        case 1://13.5~14.7ms //second cmd of send twice
            return rand() % 7 + 118;

        case 2://14.9~16.1ms //addressing, switching or dimming actions triggered via push-button or presence detector (human action)
            return rand() % 7 + 132;

        case 3://16.3~17.7ms //configuration, banks write, cmds not covered by Priorities 2 and 4
            return rand() % 9 + 146;

        case 4://17.9~19.3ms //switching or dimming actions triggered by a light sensor or timer
            return rand() % 9 + 163;

        default:
        case 5://19.5~21.1ms //periodic query commands
            return rand() % 11 + 179;

        case 0://5.5~10.5ms //ACK
            return rand() % 20 + 50;
    }
}

/**
  * @brief      Set parameters related to transmition
  *
  * @param[in]  type
  *                         CMD: send command or power cycle notification
  *                         instanceNumber + 2: send instance input event notification
  *             length
  *                         24: 24-bit forward frame
  *                         16: 16-bit forward frame
  *             priority
  *                         1~5: use priority level 1~5
  *                         others: send in a transaction
  *             repeat
  *                         1~3: repeat times. send twice: repeat = 1
  *                         0, others: send once
  *             repeatWithPriority
  *                         0: use the first frame's priority to send the second frame
  *                         1: use priority 1 to send the second frame
  *
  * @return   None
  *
  * @details  This function is used to set the setting of forward frame. User need to set
  *           send_data(x) value first, then set the setting depends on the forward frame type.
  *
  */
void setSendSetting(uint8_t type, uint8_t length, uint8_t priority, uint8_t repeat, bool repeatWithPriority)
{
    uint8_t u8T = 0, u8A = 0, u8R = 0, u8P = 0;

    if (type < 2)
    {
        F_send(type) = TRUE;
    }

    settlingTime(type) = setSettlingTime(priority);

    if (repeatWithPriority == 0)
    {
        u8T = 1;
    }

    if (length == 16)
    {
        u8A = 1;
    }

    if (repeat <= 3)
    {
        u8R = repeat;
    }

    if ((priority >= 1) && (priority <= 5))
    {
        u8P = priority;
    }

    sendSetting(type) = (u8T << 6) | (u8A << 5) | (u8R << 3) | u8P;
}

void sendPowerCycleNotification(void)
{
    uint8_t i = 0, lowestGroup = 0;

    if (var_deviceGroups)
    {
        for (i = 0; i < 32; i++)
        {
            if ((var_deviceGroups >> i) & 1)
                break;
        }

        lowestGroup = i | (1 << 5);
    }
 
    if (var_shortAddress != MASK_u8)
        sendData(CMD) = (0x7F7 << 13) | (lowestGroup << 7) | (1 << 6) | (var_shortAddress);
    else
        sendData(CMD) = (0x7F7 << 13) | (lowestGroup << 7);

    setSendSetting(CMD, 24, 2, 0, 0);
}

uint8_t sendInputNotification(uint8_t instanceNumber, uint16_t event)
{
#if(NUMBER_OF_INSTANCES)
    uint8_t byteH, byteM, i, tmpValue;
    uint16_t byteL;
    byteL = event;

    switch (eventScheme(instanceNumber))
    {
        case 1:
            byteH = var_shortAddress;
            byteM = instanceType(instanceNumber);
            break;

        case 2:
            byteH = var_shortAddress;
            byteM = instanceNumber | (1 << 5);
            break;

        case 3:
            for (i = 0; i < 32;)
            {
                if (((var_deviceGroups >> i) & 1) != 1)
                    i++;
                else
                    break;
            }

            byteH = (i) | (1 << 6);
            byteM = instanceType(instanceNumber);
            break;

        case 0:
            byteH = instanceType(instanceNumber) | (1 << 6);
            byteM = instanceNumber | (1 << 5);
            break;

        case 4:
            byteH = instanceGroup(instanceNumber, 0) | (1 << 5) | (1 << 6);
            byteM = instanceType(instanceNumber);
            break;

        default:
            return 0;
    }

    sendData(instanceNumber + 2) = (byteH << 17) | (byteM << 10) | (byteL);

    if (F_repeatTrigger(instanceNumber)) //repeat event use priority 5
    {
        setSendSetting(instanceNumber + 2, 24, 5, 0, 0);
        F_repeatTrigger(instanceNumber) = FALSE;
    }
    else
    {
        setSendSetting(instanceNumber + 2, 24, eventPriority(instanceNumber), 0, 0);
    }

    return 1;
#else
    return 0;
#endif
}

