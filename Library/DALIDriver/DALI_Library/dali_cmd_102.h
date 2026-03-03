#ifndef __DALI_CMD_103_H__
#define __DALI_CMD_103_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>
#define BROADCAST                                       0xFF
#define BROADCAST_UNADDRESSED                           0xFD
/*Standard Command*/
#define OFF                                             0
#define UP                                              1
#define DOWN                                            2
#define STEP_UP                                         3
#define STEP_DOWN                                       4
#define RECALL_MAX_LEVEL                                5
#define RECALL_MIN_LEVEL                                6
#define STEP_DOWN_AND_OFF                               7
#define ON_AND_STEP_UP                                  8
#define ENABLE_DAPC_SEQUENCE                            9
#define GO_TO_LAST_ACTIVE_LEVEL                         10        //DALI2
//Reserve                                               11~15
#define GO_TO_SCENE_0                                   16
#define GO_TO_SCENE_1                                   17
#define GO_TO_SCENE_2                                   18
#define GO_TO_SCENE_3                                   19
#define GO_TO_SCENE_4                                   20
#define GO_TO_SCENE_5                                   21
#define GO_TO_SCENE_6                                   22
#define GO_TO_SCENE_7                                   23
#define GO_TO_SCENE_8                                   24
#define GO_TO_SCENE_9                                   25
#define GO_TO_SCENE_10                                  26
#define GO_TO_SCENE_11                                  27
#define GO_TO_SCENE_12                                  28
#define GO_TO_SCENE_13                                  29
#define GO_TO_SCENE_14                                  30
#define GO_TO_SCENE_15                                  31
#define RESET                                           32
#define STORAGE_ACTUAL_LEVEL_TO_DTR0                    33
#define SAVE_PERSISTENT_VARIABLES                       34        //DALI2
#define SET_OPERATING_MODE                              35        //DALI2
#define RESET_MEMORY_BANK                               36        //DALI2
#define IDENTIFY_DEVICE                                 37        //DALI2
//Reserve                                               38~41
#define SET_MAX_LEVEL                                   42
#define SET_MIN_LEVEL                                   43
#define SET_SYSTEM_FAILURE_LEVEL                        44
#define SET_POWER_ON_LEVEL                              45
#define SET_FADE_TIME                                   46
#define SET_FADE_RATE                                   47
#define SET_EXTENDED_FADE_TIME                          48        //DALI2
//Reserve                                               49~63
#define SET_SCENE_0                                     64
#define SET_SCENE_1                                     65
#define SET_SCENE_2                                     66
#define SET_SCENE_3                                     67
#define SET_SCENE_4                                     68
#define SET_SCENE_5                                     69
#define SET_SCENE_6                                     70
#define SET_SCENE_7                                     71
#define SET_SCENE_8                                     72
#define SET_SCENE_9                                     73
#define SET_SCENE_10                                    74
#define SET_SCENE_11                                    75
#define SET_SCENE_12                                    76
#define SET_SCENE_13                                    77
#define SET_SCENE_14                                    78
#define SET_SCENE_15                                    79
#define REMOVE_FROM_SCENE_0                             80
#define REMOVE_FROM_SCENE_1                             81
#define REMOVE_FROM_SCENE_2                             82
#define REMOVE_FROM_SCENE_3                             83
#define REMOVE_FROM_SCENE_4                             84
#define REMOVE_FROM_SCENE_5                             85
#define REMOVE_FROM_SCENE_6                             86
#define REMOVE_FROM_SCENE_7                             87
#define REMOVE_FROM_SCENE_8                             88
#define REMOVE_FROM_SCENE_9                             89
#define REMOVE_FROM_SCENE_10                            90
#define REMOVE_FROM_SCENE_11                            91
#define REMOVE_FROM_SCENE_12                            92
#define REMOVE_FROM_SCENE_13                            93
#define REMOVE_FROM_SCENE_14                            94
#define REMOVE_FROM_SCENE_15                            95
#define ADD_TO_GROUP_0                                  96
#define ADD_TO_GROUP_1                                  97
#define ADD_TO_GROUP_2                                  98
#define ADD_TO_GROUP_3                                  99
#define ADD_TO_GROUP_4                                  100
#define ADD_TO_GROUP_5                                  101
#define ADD_TO_GROUP_6                                  102
#define ADD_TO_GROUP_7                                  103
#define ADD_TO_GROUP_8                                  104
#define ADD_TO_GROUP_9                                  105
#define ADD_TO_GROUP_10                                 106
#define ADD_TO_GROUP_11                                 107
#define ADD_TO_GROUP_12                                 108
#define ADD_TO_GROUP_13                                 109
#define ADD_TO_GROUP_14                                 110
#define ADD_TO_GROUP_15                                 111
#define REMOVE_FROM_GROUP_0                             112
#define REMOVE_FROM_GROUP_1                             113
#define REMOVE_FROM_GROUP_2                             114
#define REMOVE_FROM_GROUP_3                             115
#define REMOVE_FROM_GROUP_4                             116
#define REMOVE_FROM_GROUP_5                             117
#define REMOVE_FROM_GROUP_6                             118
#define REMOVE_FROM_GROUP_7                             119
#define REMOVE_FROM_GROUP_8                             120
#define REMOVE_FROM_GROUP_9                             121
#define REMOVE_FROM_GROUP_10                            122
#define REMOVE_FROM_GROUP_11                            123
#define REMOVE_FROM_GROUP_12                            124
#define REMOVE_FROM_GROUP_13                            125
#define REMOVE_FROM_GROUP_14                            126
#define REMOVE_FROM_GROUP_15                            127
#define SET_SHORT_ADDRESS                               128
#define ENABLE_WRITE_MEMORY                             129
#define SET_POWER_ON_DELAY                              130
//Reserve                                               131~143
#define QUERY_STATUS                                    144
#define QUERY_CONTROL_GEAR_PRESENT                      145
#define QUERY_LAMP_FAILURE                              146
#define QUERY_LAMP_POWER_ON                             147
#define QUERY_LIMIT_ERROR                               148
#define QUERY_RESET_STATE                               149
#define QUERY_MISSING_SHORT_ADDRESS                     150
#define QUERY_VERSION_NUMBER                            151
#define QUERY_CONTENT_DTR0                              152
#define QUERY_DEVICE_TYPE                               153
#define QUERY_PHYSICAL_MINIMUM                          154
#define QUERY_POWER_FAILURE                             155
#define QUERY_CONTENT_DTR1                              156
#define QUERY_CONTENT_DTR2                              157
#define QUERY_OPERATING_MODE                            158        //DALI2
#define QUERY_LIGHT_SOURCE_TYPE                         159        //DALI2
#define QUERY_ACTUAL_LEVEL                              160
#define QUERY_MAX_LEVEL                                 161
#define QUERY_MIN_LEVEL                                 162
#define QUERY_POWER_ON_LEVEL                            163
#define QUERY_SYSTEM_FAILURE_LEVEL                      164
#define QUERY_FADE_TIME_FADE_RATE                       165
#define QUERY_MANUFACTURER_SPECIFIC_MODE                166        //DALI2
#define QUERY_NEXT_DEVICE_TYPE                          167        //DALI2
#define QUERY_EXTENDED_FADE_TIME                        168        //DALI2
//Reserve                                               169
#define QUERY_CONTROL_GEAR_FAILURE                      170        //DALI2
#define QUERY_POWER_ON_DELAY                            171        //DALI2
//Reserve                                               172~175
#define QUERY_SCENE_LEVEL_0                             176        //DALI2
#define QUERY_SCENE_LEVEL_1                             177
#define QUERY_SCENE_LEVEL_2                             178
#define QUERY_SCENE_LEVEL_3                             179
#define QUERY_SCENE_LEVEL_4                             180
#define QUERY_SCENE_LEVEL_5                             181
#define QUERY_SCENE_LEVEL_6                             182
#define QUERY_SCENE_LEVEL_7                             183
#define QUERY_SCENE_LEVEL_8                             184
#define QUERY_SCENE_LEVEL_9                             185
#define QUERY_SCENE_LEVEL_10                            186
#define QUERY_SCENE_LEVEL_11                            187
#define QUERY_SCENE_LEVEL_12                            188
#define QUERY_SCENE_LEVEL_13                            189
#define QUERY_SCENE_LEVEL_14                            190
#define QUERY_SCENE_LEVEL_15                            191
#define QUERY_GROUPS07                                  192
#define QUERY_GROUPS815                                 193
#define QUERY_RANDOM_ADDRESS_H                          194
#define QUERY_RANDOM_ADDRESS_M                          195
#define QUERY_RANDOM_ADDRESS_L                          196
#define READ_MEMORY_LOCATION                            197
//Reserve                                               198~223

/*APPLICATION EXTENDED COMMANDS*/
#define CMD224                                          224
#define CMD225                                          225
#define CMD226                                          226
#define CMD227                                          227
#define CMD228                                          228
#define CMD229                                          229
#define CMD230                                          230
#define CMD231                                          231
#define CMD232                                          232
#define CMD233                                          233
#define CMD234                                          234
#define CMD235                                          235
#define CMD236                                          236
#define CMD237                                          237
#define CMD238                                          238
#define CMD239                                          239
#define CMD240                                          240
#define CMD241                                          241
#define CMD242                                          242
#define CMD243                                          243
#define CMD244                                          244
#define CMD245                                          245
#define CMD246                                          246
#define CMD247                                          247
#define CMD248                                          248
#define CMD249                                          249
#define CMD250                                          250
#define CMD251                                          251
#define CMD252                                          252
#define CMD253                                          253
#define CMD254                                          254
#define QUERY_EXTENDED_VERSION_NUMBER                   255

/*Special Command*/
#define TERMINATE                                       0xA1//256
#define DTR0                                            0xA3//257
#define INITIALISE                                      0xA5//258
#define RANDOMISE                                       0xA7//259
#define COMPARE                                         0xA9//260
#define WITHDRAW                                        0xAB//261
#define PING                                            0xAD//    //DALI2
#define SEARCH_ADDR_H                                   0xB1//264
#define SEARCH_ADDR_M                                   0xB3//265
#define SEARCH_ADDR_L                                   0xB5//266
#define PROGRAM_SHORT_ADDRESS                           0xB7//267
#define VERIFY_SHORT_ADDRESS                            0xB9//268
#define QUERY_SHORT_ADDRESS                             0xBB//269
//#define PHYSICAL_SELECTION                            0xBD//270
#define ENABLE_DEVICE_TYPE                              0xC1//272
#define DTR1                                            0xC3//273
#define DTR2                                            0xC5//274
#define WRITE_MEMORY_LOCATION                           0xC7//275
#define WRITE_MEMORY_LOCATION_NO_REPLY                  0XC9//    //DALI2

#ifdef __cplusplus
}
#endif

#endif //__DALI_CMD_103_H__

