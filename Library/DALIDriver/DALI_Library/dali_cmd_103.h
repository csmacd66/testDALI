#ifndef __DALI_CMD_103_H__
#define __DALI_CMD_103_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include <stdint.h>
#include <stdbool.h>
#define BROADCAST                                 0xFF
#define BROADCAST_UNADDRESSED                     0xFD
#define INSTANCE_NUMBER(x)                        (x)
#define INSTANCE_GROUP(x)                         ((1 << 7) | x)
#define INSTANCE_TYPE(x)                          ((3 << 6) | x)
#define DEVICE                                    0xFE

#define FEATURE_BROADCAST                         0xFD
#define FEATURE_NUMBER(x)                         ((1 << 5) | x)
#define FEATURE_GROUP(x)                          ((5 << 5) | x)
#define FEATURE_TYPE(x)                           ((3 << 5) | x)
#define FEATURE                                   0xFC

/*Standard Command*/
/*Device control instruction*/
#define IDENTIFY_DEVICE                           0x00
#define RESET_POWER_CYCLE_SEEN                    0x01
//Reserve                                         0x02~0x09

/*Device configuration instruction*/
#define RESET                                     0x10
#define RESET_MEMORY_BANK                         0x11
//Reserve                                         0x12~0x13
#define SET_SHORT_ADDRESS                         0x14
#define ENABLE_WRITE_MEMORY                       0x15
#define ENABLE_APPLICATION_CONTROLLER             0x16
#define DISABLE_APPLICATION_CONTROLLER            0x17
#define SET_OPERATING_MODE                        0x18
#define ADD_TO_DEVICE_GROUPS_0_15                 0x19
#define ADD_TO_DEVICE_GROUPS_16_31                0x1A
#define REMOVE_FROM_DEVICE_GROUPS_0_15            0x1B
#define REMOVE_FROM_DEVICE_GROUPS_16_31           0x1C
#define START_QUIESCENT_MODE                      0x1D
#define STOP_QUIESCENT_MODE                       0x1E
#define ENABLE_POWER_CYCLE_NOTIFICATION           0x1F
#define DISABLE_POWER_CYCLE_NOTIFICATION          0x20
#define SAVE_PERSISTENT_VARIABLES                 0x21
//Reserve                                         0X22~0X2F

/*Device query*/
#define QUERY_DEVICE_STATUS                       0x30
#define QUERY_APPLICATION_CONTROLLER_ERROR        0x31
#define QUERY_INPUT_DEVICE_ERROR                  0x32
#define QUERY_MISSING_SHORT_ADDRESS               0x33
#define QUERY_VERSION_NUMBER                      0x34
#define QUERY_NUMBER_OF_INSTANCES                 0x35
#define QUERY_CONTENT_DTR0                        0x36
#define QUERY_CONTENT_DTR1                        0x37
#define QUERY_CONTENT_DTR2                        0x38
#define QUERY_RANDOM_ADDRESS_H                    0x39
#define QUERY_RANDOM_ADDRESS_M                    0x3A
#define QUERY_RANDOM_ADDRESS_L                    0x3B
#define READ_MEMORY_LOCATION                      0x3C
#define QUERY_APPLICATION_CONTROL_ENABLED         0x3D
#define QUERY_OPERATING_MODE                      0x3E
#define QUERY_MANUFACTURER_SPECIFIC_MODE          0x3F
#define QUERY_QUIESCENT_MODE                      0x40
#define QUERY_DEVICE_GROUPS_0_7                   0x41
#define QUERY_DEVICE_GROUPS_8_15                  0x42
#define QUERY_DEVICE_GROUPS_16_23                 0x43
#define QUERY_DEVICE_GROUPS_24_31                 0x44
#define QUERY_POWER_CYCLE_NOTIFICATION            0x45
#define QUERY_DEVICE_CAPABILITIES                 0x46
#define QUERY_EXTENDED_VERSION_NUMBER             0x47
#define QUERY_RESET_STATE                         0x48
//Reserve                                         0x49~0x60

/*Instance configuration instruction*/
#define SET_EVENT_PRIORITY                        0x61
#define ENABLE_INSTANCE                           0x62
#define DISABLE_INSTANCE                          0x63
#define SET_PRIMARY_INSTANCE_GROUP                0x64
#define SET_INSTANCE_GROUP_1                      0x65
#define SET_INSTANCE_GROUP_2                      0x66
#define SET_EVENT_SCHEME                          0x67
#define SET_EVENT_FILTER                          0x68
//Reserve                                         0x69~0x7F

/*Instance query*/
#define QUERY_INSTANCE_TYPE                       0x80
#define QUERY_RESOLUTION                          0x81
#define QUERY_INSTANCE_ERROR                      0x82
#define QUERY_INSTANCE_STATUS                     0x83
#define QUERY_EVENT_PRIORITY                      0x84
//Reserve                                         0x85
#define QUERY_INSTANCE_ENABLED                    0x86
//Reserve                                         0x87
#define QUERY_PRIMARY_INSTANCE_GROUP              0x88
#define QUERY_INSTANCE_GROUP_1                    0x89
#define QUERY_INSTANCE_GROUP_2                    0x8A
#define QUERY_EVENT_SCHEME                        0x8B
#define QUERY_INPUT_VALUE                         0x8C
#define QUERY_INPUT_VALUE_LATCH                   0x8D
#define QUERY_FEATURE_TYPE                        0x8E
#define QUERY_NEXT_FEATURE_TYPE                   0x8F
#define QUERY_EVENT_FILTER_0_7                    0x90
#define QUERY_EVENT_FILTER_8_15                   0x91
#define QUERY_EVENT_FILTER_16_23                  0x92

/*Special Command*/
#define SPECIAL_COMMAND                           0xC1
#define DIRCT_WRITE_MEMORY                        0xC5
#define DTR1_DTR0                                 0xC7
#define DTR2_DTR1                                 0xC9

#define TERMINATE                                 0x00
#define INITIALISE                                0x01
#define RANDOMISE                                 0x02
#define COMPARE                                   0x03
#define WITHDRAW                                  0x04
#define SEARCH_ADDR_H                             0x05
#define SEARCH_ADDR_M                             0x06
#define SEARCH_ADDR_L                             0x07
#define PROGRAM_SHORT_ADDRESS                     0x08
#define VERIFY_SHORT_ADDRESS                      0x09
#define QUERY_SHORT_ADDRESS                       0x0A
#define WRITE_MEMORY_LOCATION                     0x20
#define WRITE_MEMORY_LOCATION_NO_REPLY            0x21
#define DTR0                                      0x30
#define DTR1                                      0x31
#define DTR2                                      0x32
#define SEND_TESTFRAME                            0x33
/*command end*/

#ifdef __cplusplus
}
#endif

#endif //__DALI_CMD_103_H__

