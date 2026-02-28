#include "dali_cmd_103.h"
#include "dali_cd_app.h"

//Replace all standard commands and bank related commands exclude three commands below
void manufacturerMode(uint8_t address, uint8_t instance, uint8_t opcode)
{
    if (((var_operatingMode >= 0x80) && (var_operatingMode <= 0xFF)) // Operatimg mode can only be set to 0x80 ~ 0xFF. User can set conditions here to make sure the system enter the right operating mode.
            || ((F_manuCmdMode == 0) && (instance == 0xFE) && ((opcode == SET_OPERATING_MODE) || (opcode == QUERY_OPERATING_MODE) || (opcode == QUERY_MANUFACTURER_SPECIFIC_MODE))))
    {
        switch (opcode)
        {
            /* User can add commands that is not reserved in stardard here. */

            /* MUST keep these three commands below*/
            case SET_OPERATING_MODE:
                if (checkSendTwice(address, instance, opcode))
                {
                    if ((checkSupportedOperatingMode(var_DTR0)) && (var_DTR0 != var_operatingMode))
                    {
                        var_operatingMode = var_DTR0;
                        write_variable_once((uint8_t *)&var_operatingMode, 1);
                    }
                }

                break;

            case QUERY_OPERATING_MODE:
                sendData(0) = var_operatingMode;
                sendFrame_ISR(8, 0);
                break;

            case QUERY_MANUFACTURER_SPECIFIC_MODE:
                if ((var_operatingMode >= 0x80) && (var_operatingMode <= MASK_u8))
                {
                    sendData(0) = MASK_u8;
                    sendFrame_ISR(8, 0);
                }

                break;

            default:
                break;
        }
    }
}

uint8_t checkSupportedOperatingMode(uint8_t dtr0)
{
    if (dtr0 == 0) //Customer define. Check if the DTR0's value be in the range of supported operating modes
        return 1;
    else
        return 0;
}

