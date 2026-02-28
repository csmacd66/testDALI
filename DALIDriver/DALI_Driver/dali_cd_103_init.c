#include "dali_cd_app.h"
#include "EEPROM_Emulate.h"

void DALI_Init(void)
{
    transmitParameter();
    storageParameter();

#if(NUMBER_OF_INSTANCES)
    instanceParameter();
#else
    g_parameter.numberOfInstances = 0;
#endif
    g_parameter.numberOfFeatures = NUMBER_OF_FEATURES;
    featureType(0) = 0;
    init_EEPROM:
    if (Init_EEPROM(TOTAL_BANK_SIZE + TOTAL_NVM_SIZE, 8))
        while (1); //Error! Please refer to EEPROM_Emulate.c for error information

    if (Search_Valid_Page())
    {
        DefaultState();
        bankDataInit();
        write_variable();
    }
    else
    {
        read_variable();
#if(USE_PART301)
        if (old_amount_301(0) != SW_AMOUNT_OF_301)
        {
            erase_variable();
            goto init_EEPROM;
        }
#endif
    }

#if 0  //Customer define. If you put the GTIN & SN value in Flash GTIN_ADDR and SN_ADDR, change #if 0 to $if 1.
    read_GTIN();
    read_serial_number();
    write_variable();
#endif
    PowerOnState();
    checkShortAddress();
}

void transmitParameter(void)
{
    INIT_TIME = 900000; //15min
    SEND_TWICE_TIME = 120; //116~126ms (94~104(Grey area) -2(stop condition) +24(next frame length))
    IDENTIFICATION_TIME = 10000; //10s
    NEW_POWER_ON_TIME[0] = 1320; //1.3s
    NEW_POWER_ON_TIME[1] = 5000; //5s
    BACK_TIME = setSettlingTime(0) * 100 * CyclesPerUs; //4.6 ms after stop condition ends
    MAX_SIGNAL_TIME = 580 * CyclesPerUs;
    MIN_SIGNAL_TIME = 280 * CyclesPerUs;
    STOP_TIME = 1800 * CyclesPerUs;
    STOP_CONDITION = 390 * CyclesPerUs;
    HIGH_TIME = 417 * CyclesPerUs; //Customer Define // IO high level length //set DALI BUS half bit length to 417
    LOW_TIME = 417 * CyclesPerUs; //Customer Define // IO low level length //set DALI BUS half bit length to 417
    TYP_TIME = 417 * CyclesPerUs;
#if CHECK_COLLISION
    BREAK_TIME = 1300 * CyclesPerUs; //1.2 //1.2 ~ 1.4 ms
    MIN_GREY_AREA = 380 * CyclesPerUs; //Customer Define //357~400
    MAX_GREY_AREA = 466 * CyclesPerUs; //Customer Define //434~476
#endif
}

void storageParameter(void)
{
    uint8_t index;
    bankAreaInit();

    for (index = 0; index < TOTAL_BANK_NUMBER; index++)
    {
        NVM_SIZE[index] = getBankSizeByOrder(index);
        NVM_ARRAY[index] = getBankptrByOrder(index);
    }

    TOTAL_NVM_SIZE = 0;
    NVM_SIZE[index] = Nvariable;
    TOTAL_NVM_SIZE = NVM_SIZE[index];
    NVM_ARRAY[index] = (uint8_t *)(&(g_parameter.var.var));
}

void instanceParameter(void)
{
#if (NUMBER_OF_INSTANCES)
    uint8_t index = TOTAL_BANK_NUMBER + 1, instanceTypeIndex, instanceNumber = 0, type, i;

    for (instanceTypeIndex = 0; instanceTypeIndex < TYPES_OF_INSTANCES; instanceTypeIndex++)
    {
        initInstanceInfo(INSTANCE_TYPE[instanceTypeIndex], instanceTypeIndex, instanceNumber);
        type = INSTANCE_TYPE[instanceTypeIndex];

        for (i = startInstanceNo(instanceTypeIndex); i < startInstanceNo(instanceTypeIndex) + amountType(instanceTypeIndex); i++)
        {
            NVM_SIZE[index + i] = Ivariable;
            NVM_ARRAY[index + i] = (uint8_t *)(&instanceNo[instanceNumber].NVM.instanceNVM);
            instanceTypeIndex(instanceNumber) = instanceTypeIndex;
            initInstanceParameter(instanceNumber, type);
            instanceNumber++;
        }
    }

#if(USE_PART301 == 1 && TYPES_OF_INSTANCES == 1)
    g_parameter.numberOfInstances = instanceNumber;
#else
    g_parameter.numberOfInstances = NUMBER_OF_INSTANCES;
#endif

    for (i = 0; i < g_parameter.numberOfInstances; i++)
        TOTAL_NVM_SIZE += NVM_SIZE[index + i];

#endif
}

void DefaultState(void)
{
#if(NUMBER_OF_INSTANCES)
    uint8_t instanceNumber, type, i;

    for (i = 0; i < TYPES_OF_INSTANCES; i++)
    {
        type = INSTANCE_TYPE[i];

        for (instanceNumber = startInstanceNo(i); instanceNumber < startInstanceNo(i) + amountType(i); instanceNumber++)
            initDefaultInstance(instanceNumber, type);
    }

#endif
    DefaultDevice();
}

void ResetState(void)
{
#if(NUMBER_OF_INSTANCES)
    uint8_t instanceNumber;

    for (instanceNumber = 0; instanceNumber < g_parameter.numberOfInstances; instanceNumber++)
        initResetInstance(instanceNumber);

#endif
    ResetDevice();
}

void PowerOnState(void)
{
    F_receiveFinish = 0;
    F_pollingInstance = 0;
    F_checkCollision = CHECK_COLLISION;
    F_checkAnsCollision = CHECK_ANS_COLLISION;
    F_manuCmdMode = MANU_CMD_MODE;

#if(NUMBER_OF_INSTANCES)
    uint8_t instanceNumber;

    for (instanceNumber = 0; instanceNumber < g_parameter.numberOfInstances; instanceNumber++)
        initPowerOnInstance(instanceNumber);

    g_parameter.codeDelay = CODE_DELAY_CONPENSATION_FOR_DEAD_TIME;

#endif
    PowerOnDevice();
}

void DefaultDevice(void)
{
    var_shortAddress = MASK_u8;
    var_deviceGroups = 0;
    var_randomAddress = 0xFFFFFF;
    var_operatingMode = 0;
    g_parameter.flag.f_applicationEnable = APPLICATION_CONTROLLER_PRESENT;
    var_applicationActive = APPLICATION_CONTROLLER_PRESENT;
    sta_applicationActive = (var_applicationActive > 0) ? TRUE : FALSE;
    var_powerCycleNotification = FALSE;
    sta_resetState = TRUE;
}

void ResetDevice(void)
{
    var_deviceGroups = 0;
    var_searchAddress = 0xFFFFFF;
    var_randomAddress = 0xFFFFFF;
    sta_quiescentMode = DISABLE;
    F_writeEnableState = DISABLE;
    sta_powerCycleSeen = FALSE;
    sta_applicationControllerError = FALSE;
    sta_inputDeviceError = FALSE;
    sta_resetState = TRUE;
    sta_applicationActive = (var_applicationActive > 0) ? TRUE : FALSE;
}

void PowerOnDevice(void)
{
    var_searchAddress = 0xFFFFFF;
    var_DTR0 = 0;
    var_DTR1 = 0;
    var_DTR2 = 0;
    sta_quiescentMode = DISABLE;
    F_writeEnableState = DISABLE;
    sta_powerCycleSeen = TRUE;
    var_initialisationState = DISABLE;
    sta_applicationControllerError = FALSE;
    sta_inputDeviceError = FALSE;
    sta_resetState = TRUE;
    g_parameter.flag.f_applicationEnable = APPLICATION_CONTROLLER_PRESENT;
    var_powerEventTime = 0;
    sta_applicationActive = (var_applicationActive > 0) ? TRUE : FALSE;
    F_newPowerCycleNotice = (var_powerCycleNotification > 0) ? TRUE : FALSE;
    g_parameter.newPowerOnCounter = 0;
}

#if(USE_PART103_INPUT_DEVICE)
void DefaultInstance103(uint8_t instanceNumber)
{
    instanceType(instanceNumber) = PART103;
    resolution(instanceNumber) = 3;
    extendedVersionNumber(instanceNumber) = Version(2, 0);
    instanceActive(instanceNumber) = TRUE;
    instanceGroup(instanceNumber, 0) = MASK_u8;
    instanceGroup(instanceNumber, 1) = MASK_u8;
    instanceGroup(instanceNumber, 2) = MASK_u8;
    eventScheme(instanceNumber) = 0;
    eventPriority(instanceNumber) = 4;
    eventFilter(instanceNumber) = 0xFFFFFF;
}

void ResetInstance103(uint8_t instanceNumber)
{
    instanceGroup(instanceNumber, 0) = MASK_u8;
    instanceGroup(instanceNumber, 1) = MASK_u8;
    instanceGroup(instanceNumber, 2) = MASK_u8;
    eventFilter(instanceNumber) = 0xFFFFFF;
    eventScheme(instanceNumber) = 0;
    instanceError(instanceNumber) = FALSE;
}

void PowerOnInstance103(uint8_t instanceNumber)
{
    instanceError(instanceNumber) = FALSE;
}
#endif

