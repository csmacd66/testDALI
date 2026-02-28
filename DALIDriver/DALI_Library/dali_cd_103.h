#ifndef __DALI_CD_103_H__
#define __DALI_CD_103_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include <stdint.h>
#include <stdbool.h>
#include "dali_cd_app.h"

#define MASK_u8                                   0xFF
#define MASK_u16                                  0xFFFF
#define MASK_u24                                  0xFFFFFF
#define MASK_u32                                  0xFFFFFFFF

/*sending or receiving status*/
#define IDLE                                      0
#define RECEIVING                                 1
#define SENDING                                   2
#define ACKING                                    3

#define WITHDRAWN                                 2

#define CMD                                       1

#if CHECK_COLLISION
#define INIT_STATE    0
#define DETECT_STATE  1
#define BREAK_STATE   2
#endif

typedef struct
{
    bool f_checkSendTwice: 1;
    bool f_identification: 1;
    bool f_newPowerCycleNotice: 1;
    bool f_sendPowerCycleNotification: 1;
    bool f_pollingInstance: 1;
    bool f_receiveFinish: 1;
    bool f_writeEnableState: 1;
    bool f_manuCmdMode: 1;
    bool f_applicationEnable: 1;
    bool f_checkCollision: 1;
    bool f_checkAnsCollision: 1;
    uint8_t reserved: 5;
} flag_t;

typedef union
{
    uint8_t _status;
    struct
    {
        bool inputDeviceError: 1;
        bool quiescentMode: 1;
        bool shortAddress: 1;
        bool applicationActive: 1;
        bool applicationControllerError: 1;
        bool powerCycleSeen: 1;
        bool resetState: 1;
        bool reserved: 1;
    } info;
} status_t;

/*save in Flash*/
#define Nvariable 12
typedef union
{
    uint8_t var[Nvariable];//3*4
    struct
    {
        //Device variable
        uint32_t deviceGroups;//1
        uint32_t randomAddress;//2
        uint8_t shortAddress;
        uint8_t operatingMode;
        uint8_t applicationActive;
        uint8_t powerCycleNotification;//3
    } info;
} var_t;

typedef struct
{
    var_t var;//variables will be written to flash
    flag_t flag;

    status_t status;
    uint8_t address, instance, opcode, ack;
    uint8_t twice, twice_cmd[3], sendTwiceCounter, powerEventTime;
    uint32_t backTime, maxSignalTime, minSignalTime, stopTime, highTime, lowTime, typTime, stopCondition;
    uint16_t sendTwiceTime, identificationTime, idStepTime, newPowerOnTime[2], newPowerOnCounter;
    uint32_t initTime, idCounter, initCounter, quiescentmodeCounter, idStepCounter;
#if CHECK_COLLISION
    uint32_t breakTime, minGreyArea, maxGreyArea;
#endif
    uint8_t send_rcv;/*sending or receiving*/
    uint8_t numberOfInstances, numberOfFeatures, totalBankNumber, totalBankSize, totalNVMSize;
    uint8_t precededCmd[3];
    uint8_t setDTxInit, setDRxInit;
    uint8_t codeDelay;
    uint32_t seed;

    //Device variable
    uint32_t searchAddress;
    uint8_t dtr[3];
    uint8_t initialisationState;
    uint8_t f_statusCollision;
    uint8_t applicationEnable;

    //instance variable
    uint8_t eventSourceH, eventSourceL;
    uint16_t eventInfo;
    uint8_t inputLatchStep;
    uint32_t inputLatchValue;
} parameter_t;

typedef struct
{
    uint8_t f_send;
    uint8_t  f_repeatCMD;
    uint8_t f_sendFrame;
    uint8_t f_sendUnFinish;
    uint32_t send_data;
    uint32_t settling_counter;
    uint32_t settling_time;
    uint32_t send_setting;
} transmitPara_t;

/*Status*/
#define sta_allStatus                                   g_parameter.status._status
#define sta_inputDeviceError                            g_parameter.status.info.inputDeviceError
#define sta_quiescentMode                               g_parameter.status.info.quiescentMode
#define sta_shortAddress                                g_parameter.status.info.shortAddress
#define sta_applicationActive                           g_parameter.status.info.applicationActive
#define sta_applicationControllerError                  g_parameter.status.info.applicationControllerError
#define sta_powerCycleSeen                              g_parameter.status.info.powerCycleSeen
#define sta_resetState                                  g_parameter.status.info.resetState

/*Variable*/
#define var_deviceGroups                                g_parameter.var.info.deviceGroups
#define var_randomAddress                               g_parameter.var.info.randomAddress
#define var_shortAddress                                g_parameter.var.info.shortAddress
#define var_operatingMode                               g_parameter.var.info.operatingMode
#define var_applicationActive                           g_parameter.var.info.applicationActive
#define var_powerCycleNotification                      g_parameter.var.info.powerCycleNotification

#define var_seed                                        g_parameter.seed
#define ADDRESS                                         g_parameter.address
#define INSTANCE                                        g_parameter.instance
#define OPCODE                                          g_parameter.opcode
#define ACK                                             g_parameter.ack
#define BACK_TIME                                       g_parameter.backTime
#define MAX_SIGNAL_TIME                                 g_parameter.maxSignalTime
#define MIN_SIGNAL_TIME                                 g_parameter.minSignalTime
#define STOP_TIME                                       g_parameter.stopTime
#define HIGH_TIME                                       g_parameter.highTime
#define LOW_TIME                                        g_parameter.lowTime
#define TYP_TIME                                        g_parameter.typTime
#define STOP_CONDITION                                  g_parameter.stopCondition
#define INIT_TIME                                       g_parameter.initTime
#define SEND_TWICE_TIME                                 g_parameter.sendTwiceTime
#define IDENTIFICATION_TIME                             g_parameter.identificationTime
#define ID_STEP_TIME                                    g_parameter.idStepTime
#define NEW_POWER_ON_TIME                               g_parameter.newPowerOnTime
#define TOTAL_BANK_NUMBER                               g_parameter.totalBankNumber
#define TOTAL_BANK_SIZE                                 g_parameter.totalBankSize
#define TOTAL_NVM_SIZE                                  g_parameter.totalNVMSize
#define SET_DTX_INIT                                    g_parameter.setDTxInit
#define SET_DRX_INIT                                    g_parameter.setDRxInit

#define var_powerEventTime                              g_parameter.powerEventTime
#define var_searchAddress                               g_parameter.searchAddress
#define var_DTR0                                        g_parameter.dtr[0]
#define var_DTR1                                        g_parameter.dtr[1]
#define var_DTR2                                        g_parameter.dtr[2]
#define var_initialisationState                         g_parameter.initialisationState
#define F_statusCollision                               g_parameter.f_statusCollision
#define var_eventSourceH                                g_parameter.eventSourceH
#define var_eventSourceL                                g_parameter.eventSourceL
#define var_eventInfo                                   g_parameter.eventInfo

#define F_checkSendTwice                                g_parameter.flag.f_checkSendTwice
#define F_identification                                g_parameter.flag.f_identification
#define F_newPowerCycleNotice                           g_parameter.flag.f_newPowerCycleNotice
#define F_sendPowerCycleNotification                    g_parameter.flag.f_sendPowerCycleNotification
#define F_pollingInstance                               g_parameter.flag.f_pollingInstance
#define F_receiveFinish                                 g_parameter.flag.f_receiveFinish
#define F_writeEnableState                              g_parameter.flag.f_writeEnableState
#define F_manuCmdMode                                   g_parameter.flag.f_manuCmdMode
#define F_checkCollision                                g_parameter.flag.f_checkCollision
#define F_checkAnsCollision                             g_parameter.flag.f_checkAnsCollision

#define F_send(x)                                       transmitPara[x].f_send//0: response, 1: command, 2: event
#define F_repeatCMD(x)                                  transmitPara[x].f_repeatCMD
#define F_sendFrame(x)                                  transmitPara[x].f_sendFrame
#define F_sendUnFinish(x)                               transmitPara[x].f_sendUnFinish
#define sendData(x)                                     transmitPara[x].send_data
#define settlingCounter(x)                              transmitPara[x].settling_counter
#define settlingTime(x)                                 transmitPara[x].settling_time
#define sendSetting(x)                                  transmitPara[x].send_setting

#define Version(x,y)                                    (x << 2 | y)
#define isCommand(x)                                    (x & BIT0)
#define Debug_While(x)                                  while(x)

#if CHECK_COLLISION
#define BREAK_TIME                                      g_parameter.breakTime
#define MIN_GREY_AREA                                   g_parameter.minGreyArea
#define MAX_GREY_AREA                                   g_parameter.maxGreyArea
#endif

extern volatile parameter_t g_parameter;

void DALI_Init(void);
void transmitParameter(void);
void storageParameter(void);
void instanceParameter(void);
void DefaultState(void);
void ResetState(void);
void PowerOnState(void);
void DefaultDevice(void);
void ResetDevice(void);
void PowerOnDevice(void);
void DefaultInstance103(uint8_t);
void ResetInstance103(uint8_t);
void PowerOnInstance103(uint8_t);

void setVECTORtoSRAM(void);
void startReceive(void);
void checkReceiveFinish(void);
uint32_t ParseCapData(void);
uint8_t Command(uint8_t, uint8_t, uint8_t);
void specialCommand(uint8_t, uint8_t, uint8_t);
void deviceCommand(uint8_t, uint8_t, uint8_t);
void sendPowerCycleNotification(void);
void stopIdentification(void);
void checkResetState(void);
void checkResetState103(uint8_t);
void checkShortAddress(void);
uint8_t checkSendTwice(uint8_t, uint8_t, uint8_t);
void cleanSendTwiceCmd(uint8_t, uint8_t, uint8_t);
void clearprecededCommand(void);
uint32_t setSettlingTime(uint8_t);
void setSendSetting(uint8_t, uint8_t, uint8_t, uint8_t, bool);
void sendFrame(uint8_t);
uint32_t clamp(uint32_t, uint32_t, uint32_t);
void clearSendFlag(uint8_t);
void checkDeviceResetState(void);
void checkCollision(void);
uint8_t Event(uint8_t, uint8_t, uint8_t);
void getRandomAddress(void);

void manufacturerMode(uint8_t, uint8_t, uint8_t);
uint8_t checkSupportedOperatingMode(uint8_t);

#ifdef __cplusplus
}
#endif

#endif //__DALI_CD_103_H__

