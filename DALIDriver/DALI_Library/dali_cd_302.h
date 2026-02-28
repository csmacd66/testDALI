#ifndef __DALI_CD_302_H__
#define __DALI_CD_302_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include <stdint.h>
#include <stdbool.h>
#include "dali_cd_app.h"

#define SWITCH                             0
#define SLIDER                             1

typedef struct
{
    uint32_t dataMax;
    uint32_t resoMax;
} parameter302_t;

/*Variable*/
#define dataMax(x)                         g_parameter302[x].dataMax
#define resoMax(x)                         g_parameter302[x].resoMax

#define switchType(x)                      instanceNo[x].NVM.info.instanceNVM[2]
#define isPositionEventEnabled(x)          ((instanceNo[x].NVM.info.eventFilter >> 0) & BIT0)

#define isManufacturerSpecificError1(x)    ((instanceNo[x].instanceErrorByte >> 4) & BIT0)
#define isManufacturerSpecificError2(x)    ((instanceNo[x].instanceErrorByte >> 5) & BIT0)
#define isManufacturerSpecificError3(x)    ((instanceNo[x].instanceErrorByte >> 6) & BIT0)
#define isManufacturerSpecificError4(x)    ((instanceNo[x].instanceErrorByte >> 7) & BIT0)

void parameter302(uint8_t);
void DefaultInstance302(uint8_t);
void ResetInstance302(uint8_t);
void PowerOnInstance302(uint8_t);

void initInstanceInfo302(uint8_t, uint8_t, uint8_t);
uint8_t instanceAdditional302(uint8_t, uint8_t, uint8_t, uint8_t);
void checkResetState302(uint8_t);
uint8_t checkHysteresisMinDefault(uint8_t);
uint8_t checkEventFilter302(uint8_t);
uint8_t checkEventFilterMaximum302(uint8_t);
uint8_t checkEventFilterLimit302(uint8_t, uint8_t);
uint32_t setInputValue302(uint8_t);
uint16_t setEvent302(uint8_t, uint32_t);
uint8_t checkEvent302(uint8_t);
void checkSwitch(uint8_t);

#ifdef __cplusplus
}
#endif

#endif //__DALI_CD_302_H__