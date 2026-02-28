#ifndef __DALI_FMC_H__
#define __DALI_FMC_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include <stdint.h>

void read_bank(void);
void write_bank(void);
void read_variable(void);
void write_variable(void);
void write_variable_once(uint8_t *, uint8_t);
void erase_variable(void);
int32_t set_data_flash_base(uint32_t);
void read_serial_number(void);
void read_GTIN(void);

#ifdef __cplusplus
}
#endif

#endif //__DALI_FMC_H__

