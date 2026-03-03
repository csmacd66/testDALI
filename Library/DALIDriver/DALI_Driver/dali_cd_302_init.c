#include "math.h"
#include "dali_cd_app.h"
#if(USE_PART302)
void DefaultInstance302(uint8_t instanceNumber)
{
    uint8_t i;
    int8_t index = calInstanceIndex(instanceNumber);

    if (index < 0)
    {
        while (1); //ERROR: The instance number order is wrong or the sensor amount is larger than define in DALI_CD_APP.h.
    }

    instanceType(instanceNumber) = PART302;

    /* The setting of resolution() value is depends on the type of external switches or sliders.              */
    /* Please refer to IEC 62386-302 Annex A for more examples of setting resolution for switches or sliders. */
    if (index <= AMOUNT_OF_302_switch)
    {
        switchType(instanceNumber) = SWITCH;
        resolution(instanceNumber) = 1;  //customer define
    }
    else
    {
        switchType(instanceNumber) = SLIDER;
        resolution(instanceNumber) = 10; //customer define
    }

    extendedVersionNumber(instanceNumber) = Version(2, 0);
    instanceActive(instanceNumber) = TRUE;

    for (i = 0; i < 3; i++)
    {
        instanceGroup(instanceNumber, i) = MASK_u8;
    }

    eventScheme(instanceNumber) = 0;
    eventPriority(instanceNumber) = 3;
    eventFilter(instanceNumber) = 1;
    instanceError(instanceNumber) = FALSE;
    tDeadtime(instanceNumber) = 2;
    tReport(instanceNumber) = 0;
    instanceErrorByte(instanceNumber) = 0;
}

void ResetInstance302(uint8_t instanceNumber)
{
    uint8_t i;

    for (i = 0; i < 3; i++)
    {
        instanceGroup(instanceNumber, i) = MASK_u8;
    }

    eventScheme(instanceNumber) = 0;
    eventPriority(instanceNumber) = 3;
    eventFilter(instanceNumber) = 1;
    instanceError(instanceNumber) = FALSE;
    tDeadtime(instanceNumber) = 2;
    tReport(instanceNumber) = 0;
}

void PowerOnInstance302(uint8_t instanceNumber)
{
    uint8_t u8tmp1 = instanceTypeIndex(instanceNumber);
    uint32_t u32tmp = tReport(instanceNumber) * Tincr_report(u8tmp1);
    int8_t index = calInstanceIndex(instanceNumber);
    uint8_t no = instanceNumber - startInstanceNo(index);

    if (index < 0)
    {
        while (1); //ERROR: The instance number order is wrong or the sensor amount is larger than define in DALI_CD_APP.h.
    }

    instanceError(instanceNumber) = FALSE;
    instanceErrorByte(instanceNumber) = 0;
    F_deadtime(instanceNumber) = TRUE;
    //    cDeadtime(instanceNumber) = setDeadtime(index);
    
    if (index <= AMOUNT_OF_302_switch)
        dataMax(no) = 1;
    else
        dataMax(no) = 4096; //ADC sampling result max value
    
    if (resolution(instanceNumber) <= 10)
        resoMax(no) = pow(2, resolution(instanceNumber)) - 1;
    else
        resoMax(no) = pow(2, 10) - 1;
}

uint32_t setInputValue302(uint8_t instanceNumber) // Do not change the API name
{
    uint8_t tmpValue = SensorValue[instanceNumber];
    return SensorValue[instanceNumber] * resoMax(instanceNumber) / dataMax(instanceNumber);
}

uint8_t checkEvent302(uint8_t instanceNumber)
{
    uint8_t state = 0;
    checkSwitch(instanceNumber);
    checkReportTime(instanceNumber);
    state = checkDeadtime(instanceNumber);
    return state;
}

#endif