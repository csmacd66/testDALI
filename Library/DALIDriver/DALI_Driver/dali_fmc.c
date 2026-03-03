#include "dali_cd_app.h"
#include "EEPROM_Emulate.h"

void read_bank(void)
{
    uint32_t i = 0, j = 0, k = 0;

    for (i = 0; i < TOTAL_BANK_NUMBER; i++)
    {
        for (j = 0; j < NVM_SIZE[i]; j++)
        {
            if (Read_Data(j + k, (NVM_ARRAY[i] + j)))
            {
                while (1); //error state
            }
        }

        k += NVM_SIZE[i];
    }
}

void write_bank(void)
{
    uint32_t i = 0, j = 0, k = 0, status = 0;
    SYS_UnlockReg();
    /* Enable FMC ISP function */
    FMC_Open();

    for (i = 0; i < TOTAL_BANK_NUMBER; i++)
    {
        for (j = 0; j < NVM_SIZE[i]; j++)
        {
            if ((j == 2) && (i != 0))
                status = Write_Data(j + k, 0xFF);
            else
                status = Write_Data(j + k, *(NVM_ARRAY[i] + j));

            if (status)
                while (1); //error state
        }

        k += NVM_SIZE[i];
    }

    FMC_Close();
    SYS_LockReg();
}

void read_variable(void)
{
    uint32_t i = 0, j = 0, k = 0;

    for (i = 0; i < TOTAL_BANK_NUMBER + TOTAL_NVM_TYPE; i++)
    {
        for (j = 0; j < NVM_SIZE[i]; j++)
        {
            if (Read_Data(j + k, (NVM_ARRAY[i] + j)))
            {
                while (1); //error state
            }
        }

        k += NVM_SIZE[i];
    }
}

extern EEPROM_SETTING *EEPROM_Setting;

void write_variable(void)
{
    uint32_t i = 0, j = 0, k = 0, status = 0;
    SYS_UnlockReg();
    /* Enable FMC ISP function */
    FMC_Open();

    for (i = 0; i < TOTAL_BANK_NUMBER + TOTAL_NVM_TYPE; i++)
    {
        for (j = 0; j < NVM_SIZE[i]; j++)
        {
            if ((j == 2) && (i != 0) && (i < TOTAL_BANK_NUMBER))
                status = Write_Data(j + k, 0xFF);
            else
                status = Write_Data(j + k, *(NVM_ARRAY[i] + j));

            if (status)
                while (1); //error state
        }

        k += NVM_SIZE[i];
    }

    SYS_LockReg();
}

/*write all variables to flash*/
void write_variable_once(uint8_t *address, uint8_t size)
{
    uint32_t i, j, k = 0, index, status = 0;
    SYS_UnlockReg();

    for (i = 0; i < TOTAL_BANK_NUMBER + TOTAL_NVM_TYPE; i++)
    {
        index = address - NVM_ARRAY[i];

        if (index < NVM_SIZE[i])
        {
            for (j = 0; j < size; j++)
            {
                if (Write_Data(index + j + k, NVM_ARRAY[i][index + j]))
                {
                    while (1); //error state
                }
            }

            k = 0;
        }
        else
        {
            k += NVM_SIZE[i];
        }
    }

    SYS_LockReg();
}

void erase_variable(void)
{
    uint32_t addr;
    SYS_UnlockReg();
    /* Enable FMC ISP function */
    FMC_Open();
    FMC_EnableAPUpdate();

    for (addr = DATAFLASH_BASE_ADDR; addr < DATAFLASH_MAX_ADDR; addr += 200)
        FMC_Erase(addr);

    FMC_DisableAPUpdate();
    FMC_Close();
    SYS_LockReg();
}

void read_serial_number(void)
{
    uint32_t SN[2];
    uint8_t i;
    SYS_UnlockReg();
    FMC_Open();
    SN[0] = FMC_Read(SN_ADDR);
    SN[1] = FMC_Read(SN_ADDR + 4);
    FMC_Close();
    SYS_LockReg();

    for (i = 0; i < 4; i++)
    {
        bank0.serial_number[i] = (SN[0] >> (i * 8)) & 0xFF;
        bank0.serial_number[i + 4] = (SN[1] >> (i * 8)) & 0xFF;
    }
}

void read_GTIN(void)
{
    uint32_t GTIN[2];
    uint8_t i;
    SYS_UnlockReg();
    FMC_Open();
    GTIN[0] = FMC_Read(GTIN_ADDR);
    GTIN[1] = FMC_Read(GTIN_ADDR + 4);
    FMC_Close();
    SYS_LockReg();

    for (i = 0; i < 4; i++)
        bank0.GTIN[i] = (GTIN[0] >> (i * 8)) & 0xFF;

    bank0.GTIN[4] = (GTIN[1] >> (0 * 8)) & 0xFF;
    bank0.GTIN[5] = (GTIN[1] >> (1 * 8)) & 0xFF;
}

