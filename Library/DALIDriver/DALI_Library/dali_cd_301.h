#ifndef __DALI_CD_301_H__
#define __DALI_CD_301_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include <stdint.h>
#include <stdbool.h>
#include "dali_cd_app.h"

#define RELEASE             0x00
#define PRESS               0x01
#define SHORT               0x02
#define DOUBLE              0x05
#define LONGPRESS_START     0x09
#define LONGPRESS_REPEAT    0x0B
#define LONGPRESS_STOP      0x0C
#define FREE                0x0E
#define STUCK               0x0F

#define S_ABSENCE           0x01
#define S_PRESS_DBCK        0x02
#define S_PRESS             0x04
#define S_HOLD              0x08
#define S_RELEASE_DBCK      0x10
#define S_RELEASE           0x20

typedef struct
{
    uint8_t buttonStatus[AMOUNT_OF_301];
    uint8_t tincr_sample;
    uint8_t sw_amount_of_301;
} parameter301_t;

#define SW_AMOUNT_OF_301                   g_parameter301.sw_amount_of_301
#define S_buttonStatus(x)                  g_parameter301.buttonStatus[x]
#define Tincr_sample                       g_parameter301.tincr_sample

/*Variable*/
#define tShort(x)                          instanceNo[x].NVM.info.instanceNVM[0]
#define tDouble(x)                         instanceNo[x].NVM.info.instanceNVM[1]
#define tRepeat(x)                         instanceNo[x].NVM.info.instanceNVM[2]
#define tStuck(x)                          instanceNo[x].NVM.info.instanceNVM[3]
#define tShortMin(x)                       instanceNo[x].NVM.info.instanceNVM[4]
#define tDoubleMin(x)                      instanceNo[x].NVM.info.instanceNVM[5]
#define old_amount_301(x)                  instanceNo[x].NVM.info.instanceNVM[6]

#define cShortPress(x)                     instanceNo[x].var.eventCounter[0]
#define cDoublePress(x)                    instanceNo[x].var.eventCounter[1]
#define cRepeat(x)                         instanceNo[x].var.eventCounter[2]
#define cStuck(x)                          instanceNo[x].var.eventCounter[3]

#define isButtonReleasedEventEnabled(x)    ((instanceNo[x].NVM.info.eventFilter >> 0) & BIT0)
#define isButtonPressedEventEnabled(x)     ((instanceNo[x].NVM.info.eventFilter >> 1) & BIT0)
#define isShortPressEventEnabled(x)        ((instanceNo[x].NVM.info.eventFilter >> 2) & BIT0)
#define isDoublePressEventEnabled(x)       ((instanceNo[x].NVM.info.eventFilter >> 3) & BIT0)
#define isLongPressStartEventEnabled(x)    ((instanceNo[x].NVM.info.eventFilter >> 4) & BIT0)
#define isLongPressRepeatEventEnabled(x)   ((instanceNo[x].NVM.info.eventFilter >> 5) & BIT0)
#define isLongPressStopEventEnabled(x)     ((instanceNo[x].NVM.info.eventFilter >> 6) & BIT0)
#define isButtonStuckEventEnabled(x)       ((instanceNo[x].NVM.info.eventFilter >> 7) & BIT0)

#define isButtonStuck(x)                   ((instanceNo[x].instanceErrorByte >> 0) & BIT0)
#define isManufacturerSpecificError1(x)    ((instanceNo[x].instanceErrorByte >> 4) & BIT0)
#define isManufacturerSpecificError2(x)    ((instanceNo[x].instanceErrorByte >> 5) & BIT0)
#define isManufacturerSpecificError3(x)    ((instanceNo[x].instanceErrorByte >> 6) & BIT0)
#define isManufacturerSpecificError4(x)    ((instanceNo[x].instanceErrorByte >> 7) & BIT0)

uint8_t buttonValue(uint8_t);

void DefaultInstance301(uint8_t);
void ResetInstance301(uint8_t);
void PowerOnInstance301(uint8_t);

void initInstanceInfo301(uint8_t, uint8_t, uint8_t);
uint8_t instanceAdditional301(uint8_t, uint8_t, uint8_t, uint8_t);
void checkResetState301(uint8_t);
bool checkEventFilter301(uint8_t);
bool checkEventFilterMaximum301(uint8_t);
uint8_t checkEventFilterLimit301(uint8_t, uint8_t);
uint32_t setInputValue301(uint8_t);
uint8_t checkEvent301(uint8_t);
void checkButton(uint8_t);
void checkShortTime(uint8_t);
void checkDoubleTime(uint8_t);
void checkRepeatTime(uint8_t);
void checkStuckTime(uint8_t);
uint8_t checkButtonStatus(uint8_t);

#ifdef __cplusplus
}
#endif

#endif //__DALI_CD_301_H__
