#ifndef __DALI_ADDRESSING_24_H__
#define __DALI_ADDRESSING_24_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "dali_cmd_103.h"

void DALI_Addressing_24(void);

void dtr0_24(uint8_t data);
void dtr1_24(uint8_t data);
void dtr2_24(uint8_t data);
void dtr1_dtr0(uint8_t data1, uint8_t data0);
void dtr2_dtr1(uint8_t data2, uint8_t data1);
int8_t direct_write_momory(uint8_t offset, uint8_t data);

void setStandardCMD24(uint8_t address, uint8_t opcode);
int8_t queryStandardCMD24(uint8_t address, uint8_t opcode);

void setInstanceCMD24(uint8_t address, uint8_t instance, uint8_t opcode);
int8_t queryInstanceCMD24(uint8_t address, uint8_t instance, uint8_t opcode);

void setSpecialCMD24(uint8_t instance, uint8_t opcode);
void setSpecialCMD24_2(uint8_t instance, uint8_t opcode);
int8_t querySpecialCMD24(uint8_t instance, uint8_t opcode);

void removeFromGroup24(uint8_t address);
void removeAddress24(uint8_t address);
uint8_t Search_Address_24(void);

#ifdef __cplusplus
}
#endif

#endif //__DALI_ADDRESSING_24_H__

