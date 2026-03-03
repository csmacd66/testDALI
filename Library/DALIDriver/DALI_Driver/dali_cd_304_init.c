#include <math.h>
#include "dali_cd_app.h"
#if(USE_PART304)
extern volatile uint8_t F_ALS_Ready;

void DefaultInstance304(uint8_t instanceNumber)
{
    uint8_t i;
    int8_t index = calInstanceIndex(instanceNumber);

    if (index < 0)
    {
        while (1); //ERROR: The instance number order is wrong or the sensor amount is larger than define in DALI_CD_APP.h.
    }

    instanceType(instanceNumber) = PART304;
    resolution(instanceNumber) = 10; //customer define, depends on the sensor
    extendedVersionNumber(instanceNumber) = Version(2, 0);
    instanceActive(instanceNumber) = TRUE;

    for (i = 0; i < 3; i++)
    {
        instanceGroup(instanceNumber, i) = MASK_u8;
    }

    eventScheme(instanceNumber) = 0;
    eventPriority(instanceNumber) = 4;
    eventFilter(instanceNumber) = 1;
    instanceError(instanceNumber) = FALSE;
    tDeadtime(instanceNumber) = 30;//*50=1500 ms = 1.5 sec
    tReport(instanceNumber) = 30;//*1000=30000 ms = 30 sec
    instanceErrorByte(instanceNumber) = 0;
    hysteresisMin(instanceNumber) = var_hysteresisMinDefault(index);
    hysteresis(instanceNumber) = 5;
}

void ResetInstance304(uint8_t instanceNumber)
{
    uint8_t i;
    int8_t index = calInstanceIndex(instanceNumber);

    if (index < 0)
    {
        while (1); //ERROR: The instance number order is wrong or the sensor amount is larger than define in DALI_CD_APP.h.
    }

    for (i = 0; i < 3; i++)
    {
        instanceGroup(instanceNumber, i) = MASK_u8;
    }

    var_hysteresisMinDefault(index) = checkHysteresisMinDefault(instanceNumber);
    eventScheme(instanceNumber) = 0;
    eventPriority(instanceNumber) = 4;
    eventFilter(instanceNumber) = 1;
    instanceError(instanceNumber) = FALSE;
    tDeadtime(instanceNumber) = 30;
    tReport(instanceNumber) = 30;
    hysteresisMin(instanceNumber) = var_hysteresisMinDefault(index);
    hysteresis(instanceNumber) = 5;
}

void PowerOnInstance304(uint8_t instanceNumber)
{
    uint8_t u8tmp1 = instanceTypeIndex(instanceNumber);
    uint32_t u32tmp = tReport(instanceNumber) * Tincr_report(u8tmp1);
    int8_t index = calInstanceIndex(instanceNumber);

    if (index < 0)
    {
        while (1); //ERROR: The instance number order is wrong or the sensor amount is larger than define in DALI_CD_APP.h.
    }

    instanceError(instanceNumber) = FALSE;
    instanceErrorByte(instanceNumber) = 0;
    inputValueMask(instanceNumber) = (uint32_t)pow(2, resolution(instanceNumber)) - 1;
    var_hysteresisMinDefault(index) = checkHysteresisMinDefault(instanceNumber);
    var_hysteresisBand(index) = 0;
    var_hysteresisBandHigh(index) = 0;
    var_hysteresisBandLow(index) = 0;
    F_deadtime(instanceNumber) = TRUE;
    cDeadtime(instanceNumber) = setDeadtime(index);
}

uint32_t setInputValue304(uint8_t instanceNumber) // Do not change the API name
{
    return SensorValue[instanceNumber];
}

uint8_t checkEvent304(uint8_t instanceNumber, uint8_t instanceErrorState)
{
    uint8_t state = 0;

    if (instanceErrorState)
    {
        setInstanceError(instanceNumber);
        clearSendFlag(instanceNumber);
    }
    else
    {
        clearInstanceError(instanceNumber);

        if (F_ALS_Ready)
        {
            checkLightSensor(instanceNumber);
            F_ALS_Ready = F_outsideHysteresis(instanceNumber);
        }

        checkReportTime(instanceNumber);
        state = checkDeadtime(instanceNumber);
    }

    return state;
}

#endif