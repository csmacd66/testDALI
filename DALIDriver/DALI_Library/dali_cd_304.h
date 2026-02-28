#ifndef __DALI_CD_304_H__
#define __DALI_CD_304_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include <stdint.h>
#include <stdbool.h>
#include "NuMicro.h"
#include "dali_cd_instance.h"

typedef struct
{
    uint32_t hysteresisMinDefault;
    uint32_t hysteresisBand;
    uint32_t hysteresisBandHigh;
    uint32_t hysteresisBandLow;
} parameter304_t;

/*Variable*/
#define var_hysteresisMinDefault(x)        g_parameter304[x].hysteresisMinDefault
#define var_hysteresisBand(x)              g_parameter304[x].hysteresisBand
#define var_hysteresisBandHigh(x)          g_parameter304[x].hysteresisBandHigh
#define var_hysteresisBandLow(x)           g_parameter304[x].hysteresisBandLow

#define hysteresisMin(x)                   instanceNo[x].NVM.info.instanceNVM[2]
#define hysteresis(x)                      instanceNo[x].NVM.info.instanceNVM[3]

#define isIlluminanceLevelEventEnabled(x)  ((instanceNo[x].NVM.info.eventFilter >> 0) & BIT0)

#define F_outsideHysteresis(x)             instanceNo[x].flag.info.flag4

#define isPhysicalSensorFailure(x)         ((instanceNo[x].instanceErrorByte >> 0) & BIT0)
#define isManufacturerSpecificError1(x)    ((instanceNo[x].instanceErrorByte >> 4) & BIT0)
#define isManufacturerSpecificError2(x)    ((instanceNo[x].instanceErrorByte >> 5) & BIT0)
#define isManufacturerSpecificError3(x)    ((instanceNo[x].instanceErrorByte >> 6) & BIT0)
#define isManufacturerSpecificError4(x)    ((instanceNo[x].instanceErrorByte >> 7) & BIT0)

void parameter304(uint8_t);
void DefaultInstance304(uint8_t);
void ResetInstance304(uint8_t);
void PowerOnInstance304(uint8_t);

void initInstanceInfo304(uint8_t, uint8_t, uint8_t);
uint8_t instanceAdditional304(uint8_t, uint8_t, uint8_t, uint8_t);
void checkResetState304(uint8_t);
uint8_t checkHysteresisMinDefault(uint8_t);
uint8_t checkEventFilter304(uint8_t);
uint8_t checkEventFilterMaximum304(uint8_t);
uint8_t checkEventFilterLimit304(uint8_t, uint8_t);
uint32_t setInputValue304(uint8_t);
uint16_t setEvent304(uint8_t, uint32_t);
void calHysteresisBand(uint8_t, uint32_t);
void checkLightSensor(uint8_t);
uint8_t checkEvent304(uint8_t, uint8_t);

#ifdef __cplusplus
}
#endif

#endif //__DALI_CD_304_H__
