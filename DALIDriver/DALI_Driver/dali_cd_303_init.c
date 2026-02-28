#include "dali_cd_app.h"
#if(USE_PART303)
extern volatile uint8_t F_PIRWarmUpEnd[AMOUNT_OF_303];

void parameter303(uint8_t instanceNumber)
{
    int8_t index = calInstanceIndex(instanceNumber);

    if (index < 0)
    {
        while (1); //ERROR: The instance number order is wrong or the sensor amount is larger than define in DALI_CD_APP.h.
    }

    SENSOR_TYPE(instanceNumber) = MOVEMENT_SENSOR; //Depends on the type of occupancy sensor. MOVEMENT or PRESENCE

    /* var_occupancyCapabilities:                                                                                                             */
    /*                          Bit 0: Configuration and querying of "detectionRange" supported, 1: Y, 0: N.                                  */
    /*                          Bit 1: Configuration and querying of "detectionSensitivity" supported, 1: Y, 0: N.                            */
    /* DEFAULT_DETECTION_RANGE, DEFAULT_DETECTION_SENSITIVITY:                                                                                */
    /*                          Default value of detectionRange or detectionSensitivity, depending on the value of var_occupancyCapabilities. */
    /*                          Value range: 0-100%: 0 is the lowest. 100 is the highest. 255: Adjustment not supported.                      */
    var_occupancyCapabilities(index) = 0; //customer define
    DEFAULT_DETECTION_RANGE(index) = MASK_u8; //customer define
    DEFAULT_DETECTION_SENSITIVITY(index) = MASK_u8; //customer define

    /* Depends on the type of movement sensor, it is possible that a very short pulse may be produced only when movement is first detected.   */
    /* After the movement is detected, set and remain in Occupied & Movement state for at least 1 second (REMAIN_PIR_STATE_TIME).             */
    /* DALI library will set a counter to count the REMAIN_PIR_STATE_TIME, and set to Occupied & No Movement state when times up.             */
    /* If not support, set 0.                                                                                                                 */
    REMAIN_PIR_STATE_TIME(index) = 7000; //customer define, ms

    /* Depends on the type of sensor, it is possible that a warm up time may be needed after first power on or after the sensor is triggered. */
    /* Set warm up time in warmupTime303, and use checkWarmUp()to count.                                                                      */
    /* If not support, set 0.                                                                                                                 */
    PIR_WARM_UP_TIME(index) = 30000; //customer define, 30 sec
}

void DefaultInstance303(uint8_t instanceNumber)
{
    uint8_t i;
    int8_t index = calInstanceIndex(instanceNumber);

    if (index < 0)
    {
        while (1); //ERROR: The instance number order is wrong or the sensor amount is larger than define in DALI_CD_APP.h.
    }

    instanceType(instanceNumber) = PART303;
    resolution(instanceNumber) = 2;
    extendedVersionNumber(instanceNumber) = Version(2, 1);
    instanceActive(instanceNumber) = TRUE;

    for (i = 0; i < 3; i++)
    {
        instanceGroup(instanceNumber, i) = MASK_u8;
    }

    eventScheme(instanceNumber) = 0;
    eventPriority(instanceNumber) = 4;
    eventFilter(instanceNumber) = 3;
    instanceError(instanceNumber) = FALSE;
    tDeadtime(instanceNumber) = 2;
    tHold(instanceNumber) = 90;
    tReport(instanceNumber) = 20;
    instanceErrorByte(instanceNumber) = 0;
    catching(instanceNumber) = FALSE;
    var_detectionRange(instanceNumber) = DEFAULT_DETECTION_RANGE(index);
    var_detectionSensitivity(instanceNumber) = DEFAULT_DETECTION_SENSITIVITY(index);
}

void ResetInstance303(uint8_t instanceNumber)
{
    uint8_t i;

    for (i = 0; i < 3; i++)
    {
        instanceGroup(instanceNumber, i) = MASK_u8;
    }

    eventScheme(instanceNumber) = 0;
    eventPriority(instanceNumber) = 4;
    eventFilter(instanceNumber) = 3;
    instanceError(instanceNumber) = FALSE;
    tDeadtime(instanceNumber) = 2;
    tHold(instanceNumber) = 90;
    tReport(instanceNumber) = 20;
    catching(instanceNumber) = FALSE;
}

void PowerOnInstance303(uint8_t instanceNumber)
{
    uint8_t index = instanceNumber - startInstanceNo(instanceTypeIndex(instanceNumber));
    g_parameter303[index].warmupCounter = 0;
    instanceError(instanceNumber) = FALSE;
    instanceErrorByte(instanceNumber) = 0;
    catching(instanceNumber) = FALSE;
    F_sendEvent(instanceNumber) = FALSE;
    F_deadtime(instanceNumber) = TRUE;
    //F_report(instanceNumber) = FALSE;//default setting
    F_repeatTrigger(instanceNumber) = FALSE;
    F_movementTrigger(instanceNumber) = FALSE;
    F_occupiedTrigger(instanceNumber) = FALSE;

    if (checkRepeatEnable(instanceNumber))//check tReport, event filter.
    {
        F_report(instanceNumber) = TRUE;
        cReport(instanceNumber) = 1;
    }
    else
    {
        F_report(instanceNumber) = FALSE;
    }
}

uint8_t checkWarmUp303(uint8_t instanceNumber)
{
    uint8_t index = instanceNumber - startInstanceNo(instanceTypeIndex(instanceNumber));

    if (g_parameter303[index].warmupCounter < PIR_WARM_UP_TIME(index))
    {
        g_parameter303[index].warmupCounter = g_parameter303[index].warmupCounter + 1;
        return 0;
    }
    else
    {
        return 1;
    }
}

uint8_t checkEvent303(uint8_t instanceNumber, uint8_t instanceErrorState)
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

        if (F_PIRWarmUpEnd[instanceNumber - startInstanceNo(instanceTypeIndex(instanceNumber))])
            checkOccupancySensor(instanceNumber, SensorValue[instanceNumber]);

        checkReportTime(instanceNumber);
        state = checkDeadtime(instanceNumber);
    }

    return state;
}

#endif