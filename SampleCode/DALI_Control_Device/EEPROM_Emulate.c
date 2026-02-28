/**************************************************************************//**
 * @file     EEPROM_Emulate.c
 * @brief    Emulate Data Flash as EEPROM
 * Copyright (C) 2015 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include "EEPROM_Emulate.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Variables                                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t g_u32Total_Data;
uint32_t g_u32Single_Pages;
uint32_t g_u32Total_Pages;
uint32_t g_u32EEPROM_Amount;
uint8_t *g_pu8EEPROM_Data;
EEPROM_SETTING *EEPROM_Setting;

/**
  * @brief     Enable FMC ISP function.
  */
void FMC_Enable(void)
{
    /* Unlock protected registers for ISP function */
    SYS_UnlockReg();

    CLK->AHBCLK |= CLK_AHBCLK_ISPCKEN_Msk;
    /* Enable ISP function */
    FMC_Open();
    FMC_EnableAPUpdate();

    /* Enable LDROM to be able to program when using LDROM */
    if (USE_LDROM == 1)
        FMC_EnableLDUpdate();
}

/**
  * @brief     Disable FMC ISP function.
  */
void FMC_Disable(void)
{
    /* Disable LDROM update function */
    if (USE_LDROM == 1)
        FMC_DisableLDUpdate();

    FMC_DisableAPUpdate();
    /* Disable ISP function */
    FMC_Close();

    /* Lock protected registers */
    SYS_LockReg();
}

/**
  * @brief      Initial Data Flash as EEPROM.
  * @param[in]  data_size: The amount of user's data, unit in byte.
  * @param[in]  use_pages: The amount of Flash pages which user want to use for single emulated EEPROM.
  * @retval     Err_OverPageSize: The amount of user's data is over than the maximun amount.
  * @retval     ERR_OVER_PAGE_AMOUNT: The amount of page which user want to use is over than the maximun amount.
  * @retval     0: Success
  */
uint32_t Init_EEPROM(uint32_t data_amount, uint32_t use_pages)
{
    uint32_t i;

    /* The amount of data includes 1 byte address and 1 byte data */
    g_u32Total_Data = data_amount;
    /* The amount of pages which are used to emulate EEPROM */
    g_u32Single_Pages = use_pages;
    g_u32EEPROM_Amount = g_u32Total_Data / MAX_DATA_PER_PAGE;

    if (g_u32Total_Data % MAX_DATA_PER_PAGE)
        g_u32EEPROM_Amount++;

    g_u32Total_Pages = g_u32EEPROM_Amount * g_u32Single_Pages;

    /* Check setting is valid or not */
    /* The amount of data for single EEPROM is more than the maximun amount or not */
    if (MAX_DATA_PER_PAGE > MAX_DATA_AMOUNT)
        return ERR_OVER_DATA_AMOUNT;

    /* The amount of Flash pages is more than the valid value */
    if (g_u32Total_Pages > (DATAFLASH_PAGE + LDROM_PAGE * USE_LDROM))
        return ERR_OVER_PAGE_AMOUNT;

    /* Init SRAM for data */
    g_pu8EEPROM_Data = (uint8_t *)malloc(sizeof(uint8_t) * g_u32Total_Data);

    /* Fill initial data 0xFF */
    for (i = 0; i < g_u32Total_Data; i++)
    {
        g_pu8EEPROM_Data[i] = 0xFF;
    }

    /* Init SRAM for EEPROM setting */
    EEPROM_Setting = (EEPROM_SETTING *)malloc(sizeof(EEPROM_SETTING) * g_u32EEPROM_Amount);

    /* Init EEPROM setting */
    for (i = 0; i < g_u32EEPROM_Amount; i++)
    {
        EEPROM_Setting[i].Start_Page = g_u32Single_Pages * i;
        EEPROM_Setting[i].Current_Valid_Page = 0;
        EEPROM_Setting[i].Current_Cursor = 0;
    }

    return 0;
}

/**
  * @brief     Search which page has valid data and where is current cursor for the next data to write.
  */
uint8_t Search_Valid_Page(void)
{
    uint32_t i, j, temp;
    uint32_t page_addr;
    uint8_t addr, data, state = 1;
    uint16_t *Page_Status_ptr;

    /* Enable FMC ISP function */
    FMC_Enable();

    /* Set information of each pages to Page_Status */
    Page_Status_ptr = (uint16_t *)malloc(sizeof(uint16_t) * g_u32Single_Pages);

    /* Init for each emulated EEPROM */
    for (i = 0; i < g_u32EEPROM_Amount; i++)
    {
        for (j = 0; j < g_u32Single_Pages; j++)
        {
            page_addr = DATAFLASH_BASE_ADDR + FMC_FLASH_PAGE_SIZE * (EEPROM_Setting[i].Start_Page + j);

            /* Change address to LDROM if address is over Data Flash address */
            if (page_addr >= DATAFLASH_MAX_ADDR)
                page_addr = FMC_LDROM_BASE + (page_addr - DATAFLASH_MAX_ADDR);

            /* Get page status */
            Page_Status_ptr[j] = (uint16_t)FMC_Read(page_addr);
        }

        /* Search which page has valid data */
        for (j = 0; j < g_u32Single_Pages; j++)
        {
            if (Page_Status_ptr[j] != STATUS_UNWRITTEN)
                EEPROM_Setting[i].Current_Valid_Page = j;
        }

        /* If Data Flash is used for first time, set counter = 0 */
        if (Page_Status_ptr[EEPROM_Setting[i].Current_Valid_Page] == STATUS_UNWRITTEN)
        {
            /* Point to valid page */
            page_addr = DATAFLASH_BASE_ADDR + FMC_FLASH_PAGE_SIZE * (EEPROM_Setting[i].Start_Page + EEPROM_Setting[i].Current_Valid_Page);

            if (page_addr >= DATAFLASH_MAX_ADDR)
                page_addr = FMC_LDROM_BASE + (page_addr - DATAFLASH_MAX_ADDR);

            /* Set counter = 0 */
            FMC_Write(page_addr, 0xFFFF0000);
            /* Set cursor to current Data Flash address */
            EEPROM_Setting[i].Current_Cursor = 2;
        }
        else
        {
            /* Search where is current cursor for the next data to write and get the data has been written */
            /* Point to valid page */
            state = 0;
            page_addr = DATAFLASH_BASE_ADDR + FMC_FLASH_PAGE_SIZE * (EEPROM_Setting[i].Start_Page + EEPROM_Setting[i].Current_Valid_Page);

            if (page_addr >= DATAFLASH_MAX_ADDR)
                page_addr = FMC_LDROM_BASE + (page_addr - DATAFLASH_MAX_ADDR);

            /* Check even value */
            temp = FMC_Read(page_addr);
            addr = (temp & EVEN_ADDR_MASK) >> EVEN_ADDR_POS;
            data = (temp & EVEN_DATA_MASK) >> EVEN_DATA_POS;

            /* Check Address is 0xFF (un-written) of not */
            if (addr == 0xFF)
            {
                /* If Address is 0xFF, then set cursor to current Data Flash address */
                EEPROM_Setting[i].Current_Cursor = 2;
            }
            else
            {
                /* Copy the address and data to SRAM */
                g_pu8EEPROM_Data[(MAX_DATA_PER_PAGE * i) + addr] = data;

                /* Check the whole page */
                for (j = 4; j < FMC_FLASH_PAGE_SIZE; j += 4)
                {
                    /* Check odd value */
                    temp = FMC_Read(page_addr + j);
                    addr = (temp & ODD_ADDR_MASK) >> ODD_ADDR_POS;
                    data = (temp & ODD_DATA_MASK) >> ODD_DATA_POS;

                    /* Check Address is 0xFF (un-written) of not */
                    if (addr == 0xFF)
                    {
                        /* If Address is 0xFF, then set cursor to current Data Flash address */
                        EEPROM_Setting[i].Current_Cursor = j;
                        break;
                    }
                    else
                    {
                        /* Copy the address and data to SRAM */
                        g_pu8EEPROM_Data[(MAX_DATA_PER_PAGE * i) + addr] = data;
                    }

                    /* Check even value */
                    addr = (temp & EVEN_ADDR_MASK) >> EVEN_ADDR_POS;
                    data = (temp & EVEN_DATA_MASK) >> EVEN_DATA_POS;

                    /* Check Address is 0xFF (un-written) of not */
                    if (addr == 0xFF)
                    {
                        /* If Address is 0xFF, then set cursor to current Data Flash address */
                        EEPROM_Setting[i].Current_Cursor = j + 2;
                        break;
                    }
                    else
                    {
                        /* Copy the address and data to SRAM */
                        g_pu8EEPROM_Data[(MAX_DATA_PER_PAGE * i) + addr] = data;
                    }
                }
            }
        }
    }

    /* Disable FMC ISP function */
    FMC_Disable();
    return state;
}

/**
  * @brief      Read one byte data from SRAM.
  * @param[in]  index: The index of data address.
  * @param[in]  data: The data in the index of data address from SRAM.
  * @retval     ERR_ERROR_INDEX: The input index is now valid.
  * @retval     0: Success
  */
uint32_t Read_Data(uint32_t index, volatile uint8_t *data)
{
    /* Check the index is valid or not */
    if (index >= g_u32Total_Data)
    {
        return ERR_ERROR_INDEX;
    }

    /* Get the data from SRAM */
    *data = g_pu8EEPROM_Data[index];

    return 0;
}

/**
  * @brief     Write one byte data to SRAM and current valid page.
               If this index has the same data, it will not changed in SRAM and Data Flash.
               If current valid page is full, execute Manage_Next_Page() to copy valid data to next page.
  * @param[in]  index: The index of data address.
  * @param[in]  data: The data that will be writeen.
  * @retval     ERR_ERROR_INDEX: The input index is not valid.
  * @retval     0: Success
  */
uint32_t Write_Data(uint32_t index, uint8_t data)
{
    uint32_t temp = 0;
    uint32_t target_EEPROM_number;
    uint8_t target_EEPROM_index;
    uint32_t page_addr;

    /* Check the index is valid or not */
    if (index > g_u32Total_Data)
    {
        return ERR_ERROR_INDEX;
    }

    /* If the writing data equals to current data, then skip the write process */
    if (g_pu8EEPROM_Data[index] == data)
    {
        return 0;
    }

    /* Enable FMC ISP function */
    FMC_Enable();

    /* Point to target EEPROM and index */
    target_EEPROM_number = index / MAX_DATA_PER_PAGE;
    target_EEPROM_index = (uint8_t)(index % MAX_DATA_PER_PAGE);

    /* Point to valid page */
    page_addr = DATAFLASH_BASE_ADDR + FMC_FLASH_PAGE_SIZE * (EEPROM_Setting[target_EEPROM_number].Start_Page + EEPROM_Setting[target_EEPROM_number].Current_Valid_Page);

    if (page_addr >= DATAFLASH_MAX_ADDR)
        page_addr = FMC_LDROM_BASE + (page_addr - DATAFLASH_MAX_ADDR);

    /* Current cursor points to odd position*/
    if ((EEPROM_Setting[target_EEPROM_number].Current_Cursor & 0x3) == 0)
    {
        /* Write data to Data Flash */
        temp = 0xFFFF0000 | (target_EEPROM_index << ODD_ADDR_POS) | (data << ODD_DATA_POS);
        FMC_Write(page_addr + EEPROM_Setting[target_EEPROM_number].Current_Cursor, temp);
        /* Write data to SRAM */
        g_pu8EEPROM_Data[index] = data;
    }
    /* Current cursor points to even position*/
    else
    {
        /* Read the odd position data */
        temp = FMC_Read(page_addr + (EEPROM_Setting[target_EEPROM_number].Current_Cursor - 2));
        /* Combine odd position data and even position data */
        temp &= ~(EVEN_ADDR_MASK | EVEN_DATA_MASK);
        temp |= (target_EEPROM_index << EVEN_ADDR_POS) | (data << EVEN_DATA_POS);
        /* Write data to Data Flash */
        FMC_Write(page_addr + (EEPROM_Setting[target_EEPROM_number].Current_Cursor - 2), temp);
        /* Write data to SRAM */
        g_pu8EEPROM_Data[index] = data;
    }

    /* If current cursor points to the last position, then execute Manage_Next_Page() */
    if (EEPROM_Setting[target_EEPROM_number].Current_Cursor == (FMC_FLASH_PAGE_SIZE - 2))
    {
        /* Copy valid data to next page */
        Manage_Next_Page(target_EEPROM_number);
    }
    /* Add current cursor */
    else
    {
        /* Set current cursor to next position */
        EEPROM_Setting[target_EEPROM_number].Current_Cursor += 2;
    }

    /* Disable FMC ISP function */
    FMC_Disable();

    return 0;
}

/**
  * @brief     Manage the valid data from SRAM to new page.
  */
void Manage_Next_Page(uint32_t EEPROM_number)
{
    uint32_t i, counter, temp = 0, data_flag = 0;
    uint32_t data_start_index, data_end_index;
    uint32_t page_addr, new_page, new_page_addr;

    /* Enable FMC ISP function */
    FMC_Enable();

    /* Point to valid page */
    page_addr = DATAFLASH_BASE_ADDR + FMC_FLASH_PAGE_SIZE * (EEPROM_Setting[EEPROM_number].Start_Page + EEPROM_Setting[EEPROM_number].Current_Valid_Page);

    if (page_addr >= DATAFLASH_MAX_ADDR)
        page_addr = FMC_LDROM_BASE + (page_addr - DATAFLASH_MAX_ADDR);

    /* Copy the valid data (not 0xFF) from SRAM to new valid page */
    /* Get counter from the first two bytes */
    counter = FMC_Read(page_addr);

    /* If current valid page is the last page, choose the first page as valid page */
    if ((EEPROM_Setting[EEPROM_number].Current_Valid_Page + 1) == g_u32Single_Pages)
    {
        new_page = 0;
        /* Add counter to record 1 E/W cycle finished for all pages */
        counter++;
    }
    else
    {
        new_page = EEPROM_Setting[EEPROM_number].Current_Valid_Page + 1;
    }

    /* Point to new page */
    new_page_addr = DATAFLASH_BASE_ADDR + FMC_FLASH_PAGE_SIZE * (EEPROM_Setting[EEPROM_number].Start_Page + new_page);

    if (new_page_addr >= DATAFLASH_MAX_ADDR)
        new_page_addr = FMC_LDROM_BASE + (new_page_addr - DATAFLASH_MAX_ADDR);

    /* Data start index of current EEPROM */
    data_start_index = EEPROM_number * MAX_DATA_PER_PAGE;

    /* Data end index of current EEPROM */
    if ((data_start_index + MAX_DATA_PER_PAGE) < g_u32Total_Data)
        data_end_index = data_start_index + MAX_DATA_PER_PAGE;
    else
        data_end_index = g_u32Total_Data;

    /* Copy first valid data */
    while (1)
    {
        /* Not a valid data, skip */
        if (g_pu8EEPROM_Data[data_start_index] == 0xFF)
        {
            data_start_index++;
        }
        /* Combine counter and first valid data, and write to new page */
        else
        {
            counter &= ~(EVEN_ADDR_MASK | EVEN_DATA_MASK);
            counter |= ((data_start_index - (MAX_DATA_PER_PAGE * EEPROM_number)) << EVEN_ADDR_POS) | (g_pu8EEPROM_Data[data_start_index] << EVEN_DATA_POS);
            FMC_Write(new_page_addr, counter);
            data_start_index++;
            break;
        }
    }

    /* Copy the rest of data */
    for (i = 4; data_start_index < data_end_index; data_start_index++)
    {
        /* Not a valid data, skip */
        if (g_pu8EEPROM_Data[data_start_index] == 0xFF)
        {
            continue;
        }
        /* Write to new page */
        else
        {
            /* Collect two valid data and write to Data Flash */
            /* First data, won't write to Data Flash immediately */
            if (data_flag == 0)
            {
                temp |= ((data_start_index - (MAX_DATA_PER_PAGE * EEPROM_number)) << ODD_ADDR_POS) | (g_pu8EEPROM_Data[data_start_index] << ODD_DATA_POS);
                data_flag = 1;
            }
            /* Second data, write to Data Flash after combine with first data */
            else
            {
                temp |= ((data_start_index - (MAX_DATA_PER_PAGE * EEPROM_number)) << EVEN_ADDR_POS) | (g_pu8EEPROM_Data[data_start_index] << EVEN_DATA_POS);
                FMC_Write(new_page_addr + i, temp);
                temp = 0;
                data_flag = 0;
                i += 4;
            }
        }
    }

    /* Set cursor to new page */
    EEPROM_Setting[EEPROM_number].Current_Cursor = i;

    /* If there is one valid data left, write to Data Flash */
    if (data_flag == 1)
    {
        temp |= 0xFFFF0000;
        FMC_Write(new_page_addr + i, temp);
        EEPROM_Setting[EEPROM_number].Current_Cursor += 2;
    }

    /* Erase the old page */
    FMC_Erase(page_addr);
    /* Point to new valid page */
    EEPROM_Setting[EEPROM_number].Current_Valid_Page = new_page;

    /* Disable FMC ISP function */
    FMC_Disable();
}

/**
  * @brief      Get the cycle counter for how many cycles has page been erased/programmed.
  * @param[in]  EEPROM_number: The number of target EEPROM.
  * @retval     Cycle_Conter: The cycles that page has been erased/programmed.
  */
uint16_t Get_Cycle_Counter(uint32_t EEPROM_number)
{
    uint32_t page_addr;
    uint16_t Cycle_Counter;

    /* Point to valid page */
    page_addr = DATAFLASH_BASE_ADDR + FMC_FLASH_PAGE_SIZE * (EEPROM_Setting[EEPROM_number].Start_Page + EEPROM_Setting[EEPROM_number].Current_Valid_Page);

    if (page_addr >= DATAFLASH_MAX_ADDR)
        page_addr = FMC_LDROM_BASE + (page_addr - DATAFLASH_MAX_ADDR);

    /* Get the cycle counter from first two bytes in current Data Flash page */
    Cycle_Counter = (uint16_t)FMC_Read(page_addr);

    return Cycle_Counter;
}

