#ifndef __DALI_CD_303_H__
#define __DALI_CD_303_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include <stdint.h>
#include <stdbool.h>
#include "NuMicro.h"
#include "dali_cd_instance.h"

/*Part 303 event*/
#define NO_MOVEMENT                      0
#define MOVEMENT                         1
#define VACANT                           0
#define OCCUPIED                         1
#define PRESENCE_SENSOR                  0
#define MOVEMENT_SENSOR                  1
/*Part 303 inputValue*/
#define VACANT_NOMOVEMENT               0x00
#define VACANT_MOVEMENT                 0x55
#define OCCUPIED_NOMOVEMENT             0xAA
#define OCCUPIED_MOVEMENT               0xFF

typedef struct
{
    uint8_t occupancyCapabilities;
    uint8_t defaultDetectionRange;
    uint8_t defaultDetectionSensitivity;
    uint32_t warmupTime;
    uint32_t warmupCounter;
    uint16_t remainPIRStateTime;
} parameter303_t;

#define var_occupancyCapabilities(x)     g_parameter303[x].occupancyCapabilities
#define DEFAULT_DETECTION_RANGE(x)       g_parameter303[x].defaultDetectionRange
#define DEFAULT_DETECTION_SENSITIVITY(x) g_parameter303[x].defaultDetectionSensitivity
#define PIR_WARM_UP_TIME(x)              g_parameter303[x].warmupTime
#define REMAIN_PIR_STATE_TIME(x)         g_parameter303[x].remainPIRStateTime

/*Variable*/
#define tHold(x)                         instanceNo[x].NVM.info.instanceNVM[2]
#define var_detectionRange(x)            instanceNo[x].NVM.info.instanceNVM[3]
#define var_detectionSensitivity(x)      instanceNo[x].NVM.info.instanceNVM[4]
#define SENSOR_TYPE(x)                   instanceNo[x].NVM.info.instanceNVM[5]

#define cHold(x)                         instanceNo[x].var.eventCounter[2]
#define cMovementTrigger(x)              instanceNo[x].var.eventCounter[3]

#define F_hold(x)                        instanceNo[x].flag.info.flag4
#define catching(x)                      instanceNo[x].flag.info.flag5

#define F_movementTrigger(x)             instanceNo[x].selfDefinedflag.info.flag0 // 0:noMovement, 1:movemenet
#define F_occupiedTrigger(x)             instanceNo[x].selfDefinedflag.info.flag1 // 0:vacant, 1:occupied

#define isOccupiedEventEnabled(x)        ((instanceNo[x].NVM.info.eventFilter >> 0) & BIT0)
#define isVacantEventEnabled(x)          ((instanceNo[x].NVM.info.eventFilter >> 1) & BIT0)
#define isRepeatEventEnabled(x)          ((instanceNo[x].NVM.info.eventFilter >> 2) & BIT0)
#define isMovementEventEnabled(x)        ((instanceNo[x].NVM.info.eventFilter >> 3) & BIT0)
#define isNoMovementEventEnabled(x)      ((instanceNo[x].NVM.info.eventFilter >> 4) & BIT0)

#define isPhysicalSensorFailure(x)       ((instanceNo[x].instanceErrorByte >> 0) & BIT0)
#define isManufacturerSpecificError1(x)  ((instanceNo[x].instanceErrorByte >> 4) & BIT0)
#define isManufacturerSpecificError2(x)  ((instanceNo[x].instanceErrorByte >> 5) & BIT0)
#define isManufacturerSpecificError3(x)  ((instanceNo[x].instanceErrorByte >> 6) & BIT0)
#define isManufacturerSpecificError4(x)  ((instanceNo[x].instanceErrorByte >> 7) & BIT0)

#define ifSendOccupiedEvent(x)           (F_occupiedTrigger(x) == FALSE && isOccupiedEventEnabled(x))
#define ifSendVacantEvent(x)             (F_occupiedTrigger(x) == TRUE && isVacantEventEnabled(x))
#define ifSendMovementEvent(x)           (F_movementTrigger(x) == FALSE && isMovementEventEnabled(x))
#define ifSendNoMovementEvent(x)         (F_movementTrigger(x) == TRUE && isNoMovementEventEnabled(x))

void parameter303(uint8_t);
void DefaultInstance303(uint8_t);
void ResetInstance303(uint8_t);
void PowerOnInstance303(uint8_t);
uint8_t checkWarmUp303(uint8_t);

void initInstanceInfo303(uint8_t, uint8_t, uint8_t);
void setHold303(uint8_t);
uint8_t instanceAdditional303(uint8_t, uint8_t, uint8_t, uint8_t);
void checkResetState303(uint8_t);
uint32_t setInputValue303(uint8_t);
uint16_t setEvent303(uint8_t, uint32_t);
bool checkEventFilter303(uint8_t);
bool checkEventFilterMaximum303(uint8_t);
uint8_t checkEventFilterLimit303(uint8_t, uint8_t);
void OccupancySensorTrigger(uint8_t, uint8_t, uint8_t);
void checkHoldTime(uint8_t);
void checkOccupancySensor(uint8_t, uint32_t);
uint8_t checkEvent303(uint8_t, uint8_t);

#ifdef __cplusplus
}
#endif

#endif //__DALI_CD_303_H__
