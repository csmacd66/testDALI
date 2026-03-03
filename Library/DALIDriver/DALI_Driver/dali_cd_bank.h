#ifndef __DALI_CD_BANK_H__
#define __DALI_CD_BANK_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    bool writeEnable: 1;
    bool readEnable: 1;
    bool writeFlash: 1;
    uint8_t reserved: 5;
} RWstatus_t;
extern volatile RWstatus_t RWstatus;

typedef struct bank
{
    uint8_t no; //order
    uint8_t bankNo; //bankNumber
    uint8_t size; //bankSize
    uint8_t *ptr;
    struct bank *next;
} Bank;

typedef struct
{
    uint8_t last_address;
    uint8_t reserved0;
    uint8_t last_bank;
    uint8_t GTIN[6];//MSB first:GTIN[0] is most significant //9
    uint8_t major_FW_version;
    uint8_t minor_FW_version;//11
    uint8_t serial_number[8];//MSB first:serial_number[0] is most significant//19
    uint8_t major_HW_version;//20
    uint8_t minor_HW_version;
    uint8_t Version101;
    uint8_t Version102;//2.0
    uint8_t Version103;
    uint8_t logical_device_number;
    uint8_t logical_gear_number;
    uint8_t logical_gear_index;
    uint8_t current_configuration_mode;//28
    uint8_t implementedParts15x;//0x1C
    uint8_t reserved1[2];//0x1D~0x7F (Total 99) Reserved not implemented //28+1+3 = 32 for 32-bit alignment
//    uint8_t additionalInformation[127];//0x80~0xFE //customer define, max 127 //if additionalInformation is used, set reserve1[] size to 99 first.
    uint8_t reserved2;
} bank0_t;
extern volatile bank0_t bank0;
void bank0SetValue(void);

typedef struct
{
    uint8_t last_address;
    uint8_t indicator;
    uint8_t lock_byte;//read-only if not 0x55
    uint8_t GTIN[6];//MSB first:GTIN[0] is most significant
    uint8_t serial_number[8];//MSB first:serial_number[0] is most significant
    uint8_t additionalInformation[2];//customer define, max 238 //Max bank size is 256 byte, the bank size needs to fit 32-bit alignment
    uint8_t reserved;//20
} bank1_t;

extern volatile bank1_t bank1;
void bank1SetValue(void);

#if(SELF_ADD_BANK)
typedef struct
{
    uint8_t last_address;
    uint8_t indicator;
    uint8_t lock_byte;//read-only if not 0x55
    uint8_t additionalInformation[4];//customer define, max 252 //Max bank size is 256 byte, the bank size needs to fit 32-bit alignment
    uint8_t reserved;
} bank2_t;

extern volatile bank2_t bank2;
void bank2SetValue(void);
#endif

#if(USE_PART351)
typedef struct
{
    uint8_t last_address;
    uint8_t indicator;
    uint8_t lock_byte;//read-only if not 0x55
    uint8_t bankVersion;
    uint8_t typeOfDevice;
    uint8_t maxBusSupplyCurrent;
    uint8_t maxAUXSupplyCurrent;
    uint8_t applicationControllerArbitration;//2
    uint8_t additionalInformation[3];//customer define, max 247 //Max bank size is 256 byte, the bank size needs to fit 32-bit alignment
    uint8_t reserved;
} bank201_t;

extern volatile bank201_t bank201;
void bank201SetValue(void);
#endif

#define F_writeEnable RWstatus.writeEnable
#define F_readEnable RWstatus.readEnable
#define F_writeFlash RWstatus.writeFlash

void bankAreaInit(void);
void bankDataInit(void);
void ReadMemoryLocation(uint8_t, uint8_t);
void WriteMemoryLocation(uint8_t, uint8_t, uint8_t, uint8_t);
uint8_t ResetMemoryBank(uint8_t);

void createBank(uint8_t, uint8_t, uint8_t, uint8_t *);
uint8_t countBank(void);
uint8_t checkBankNo(uint8_t);
uint8_t getBankNoByOrder(uint8_t);
uint8_t getOrderByBankNo(uint8_t);
uint8_t getBankSizeByOrder(uint8_t);
uint8_t getBankSizeByBankNo(uint8_t);
uint8_t *getBankptrByOrder(uint8_t);
uint8_t *getBankptrByBankNo(uint8_t);
void freeBankByOrder(uint8_t);
void freeBankByBankno(uint8_t);
void freeList(void);


#ifdef __cplusplus
}
#endif

#endif //__DALI_CD_BANK_H__

