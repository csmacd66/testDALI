#ifndef __DALI_CD_APP_H__
#define __DALI_CD_APP_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>
#include "NuMicro.h"

#define TEST_VERSION                          2700

//Set ENABLE to enable Application Controller function. Set DISABLE to disable Application Controller function.
#define APPLICATION_CONTROLLER_PRESENT        (DISABLE)

#define GTIN_ADDR                             0xFFE0
#define SN_ADDR                               0xFFF0

#define USE_PART351                           (DISABLE)
#define SELF_ADD_BANK                         (0)    //Enabled Memory bank 2 ~ bank 199 //customer define

#if(USE_PART351)
#define LAST_BANK_NO                          (201)
#elif(SELF_ADD_BANK)
#define LAST_BANK_NO                          (2) //Enabled Memory bank 2 ~ bank 199 //customer define
#else
#define LAST_BANK_NO                          (1)
#endif
#define NUMBER_OF_BANKS                       (2 + USE_PART351 + SELF_ADD_BANK) //how many banks used in 103 //customer define

#define USE_PART103_INPUT_DEVICE              DISABLE //for devices which is undefined in IEC 62386 Part 3XX

#define USE_PART301                           DISABLE
#define AMOUNT_OF_301                         (1 * USE_PART301)

#define USE_PART302                           ENABLE
#define AMOUNT_OF_302_switch                  (0 * USE_PART302)
#define AMOUNT_OF_302_slider                  (1 * USE_PART302)
#define AMOUNT_OF_302_rotary                  (0 * USE_PART302) 
#define AMOUNT_OF_302                         AMOUNT_OF_302_switch + AMOUNT_OF_302_slider + AMOUNT_OF_302_rotary

#define USE_PART303                           DISABLE
#define AMOUNT_OF_303                         (1 * USE_PART303)

#define USE_PART304                           DISABLE
#define AMOUNT_OF_304                         (1 * USE_PART304)

#define TYPES_OF_INSTANCES                    USE_PART301 + USE_PART302 + USE_PART303 + USE_PART304 // how many types of instance in the device
#define NUMBER_OF_INSTANCES                   AMOUNT_OF_301 + AMOUNT_OF_302 + AMOUNT_OF_303 + AMOUNT_OF_304

//Customer should check TOTAL_NVM_TYPE value if the amount of supported bank or device type changed
#define TOTAL_NVM_TYPE                        1 + NUMBER_OF_INSTANCES

//Set the numbers of feature
#define TYPES_OF_FEATURES                     0
#define NUMBER_OF_FEATURES                    0

#define CODE_DELAY_CONPENSATION_FOR_DEAD_TIME 35//0~49 The deadtime delay will accumulate //modify for 304 5.3
#define HCLK_DIV                              (2) //48 MHz /HCLK_DIV
#define CHECK_COLLISION                       (ENABLE)
#define CHECK_DALI_BUS                        (ENABLE)
#define CHECK_ANS_COLLISION                   (ENABLE)
#define RANDADDR_GENERATE                     (ENABLE)

#define MANU_CMD_MODE                         (0)
//MANU_CMD_MODE setting:
//1: support all cmds defined in protocol and self-defined cmds in manufacturerMode() under manufacturer specific modes
//0: support only special cmds (except bank write cmds), and cmds in manufacturerMode() under all manufacturer specific modes.


typedef uint8_t (*instanceCommandPtr)(uint8_t, uint8_t, uint8_t, uint8_t);
typedef void (*checkResetStatePtr)(uint8_t);

#include "board_init.h"
#include "dali_cd_103.h"
#include "dali_cd_instance.h"
#include "dali_cd_bank.h"
#include "dali_fmc.h"
#include "critical_isr.h"

#if(RANDADDR_GENERATE)
void seed_gen_ADC(void);
#endif

extern volatile uint32_t SensorValue[NUMBER_OF_INSTANCES] ; //The customer needs to enter the input value of the sensor
extern volatile uint8_t NVM_SIZE[TOTAL_NVM_TYPE + NUMBER_OF_BANKS];
extern volatile uint8_t *NVM_ARRAY[TOTAL_NVM_TYPE + NUMBER_OF_BANKS];
extern volatile transmitPara_t transmitPara[NUMBER_OF_INSTANCES + 2];

#if(NUMBER_OF_INSTANCES)
extern uint8_t INSTANCE_TYPE[TYPES_OF_INSTANCES];
extern volatile instance_t instanceNo[NUMBER_OF_INSTANCES];
extern volatile instanceInfo_t instanceInfo[TYPES_OF_INSTANCES];
extern volatile uint32_t SensorValue[NUMBER_OF_INSTANCES];

#if(USE_PART103_INPUT_DEVICE)
#define PART103                               0
#endif

#if(USE_PART301)
#define PART301 1
#include "dali_cd_301.h"
extern volatile parameter301_t g_parameter301;
#endif

#if(USE_PART302)
#define PART302 2
#include "dali_cd_302.h"
extern volatile parameter302_t g_parameter302[AMOUNT_OF_302];
#endif

#if(USE_PART303)
#define PART303 3
#include "dali_cd_303.h"
extern volatile parameter303_t g_parameter303[AMOUNT_OF_303];
#endif

#if(USE_PART304)
#define PART304 4
#include "dali_cd_304.h"
extern volatile parameter304_t g_parameter304[AMOUNT_OF_304];
#endif

#else
extern volatile instance_t instanceNo[1];
extern volatile instanceInfo_t instanceInfo[1];
#endif
extern volatile feature_t featureNo[1];

#ifdef __cplusplus
}
#endif

#endif //__DALI_CD_APP_H__
