#include <stdlib.h>
#include "dali_cd_app.h"

volatile bank0_t bank0;
volatile bank1_t bank1;
#if(USE_PART351)
    volatile bank201_t bank201;
#endif

#if(SELF_ADD_BANK)
    volatile bank2_t bank2;  //customer define
#endif

volatile RWstatus_t RWstatus;

void bankAreaInit(void)
{
    uint8_t order = 0;

    createBank(order++, 0, sizeof(bank0), (uint8_t *)&bank0);
    createBank(order++, 1, sizeof(bank1), (uint8_t *)&bank1);
#if(SELF_ADD_BANK) //customer define
    createBank(order++, 2, sizeof(bank2), (uint8_t *)&bank2);
#endif
#if(USE_PART351)
    createBank(order++, 201, sizeof(bank201), (uint8_t *)&bank201);
#endif
    TOTAL_BANK_NUMBER = countBank();

    if (TOTAL_BANK_NUMBER != NUMBER_OF_BANKS)
        while (1); // ERROR!

    TOTAL_BANK_SIZE = 0;

    for (order = 0; order < TOTAL_BANK_NUMBER; order++)
        TOTAL_BANK_SIZE = TOTAL_BANK_SIZE + getBankSizeByOrder(order);
}

void bankDataInit(void)
{
    bank0SetValue();
    bank1SetValue();
#if(SELF_ADD_BANK)
    bank2SetValue();
#endif
#if(USE_PART351)
    bank201SetValue();
#endif
}

void bank0SetValue(void)
{
    uint8_t i;
    bank0.last_address = 0x1C; //0~0x1C, 0x80~0xFE
    bank0.reserved0 = 0xFF;
    bank0.last_bank = LAST_BANK_NO; //customer define //Modify the value depends on the last bank you added.

    for (i = 0; i <= 5; i++)
        bank0.GTIN[i] = i; //customer define

    bank0.major_FW_version = 1; //customer define
    bank0.minor_FW_version = 0; //customer define

    for (i = 0; i <= 7; i++)
        bank0.serial_number[i] = i; //customer define

    bank0.major_HW_version = 1; //customer define
    bank0.minor_HW_version = 0; //customer define
    bank0.Version101 = Version(3, 0);
    bank0.Version102 = MASK_u8;
    bank0.Version103 = Version(2, 0);
    bank0.logical_device_number = 1; //customer define
    bank0.logical_gear_number = 0; //customer define
    bank0.logical_gear_index = 0; //customer define
    bank0.current_configuration_mode = 192; //customer define //192-255
    bank0.implementedParts15x = 0; //customer define
}

void bank1SetValue(void)
{
    uint8_t i;
    bank1.last_address = 0x10; //0~0x10, 0x11~0xFE
    bank1.indicator = 0;
    bank1.lock_byte = 0xFF;

    for (i = 0; i <= 5; i++)
        bank1.GTIN[i] = 0xFF; //customer define

    for (i = 0; i <= 7; i++)
        bank1.serial_number[i] = 0xFF; //customer define
}

#if(SELF_ADD_BANK)
void bank2SetValue(void)
{
    uint8_t i;
    bank2.last_address = 0; //Manufactuer specific, customer define
    bank2.indicator = 0;
    bank2.lock_byte = MASK_u8;
    //customer define data from address 0x03 to 0xFE
}
#endif

#if(USE_PART351)
void bank201SetValue(void)
{
    bank201.last_address = 0x07;
    bank201.indicator = 0; //Manufactuer specific, customer define
    bank201.lock_byte = 0xFF;
    bank201.bankVersion = 0x01;
    bank201.typeOfDevice = 0x2; //Manufacturer specific. 0:TypeA, 1:TypeB, 2:TypeC, 3~255:reserved
    bank201.maxBusSupplyCurrent = 2; //Manufacturer specific. Maximum current consumed from the bus power supply
    bank201.maxAUXSupplyCurrent = 0; //Manufacturer specific.Maximum average power consumed from the AUX power supply
    bank201.applicationControllerArbitration = 0; //Manufacturer specific. Type B with integrated AC: 0xFF(arbitration enabled), 0xFE(arbitration disabled). All other cases: 0x00
}
#endif

//Customer need to set conditions base on the bank memory map
void ReadMemoryLocation(uint8_t bankNo, uint8_t address)
{
    F_readEnable = FALSE;

    switch (bankNo)
    {
        case 0:
            if ((address <= *(getBankptrByBankNo(bankNo))) &&
                    (((address != 1) && (address <= 0x1C)) || ((address >= 0x80) && (address <= 0xFE))))
                F_readEnable = TRUE;

            break;

        case 1:
            if (address <= *(getBankptrByBankNo(bankNo)))
                F_readEnable = TRUE;

            break;
#if(SELF_ADD_BANK)

        case 2: //User defined
            if (address <= *(getBankptrByBankNo(bankNo)))
                F_readEnable = TRUE;

            break;
#endif
#if(USE_PART351)

        case 201:
            if (address <= *(getBankptrByBankNo(bankNo)))
                F_readEnable = TRUE;

            break;
#endif

        default:
            F_readEnable = FALSE;
            break;
    }

    if (F_readEnable)
    {
        sendData(0) = *(getBankptrByBankNo(bankNo) + address);
        sendFrame_ISR(8, 0);
    }
}

//Customer need to set conditions base on the data memory type
//bank0 does not support write function
void WriteMemoryLocation(uint8_t bankNo, uint8_t address, uint8_t data, uint8_t reply)
{
    F_writeEnable = FALSE;

    switch (bankNo)
    {
        case 1:
            if (((address >= 0x3) && (address <= *(getBankptrByBankNo(bankNo))) && (*(getBankptrByBankNo(bankNo) + 2) == 0x55)) || (address == 0x02))
                F_writeEnable = TRUE;

            break;
#if(SELF_ADD_BANK)

        case 2:

            //User defined
            if (((address >= 0x3) && (address <= *(getBankptrByBankNo(bankNo))) && (*(getBankptrByBankNo(bankNo) + 2) == 0x55)) || (address == 0x02))
                F_writeEnable = TRUE;

            break;
#endif
#if(USE_PART351)

        case 201:
            if (address == 0x02)//lock byte
                F_writeEnable = TRUE;
            else if (*(getBankptrByBankNo(bankNo) + 2) == 0x55) //lock byte = 0x55
            {
                if ((address == 0x01) || ((address == 0x07) && (*(getBankptrByBankNo(bankNo) + 4) == 0x01)))
                    F_writeEnable = TRUE;
                else
                    F_writeEnable = FALSE;
            }
            else
                F_writeEnable = FALSE;

            break;
#endif

        default:
            F_writeEnable = FALSE;
            break;
    }

    if (F_writeEnable)
    {
        *(getBankptrByBankNo(bankNo) + address) = data;
        F_writeFlash = TRUE;

        if (reply)
        {
            sendData(0) = data;//return value written
            sendFrame_ISR(8, 0);
        }
    }
}

//Customer need to set condition base on the data memory type
//bank0 does not support reset function
//bankNo = 1 means reset all banks
uint8_t ResetMemoryBank(uint8_t order)
{
    if (*(getBankptrByOrder(order) + 2) == 0x55)
    {
        *(getBankptrByOrder(order) + 2) = 0xFF;
#if(SELF_ADD_BANK)

        //User defined
        if (getBankNoByOrder(order) == 2)
        {
            uint8_t i;

            //reset all data from address 0x03
            for (i = 3; i < * (getBankptrByOrder(order)); i++)
            {
                *(getBankptrByOrder(order) + i) = 0xFF;
            }
        }

#endif
        return 1;
    }

    return 0;
}

//=======================================================================================
Bank *head = NULL, *current = NULL, *previous = NULL;

void createBank(uint8_t order, uint8_t bankNumber, uint8_t bankSize, uint8_t *bankptr)
{
    current = (Bank *)malloc(sizeof(Bank));
    current->next = NULL;
    current->no = order;
    current->bankNo = bankNumber;
    current->size = bankSize;
    current->ptr = bankptr;

    if (head == NULL)
        head = current;
    else
        previous->next = current;

    previous = current;
}


uint8_t countBank(void)
{
    uint8_t bankCounter = 0;
    current = head;

    while (current != NULL)
    {
        bankCounter++;
        current = current->next;
    }

    return bankCounter;
}

uint8_t checkBankNo(uint8_t bankNumber)
{
    current = head;

    while ((current != NULL) && (current->bankNo != bankNumber))
        current = current->next;

    if (current != NULL)
        return 1;
    else
        return 0;
}

uint8_t getBankNoByOrder(uint8_t order)
{
    current = head;

    while (current != NULL && current->no != order)
        current = current->next;

    if (current != NULL)
        return current->bankNo;
    else
        return 0;
}

uint8_t getOrderByBankNo(uint8_t bankNumber)
{
    current = head;

    while ((current != NULL) && (current->bankNo != bankNumber))
        current = current->next;

    if (current != NULL)
        return current->no;
    else
        return 0;
}

uint8_t getBankSizeByOrder(uint8_t order)
{
    current = head;

    while ((current != NULL) && (current->no != order))
        current = current->next;

    if (current != NULL)
        return current->size;
    else
        return 0;
}

uint8_t *getBankptrByOrder(uint8_t order)
{
    current = head;

    while ((current != NULL) && (current->no != order))
        current = current->next;

    if (current != NULL)
        return current->ptr;
    else
        while (1);
}

uint8_t *getBankptrByBankNo(uint8_t bankNumber)
{
    current = head;

    while ((current != NULL) && (current->bankNo != bankNumber))
        current = current->next;

    if (current != NULL)
        return current->ptr;
    else
        while (1);
}

