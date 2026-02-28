#ifndef __ALS_H__
#define __ALS_H__
#include <stdint.h>
#include <stdbool.h>
typedef union
{
    uint16_t Data; //1*4
    struct
    {
        uint8_t Data_L : 8;
        uint8_t Data_H : 8;
    } info;
} ALS_Channel;

extern volatile ALS_Channel ALS_CH0, ALS_CH1;
extern volatile uint16_t Lux_Level;
extern unsigned char F_Get_ALS;
extern uint8_t ALS_Counter;
extern uint16_t FB_GET_LUX_Method_BH1730FVC(unsigned short, unsigned short);
extern uint32_t Get_ALS(void);
#endif //__ALS_H__

