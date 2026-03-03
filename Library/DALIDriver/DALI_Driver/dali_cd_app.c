#include <stdlib.h>
#include <string.h>
#include "dali_cd_app.h"

volatile parameter_t g_parameter;
volatile uint8_t NVM_SIZE[TOTAL_NVM_TYPE + NUMBER_OF_BANKS];
volatile uint8_t *NVM_ARRAY[TOTAL_NVM_TYPE + NUMBER_OF_BANKS];
volatile transmitPara_t transmitPara[NUMBER_OF_INSTANCES + 2];

#if(NUMBER_OF_INSTANCES)
volatile instance_t instanceNo[NUMBER_OF_INSTANCES];
volatile instanceInfo_t instanceInfo[TYPES_OF_INSTANCES];
volatile uint32_t SensorValue[NUMBER_OF_INSTANCES]; //The customer needs to enter the input value of the sensor

//302 all parameters are defined in instance.h, no parameter is yet defined.
#if(USE_PART301)
    volatile parameter301_t g_parameter301;
#endif
#if(USE_PART302)
    volatile parameter302_t g_parameter302[AMOUNT_OF_302];
#endif
#if(USE_PART303)
    volatile parameter303_t g_parameter303[AMOUNT_OF_303];
#endif
#if(USE_PART304)
    volatile parameter304_t g_parameter304[AMOUNT_OF_304];
#endif

uint8_t INSTANCE_TYPE[TYPES_OF_INSTANCES] =
{
#if(USE_PART301)
    PART301,
#endif
#if(USE_PART302)
    PART302,
#endif
#if(USE_PART303)
    PART303,
#endif
#if(USE_PART304)
    PART304,
#endif
};
#else
volatile instance_t instanceNo[1];
volatile instanceInfo_t instanceInfo[1];
#endif
volatile feature_t featureNo[1];

void checkDeviceResetState(void)
{
    uint8_t instanceNumber;
    sta_resetState = TRUE;
    checkResetState();//Check 103 reset state
#if(NUMBER_OF_INSTANCES)

    for (instanceNumber = 0; instanceNumber < g_parameter.numberOfInstances; instanceNumber++)
    {
        (*checkResetStateInstance(instanceTypeIndex(instanceNumber)))(instanceNumber);
    }

#endif
}

void initInstanceInfo(uint8_t instanceType, uint8_t instanceTypeIndex, uint8_t instanceNumber)
{
    switch (instanceType)
    {
#if(USE_PART301)

        case 1:
            initInstanceInfo301(instanceNumber, instanceTypeIndex, (uint8_t)SW_AMOUNT_OF_301);
            break;
#endif
#if(USE_PART302)

        case 2:
            initInstanceInfo302(instanceNumber, instanceTypeIndex, (uint8_t)AMOUNT_OF_302);
            break;
#endif
#if(USE_PART303)

        case 3:
            initInstanceInfo303(instanceNumber, instanceTypeIndex, (uint8_t)AMOUNT_OF_303);
            break;
#endif
#if(USE_PART304)

        case 4:
            initInstanceInfo304(instanceNumber, instanceTypeIndex, (uint8_t)AMOUNT_OF_304);
            break;
#endif

        default:
            break;
    }
}

void initInstanceParameter(uint8_t instanceNumber, uint8_t type)
{
    switch (type)
    {
#if(USE_PART303)

        case 3:
            parameter303(instanceNumber);
            break;
#endif
        default:
            break;
    }
}

void initDefaultInstance(uint8_t instanceNumber, uint8_t type)
{
    switch (type)
    {
#if(USE_PART301)

        case 1:
            DefaultInstance301(instanceNumber);
            break;
#endif
#if(USE_PART302)

        case 2:
            DefaultInstance302(instanceNumber);
            break;
#endif
#if(USE_PART303)

        case 3:
            DefaultInstance303(instanceNumber);
            break;
#endif
#if(USE_PART304)

        case 4:
            DefaultInstance304(instanceNumber);
            break;
#endif

        default:
            break;
    }
}


void initResetInstance(uint8_t instanceNumber)
{
    switch (instanceType(instanceNumber))
    {
#if(USE_PART301)

        case 1:
            ResetInstance301(instanceNumber);
            break;
#endif
#if(USE_PART302)

        case 2:
            ResetInstance302(instanceNumber);
            break;
#endif
#if(USE_PART303)

        case 3:
            ResetInstance303(instanceNumber);
            break;
#endif
#if(USE_PART304)

        case 4:
            ResetInstance304(instanceNumber);
            break;
#endif
    }
}

void initPowerOnInstance(uint8_t instanceNumber)
{
    switch (instanceType(instanceNumber))
    {
#if(USE_PART301)

        case 1:
            PowerOnInstance301(instanceNumber);
            break;
#endif
#if(USE_PART302)

        case 2:
            PowerOnInstance302(instanceNumber);
            break;
#endif
#if(USE_PART303)

        case 3:
            PowerOnInstance303(instanceNumber);
            break;
#endif
#if(USE_PART304)

        case 4:
            PowerOnInstance304(instanceNumber);
            break;
#endif
    }
}

uint32_t setInputValue(uint8_t instanceNumber)
{
    switch (instanceType(instanceNumber))
    {
#if(USE_PART301)

        case 1:
            return setInputValue301(instanceNumber);
#endif
#if(USE_PART302)

        case 2:
            return setInputValue302(instanceNumber);
#endif
#if(USE_PART303)

        case 3:
            return setInputValue303(instanceNumber);
#endif
#if(USE_PART304)

        case 4:
            return setInputValue304(instanceNumber);
#endif
    }

    return 0;
}

uint16_t setEvent(uint8_t instanceNumber, uint32_t u32inputValue)
{
    switch (instanceType(instanceNumber))
    {
            //#if(USE_PART301)

            //        case 1:
            //            return setEvent301(instanceNumber, u32inputValue);
            //#endif
#if(USE_PART302)

        case 2:
            return setEvent302(instanceNumber, u32inputValue);
#endif
#if(USE_PART303)

        case 3:
            return setEvent303(instanceNumber, u32inputValue);
#endif
#if(USE_PART304)

        case 4:
            return setEvent304(instanceNumber, u32inputValue);
#endif
    }

    return 0;
}

bool checkEventFilter(uint8_t instanceNumber)
{
    if (sta_quiescentMode == DISABLE)
    {
        switch (instanceType(instanceNumber))
        {
#if(USE_PART301)

            case 1:
                return checkEventFilter301(instanceNumber);
#endif
#if(USE_PART302)

            case 2:
                return checkEventFilter302(instanceNumber);
#endif
#if(USE_PART303)

            case 3:
                return checkEventFilter303(instanceNumber);
#endif
#if(USE_PART304)

            case 4:
                return checkEventFilter304(instanceNumber);
#endif
        }
    }

    return 0;
}

uint8_t checkEventFilterMaximum(uint8_t instanceNumber, uint8_t byte)
{
    switch (instanceType(instanceNumber))
    {
#if(USE_PART301)

        case 1:
            return checkEventFilterMaximum301(byte);
#endif
#if(USE_PART302)

        case 2:
            return checkEventFilterMaximum302(byte);
#endif
#if(USE_PART303)

        case 3:
            return checkEventFilterMaximum303(byte);
#endif
#if(USE_PART304)

        case 4:
            return checkEventFilterMaximum304(byte);
#endif

        default:
            return 1;
    }
}

uint8_t checkEventFilterLimit(uint8_t instanceNumber, uint8_t byte)
{
    switch (instanceType(instanceNumber))
    {
#if(USE_PART301)

        case 1:
            return checkEventFilterLimit301(instanceNumber, byte);
#endif
#if(USE_PART302)

        case 2:
            return checkEventFilterLimit302(instanceNumber, byte);
#endif
#if(USE_PART303)

        case 3:
            return checkEventFilterLimit303(instanceNumber, byte);
#endif
#if(USE_PART304)

        case 4:
            return checkEventFilterLimit304(instanceNumber, byte);
#endif

        default:
            switch (byte)
            {
                case 0:
                    return var_DTR0;

                case 1:
                    return var_DTR1;

                case 2:
                    return var_DTR2;

                default:
                    return 0;
            }
    }
}

uint8_t checkRepeatEnable(uint8_t instanceNumber)
{
    switch (instanceType(instanceNumber))
    {
#if(USE_PART301)

        case 1:
            return 0;
#endif
#if(USE_PART302)

        case 2:
            if (tReport(instanceNumber))
                return 1;
            else
                return 0;

#endif
#if(USE_PART303)

        case 3:
            if (tReport(instanceNumber))
                return isRepeatEventEnabled(instanceNumber);
            else
                return 0;

#endif
#if(USE_PART304)

        case 4:
            if (tReport(instanceNumber))
                return 1;
            else
                return 0;

#endif
    }

    return 0;
}

uint8_t checkWarmUp(uint8_t instanceNumber)
{
    switch (instanceType(instanceNumber))
    {
#if(USE_PART301)

        case 1:
#endif
#if(USE_PART302)
        case 2:
#endif
#if(USE_PART304)
        case 4:
#endif
        default:
            return 1;
#if(USE_PART303)

        case 3:
            return checkWarmUp303(instanceNumber);
#endif

    }
}

void clearSendFlag(uint8_t instanceNumber)
{
    F_sendEvent(instanceNumber) = FALSE;
    F_send(instanceNumber + 2) = FALSE;
}

