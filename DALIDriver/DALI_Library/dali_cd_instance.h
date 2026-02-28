#ifndef __DALI_CD_INSTANCE_H__
#define __DALI_CD_INSTANCE_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include <stdint.h>
#include <stdbool.h>
#include "dali_cd_app.h"

typedef union
{
    uint8_t _eventSetting;
    struct
    {
        uint8_t eventScheme: 3;//0~4
        uint8_t eventPriority: 3;//2~5
        uint8_t reserved0: 2;
    } info;
} eventSetting_t;

#define Ivariable 20

typedef union
{
    uint8_t instanceNVM[Ivariable];
    struct
    {
        uint8_t instanceType;
        uint8_t resolution;
        uint8_t extendedVersionNumber;
        uint8_t instanceActive; //TRUE/FALSE //1
        uint8_t instanceGroup[3];
        eventSetting_t eventSetting;//2
        uint32_t eventFilter; //3
        uint8_t instanceNVM[8];//5
        //---------- 301 -----------
        //instanceNVM[0] = tShort
        //instanceNVM[1] = tDouble
        //instanceNVM[2] = tRepeat
        //instanceNVM[3] = tStuck
        //instanceNVM[4] = tShortMin;
        //instanceNVM[5] = tDoubleMin;
        //instanceNVM[6~7] = Reserve
        //---------- 302 -----------
        //instanceNVM[0] = tReport
        //instanceNVM[1] = tDeadtime
        //instanceNVM[2] = switchType
        //instanceNVM[3] = Reserve
        //instanceNVM[4~7] = Reserve
        //---------- 303 -----------
        //instanceNVM[0] = tReport
        //instanceNVM[1] = tDeadtime
        //instanceNVM[2] = tHold
        //instanceNVM[3] = detectionRange
        //instanceNVM[4] = detectionSensitivity
        //instanceNVM[5] = sensorType
        //instanceNVM[6~7] = Reserve
        //---------- 304 -----------
        //instanceNVM[0] = tReport
        //instanceNVM[1] = tDeadtime
        //instanceNVM[2] = hysteresisMin
        //instanceNVM[3] = hysteresis
        //instanceNVM[4~7] = Reserve
    } info;
} instanceNVM_t;

typedef union
{
    uint8_t _flag;
    struct
    {
        bool flag0;
        bool flag1;
        bool flag2;
        bool flag3;
        bool flag4;
        bool flag5;
        bool flag6;
        bool flag7;
    } info;
} instanceFlag_t;

//for each instance
typedef struct
{
    uint32_t eventCounter[4];
} instanceParameter_t;


//for each instance
typedef struct
{
    instanceNVM_t NVM;
    uint8_t instanceTypeIndex;
    uint8_t instanceError;
    uint8_t instanceErrorByte;
    uint32_t inputValue;
    uint32_t inputValueMask;
    uint16_t event;
    instanceParameter_t var;
    instanceFlag_t flag;
    instanceFlag_t selfDefinedflag;
} instance_t;

//for each instance type
typedef struct
{
    uint8_t instanceType;
    uint8_t amountType;
    uint8_t startInstanceNo;
    uint32_t tincr_deadTime;
    uint32_t tincr_report;
    instanceCommandPtr instanceCommand;
    checkResetStatePtr checkResetStateInstance;
} instanceInfo_t;

typedef struct
{
    uint8_t featureType;
} feature_t;

#define instanceTypeIndex(x)             instanceNo[x].instanceTypeIndex
#define instanceError(x)                 instanceNo[x].instanceError
#define instanceErrorByte(x)             instanceNo[x].instanceErrorByte
#define inputValue(x)                    instanceNo[x].inputValue
#define inputValueMask(x)                instanceNo[x].inputValueMask
#define event(x)                         instanceNo[x].event

#define instanceType(x)                  instanceNo[x].NVM.info.instanceType
#define resolution(x)                    instanceNo[x].NVM.info.resolution
#define extendedVersionNumber(x)         instanceNo[x].NVM.info.extendedVersionNumber
#define instanceActive(x)                instanceNo[x].NVM.info.instanceActive
#define instanceGroup(x,y)               instanceNo[x].NVM.info.instanceGroup[y]
#define eventScheme(x)                   instanceNo[x].NVM.info.eventSetting.info.eventScheme
#define eventPriority(x)                 instanceNo[x].NVM.info.eventSetting.info.eventPriority
#define eventFilter(x)                   instanceNo[x].NVM.info.eventFilter
#define eventSetting(x)                  instanceNo[x].NVM.info.eventSetting

#define tDeadtime(x)                     instanceNo[x].NVM.info.instanceNVM[0]
#define tReport(x)                       instanceNo[x].NVM.info.instanceNVM[1]

#define cDeadtime(x)                     instanceNo[x].var.eventCounter[0]
#define cReport(x)                       instanceNo[x].var.eventCounter[1]

#define F_sendEvent(x)                   instanceNo[x].flag.info.flag0
#define F_deadtime(x)                    instanceNo[x].flag.info.flag1
#define F_report(x)                      instanceNo[x].flag.info.flag2
#define F_repeatTrigger(x)               instanceNo[x].flag.info.flag3

#define instanceTypeId(x)                instanceInfo[x].instanceType
#define Tincr_deadTime(x)                instanceInfo[x].tincr_deadTime
#define Tincr_report(x)                  instanceInfo[x].tincr_report
#define amountType(x)                    instanceInfo[x].amountType
#define startInstanceNo(x)               instanceInfo[x].startInstanceNo
#define instanceCommand(x)               instanceInfo[x].instanceCommand
#define checkResetStateInstance(x)       instanceInfo[x].checkResetStateInstance

#define featureType(x)                   featureNo[x].featureType

//Instance API
uint8_t Instance(uint8_t, uint8_t, uint8_t);
int8_t calInstanceIndex(uint8_t);
uint8_t checkInstance(void);
uint8_t checkInstanceCommand(uint8_t, uint8_t, uint8_t);
uint8_t instanceStandard(uint8_t, uint8_t, uint8_t);
uint32_t setDeadtime(uint8_t);
uint32_t setReport(uint8_t);
uint8_t checkDeadtime(uint8_t);
void checkReportTime(uint8_t);
void closeReportTime(uint8_t);
uint8_t sendInputNotification(uint8_t, uint16_t);
//void instanceAdditional(uint8_t, uint8_t, uint8_t, uint8_t);
void setInstanceError(uint8_t);
void clearInstanceError(uint8_t);
uint32_t setInputValue(uint8_t);
uint16_t setEvent(uint8_t, uint32_t);
void initInstanceInfo(uint8_t, uint8_t, uint8_t);
void initInstanceParameter(uint8_t, uint8_t);
void initDefaultInstance(uint8_t, uint8_t);
void initResetInstance(uint8_t);
void initPowerOnInstance(uint8_t);
bool checkEventFilter(uint8_t);
uint8_t checkRepeatEnable(uint8_t);
uint8_t checkEventFilterLimit(uint8_t, uint8_t);
uint8_t checkEventFilterMaximum(uint8_t, uint8_t);
uint8_t checkWarmUp(uint8_t);
uint8_t checkAnswerCollision(uint8_t);
//Feature API
uint8_t Feature(uint8_t, uint8_t, uint8_t);
uint8_t checkFeatureCommand(uint8_t, uint8_t, uint8_t);
void featureCommand(uint8_t, uint8_t, uint8_t);
#ifdef __cplusplus
}
#endif

#endif //__DALI_CD_INSTANCE_H__

