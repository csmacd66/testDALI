/**************************************************************************//**
 * @file     EEPROM_Emulate.h
 * @brief    This is the header file of EEPROM_Emulate.c
 * Copyright (C) 2015 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "NuMicro.h"

typedef struct
{
    uint32_t Start_Page;
    uint32_t Current_Valid_Page;
    uint32_t Current_Cursor;
} EEPROM_SETTING;

#define USE_LDROM             1     // Set '1' to use LDROM pages when all Data Flash pages is used
#define DATAFLASH_PAGE        24    // For NDA102SD2, the page amount of DataFlash is 8
#define LDROM_PAGE            8     // For NDA102SD2, the page amount of LDROM is 8
#define MAX_DATA_PER_PAGE     80    // This setting should be less than or equal to 128

#define DATAFLASH_BASE_ADDR   0xD000
#define DATAFLASH_MAX_ADDR    (DATAFLASH_BASE_ADDR + FMC_FLASH_PAGE_SIZE*DATAFLASH_PAGE)
#define MAX_DATA_AMOUNT       256UL
#define STATUS_UNWRITTEN      0xFFFF

#define EVEN_ADDR_POS         16UL
#define EVEN_ADDR_MASK        0xFF0000
#define EVEN_DATA_POS         24UL
#define EVEN_DATA_MASK        0xFF000000
#define ODD_ADDR_POS          0UL
#define ODD_ADDR_MASK         0xFF
#define ODD_DATA_POS          8UL
#define ODD_DATA_MASK         0xFF00

#define ERR_OVER_DATA_AMOUNT      0x01
#define ERR_OVER_PAGE_AMOUNT      0x02
#define ERR_ERROR_INDEX           0x03
#define ERR_WRITE_BLOCK_STATUS    0x04

void FMC_Enable(void);
void FMC_Disable(void);
uint32_t Init_EEPROM(uint32_t data_size, uint32_t use_pages);
uint8_t Search_Valid_Page(void);
uint32_t Read_Data(uint32_t index, volatile uint8_t *data);
uint32_t Write_Data(uint32_t index, uint8_t data);
void Manage_Next_Page(uint32_t EEPROM_number);
uint16_t Get_Cycle_Counter(uint32_t EEPROM_number);

