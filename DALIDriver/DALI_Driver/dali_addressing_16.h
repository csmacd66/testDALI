#ifndef __DALI_ADDRESSING_16_H__
#define __DALI_ADDRESSING_16_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "dali_cmd_102.h"

void DALI_Addressing_16(void);

void dtr0_16(uint8_t data);
void dtr1_16(uint8_t data);
void dtr2_16(uint8_t data);
void enableDeviceType(uint8_t);
void DAPC(uint8_t address, uint8_t opcode);

void setCMD16(uint8_t, uint8_t);
void setCMD16_2(uint8_t, uint8_t);
int8_t queryCMD16(uint8_t, uint8_t);

void setSpecialCMD16(uint8_t, uint8_t);
void setSpecialCMD16_2(uint8_t, uint8_t);
int8_t querySpecialCMD16(uint8_t, uint8_t);

void removeFromGroup16(uint8_t);
void removeAddress16(uint8_t);
uint8_t Search_Address_16(void);

void readGearData(void);
void read206Value(void);
void read207Value(void);
void read208Value(void);
void read209Value(void);

double XY_u16_to_double(uint16_t data);
uint16_t XY_double_to_u16(double data);
double switchMirekKelvin(uint32_t value);

#ifdef __cplusplus
}
#endif

#endif //__DALI_ADDRESSING_16_H__

