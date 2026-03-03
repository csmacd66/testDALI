#include "dali_cd_app.h"
#if(USE_PART301)
volatile uint32_t u32Compensate = 30;

void DefaultInstance301(uint8_t instanceNumber)
{
    uint8_t i;
    int8_t index = calInstanceIndex(instanceNumber);

    if (index < 0)
    {
        while (1); //ERROR: The instance number order is wrong or the sensor amount is larger than define in DALI_CD_APP.h.
    }

    instanceType(instanceNumber) = PART301;
    resolution(instanceNumber) = 1;
    extendedVersionNumber(instanceNumber) = Version(2, 1);
    instanceActive(instanceNumber) = TRUE;

    for (i = 0; i < 3; i++)
    {
        instanceGroup(instanceNumber, i) = MASK_u8;
    }

    eventScheme(instanceNumber) = 0;
    eventPriority(instanceNumber) = 3;
    eventFilter(instanceNumber) = 0xF4;
    instanceError(instanceNumber) = FALSE;
    tShortMin(instanceNumber) = 10;  // customer define, value range: 10 ~ 255
    tDoubleMin(instanceNumber) = 25; // customer define, value range: 10 ~ 255
    tShort(instanceNumber) = clamp(tShortMin(instanceNumber), 25, 255); // value range: tShortMin ~ 255, default value: Max(25, tShortMin)
    tDouble(instanceNumber) = 0;
    tRepeat(instanceNumber) = 8;
    tStuck(instanceNumber) = 20;
    instanceErrorByte(instanceNumber) = 0;
    old_amount_301(instanceNumber) = SW_AMOUNT_OF_301;
}

void ResetInstance301(uint8_t instanceNumber)
{
    uint8_t i;

    for (i = 0; i < 3; i++)
    {
        instanceGroup(instanceNumber, i) = MASK_u8;
    }

    eventScheme(instanceNumber) = 0;
    eventPriority(instanceNumber) = 3;
    eventFilter(instanceNumber) = 0xF4;
    instanceError(instanceNumber) = FALSE;
    tShort(instanceNumber) = clamp(tShortMin(instanceNumber), 25, 255);
    tDouble(instanceNumber) = 0;
    tRepeat(instanceNumber) = 8;
    tStuck(instanceNumber) = 20;
    instanceErrorByte(instanceNumber) = 0;
}

void PowerOnInstance301(uint8_t instanceNumber)
{
    instanceError(instanceNumber) = FALSE;
    instanceErrorByte(instanceNumber) = 0;
    F_sendEvent(instanceNumber) = FALSE;
    inputValue(instanceNumber) = setInputValue(instanceNumber);
    F_deadtime(instanceNumber) = TRUE;
    S_buttonStatus(instanceNumber) = S_ABSENCE;
    Tincr_sample = 5; //5 ms sampling time
}

uint32_t setInputValue301(uint8_t instanceNumber) // Do not change the API name
{
    //Customer self-defined API
    if (SensorValue[instanceNumber])
    {
        return 0xFF;
    }

    return 0x00;
}

uint8_t checkEvent301(uint8_t instanceNumber)
{
    return checkDeadtime(instanceNumber);
}
#endif
