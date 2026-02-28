#include <stdio.h>
#include "NuMicro.h"
#include "ALS.h"

#define Ratio_threshold_Max_Number  13
#define ALS_Cal_index_Max           20
//BH1730FVC Lux register
#define D_BH1730FVC_Ratio_Max       546   //datasheet max 2.13 ==> *256 = 546
#define D_Over_Max_Value            60000
#define D_Half_Max_Value            1000
#define D_Ratio_L                   2   //<100
#define D_Ratio_M                   2   //>=100, <200
#define D_Ratio_H                   2   //>=200
#define D_Min_Value                 1
#define D_Max_Value                 60000

#define D_MAX_LUX                   1023 //2^10 - 1 = 1023
#define D_I2C_BH1730FVC_PART_NUMBE  0x71    //0x71, PRAT ID = 0x7, Revision ID = 0x1

#define D_RAM_ALS_Data_Size 80

unsigned char b_BH1730FVC_OverStatus_Flag;  //Set to 1 when value overflow or status change after recover. Set value after ALS Reset
uint32_t B_Const_A, B_Const_B;
volatile uint16_t Lux_Level = 0;
unsigned char F_Get_ALS;
volatile ALS_Channel ALS_CH0, ALS_CH1;
uint8_t ALS_Counter = 0;

unsigned short Ratio_threshold[Ratio_threshold_Max_Number] =
{
    0,  //#define D_Ratio_threshold_0  0
    20, //#define D_Ratio_threshold_1  20
    30, //#define D_Ratio_threshold_2  30
    40, //#define D_Ratio_threshold_3  40
    50, //#define D_Ratio_threshold_4  50
    60, //#define D_Ratio_threshold_5  60
    70, //#define D_Ratio_threshold_6  70
    80, //#define D_Ratio_threshold_7  80
    90, //#define D_Ratio_threshold_8  90
    100,//#define D_Ratio_threshold_9  100
    110,//#define D_Ratio_threshold_10 110
    120,//#define D_Ratio_threshold_11 120
    130 //#define D_Ratio_threshold_12 130
};

unsigned char ALS_Cal_index[ALS_Cal_index_Max] =
{
    0,  //0
    4,  //1
    8,  //2
    12, //3
    16, //4
    20, //5
    24, //6
    28, //7
    32, //8
    36, //9
    40, //10
    44, //11
    48, //12
    52, //13
    56, //14
    60, //15
    64, //16
    68, //17
    72, //18
    76  //19
};

unsigned char RAM_ALS_Cal_Data[D_RAM_ALS_Data_Size] =
{
    6, 00, 0, 01, //D_Group_0 0~3,    BH1730FVC, <  D_Ratio_threshold_1
    4, 92, 0, 01, //D_Group_1 4~7,    BH1730FVC, >= D_Ratio_threshold_2,  < D_Ratio_threshold_3
    3, 87, 0, 01, //D_Group_2 8~11,   BH1730FVC, >= D_Ratio_threshold_3,  < D_Ratio_threshold_4
    3, 85, 0, 01, //D_Group_3 12~15,  BH1730FVC, >= D_Ratio_threshold_4,  < D_Ratio_threshold_5
    3, 55, 0, 01, //D_Group_4 16~19,  BH1730FVC, >= D_Ratio_threshold_5,  < D_Ratio_threshold_6
    3, 00, 0, 01, //D_Group_5 20~23,  BH1730FVC, >= D_Ratio_threshold_6,  < D_Ratio_threshold_7
    3, 00, 0, 01, //D_Group_6 24~27,  BH1730FVC, >= D_Ratio_threshold_7,  < D_Ratio_threshold_8
    3, 00, 0, 01, //D_Group_7 28~31,  BH1730FVC, >= D_Ratio_threshold_8,  < D_Ratio_threshold_9
    2, 80, 0, 01, //D_Group_8 32~35,  BH1730FVC, >= D_Ratio_threshold_9,  < D_Ratio_threshold_10
    2, 78, 0, 01, //D_Group_9 36~39,  BH1730FVC, >= D_Ratio_threshold_10, < D_Ratio_threshold_11
    2, 68, 0, 01, //D_Group_10 40~43, BH1730FVC, >= D_Ratio_threshold_11, < D_Ratio_threshold_12
    2, 68, 0, 01, //D_Group_11 44~47, BH1730FVC, >= D_Ratio_threshold_12, < D_Ratio_threshold_13
    2, 68, 0, 01, //D_Group_12 48~51, BH1730FVC, no use
    2, 68, 0, 01, //D_Group_13 52~55, BH1730FVC, no use
    2, 68, 0, 01, //D_Group_14 56~59, BH1730FVC, no use
    2, 68, 0, 01, //D_Group_15 60~63, BH1730FVC, no use
    2, 68, 0, 01, //D_Group_16 64~67, BH1730FVC, no use
    2, 68, 0, 01, //D_Group_17 68~71, BH1730FVC, no use
    2, 68, 0, 01, //D_Group_18 72~75, BH1730FVC, no use
    2, 68, 0, 01, //D_Group_19 76~79, BH1730FVC, no use
};

void F_Check_ALS_Group(unsigned char B_Start_Address)
{
    B_Const_A = RAM_ALS_Cal_Data[B_Start_Address] * 100 + RAM_ALS_Cal_Data[B_Start_Address + 1];
    B_Const_B = RAM_ALS_Cal_Data[B_Start_Address + 2] * 100 + RAM_ALS_Cal_Data[B_Start_Address + 3];
}

uint16_t FB_GET_LUX_Method_BH1730FVC(uint16_t B_A_CH0, uint16_t B_A_CH1)
{
    unsigned char B_BH1730FVC_Ratio_before; //Ratio
    unsigned char B_Ratio_index;
    unsigned short B_Max_Threshold;
    unsigned short B_Min_Threshold;
    unsigned long B_tempx, B_tempy;
    unsigned long B_BH1730FVC_Ratio;
    unsigned long B_lux;
    unsigned short B_LUX_INTERGER;

    //first cal ratio
    B_BH1730FVC_Ratio = ((unsigned long)B_A_CH1 << 8) / B_A_CH0;  // ratio = (ch1 * 256) / ch0

    if ((B_A_CH0 >= D_Min_Value) && (B_A_CH0 < D_Max_Value) && (B_A_CH1 >= D_Min_Value) && (B_A_CH1 < D_Max_Value))
    {
        if (B_BH1730FVC_Ratio <= D_BH1730FVC_Ratio_Max)
        {
            if (B_BH1730FVC_Ratio < Ratio_threshold[1])
            {
                //Min
                F_Check_ALS_Group(ALS_Cal_index[0]); //default Group = 0

                if (B_BH1730FVC_Ratio > (Ratio_threshold[1] - D_Ratio_L))
                {
                    if (B_BH1730FVC_Ratio_before == ALS_Cal_index[1])
                    {
                        F_Check_ALS_Group(ALS_Cal_index[1]); //Change to Group = 1
                        B_BH1730FVC_Ratio_before = ALS_Cal_index[1];
                    }
                }
                else
                {
                    B_BH1730FVC_Ratio_before =  ALS_Cal_index[0]; //default Group = 0
                }
            }
            else
            {
                //Between Min and Max
                if (B_BH1730FVC_Ratio < Ratio_threshold[Ratio_threshold_Max_Number - 1])
                {
                    B_Ratio_index = 1;

                    do
                    {
                        B_Max_Threshold = Ratio_threshold[B_Ratio_index + 1];
                        B_Min_Threshold = Ratio_threshold[B_Ratio_index];

                        if ((B_Min_Threshold <= B_BH1730FVC_Ratio) && (B_BH1730FVC_Ratio < B_Max_Threshold))
                        {
                            break;
                        }
                        else
                        {
                            B_Ratio_index++;
                        }
                    }

                    while (B_Ratio_index < Ratio_threshold_Max_Number);

                    F_Check_ALS_Group(ALS_Cal_index[B_Ratio_index]);//default Group

                    B_Max_Threshold = Ratio_threshold[B_Ratio_index + 1];
                    B_Min_Threshold = Ratio_threshold[B_Ratio_index];

                    if (B_BH1730FVC_Ratio > (B_Max_Threshold - D_Ratio_L))
                    {
                        if (B_BH1730FVC_Ratio_before == ALS_Cal_index[B_Ratio_index + 1])
                        {
                            F_Check_ALS_Group(ALS_Cal_index[B_Ratio_index + 1]); //Change Group
                            B_BH1730FVC_Ratio_before = ALS_Cal_index[B_Ratio_index + 1];
                        }
                    }
                    else if (B_BH1730FVC_Ratio < (B_Min_Threshold + D_Ratio_L))
                    {
                        if (B_BH1730FVC_Ratio_before == ALS_Cal_index[B_Ratio_index - 1])
                        {
                            F_Check_ALS_Group(ALS_Cal_index[B_Ratio_index - 1]); //Change Group
                            B_BH1730FVC_Ratio_before = ALS_Cal_index[B_Ratio_index - 1];
                        }
                    }
                    else
                    {
                        B_BH1730FVC_Ratio_before = ALS_Cal_index[B_Ratio_index]; //default Group
                    }
                }
                else
                {
                    //Max Limit
                    F_Check_ALS_Group(ALS_Cal_index[10]);  // >= D_Ratio_threshold_9 //default Group = 10

                    if (B_BH1730FVC_Ratio < (Ratio_threshold[10] + D_Ratio_H))
                    {
                        if (B_BH1730FVC_Ratio_before == ALS_Cal_index[9])
                        {
                            F_Check_ALS_Group(ALS_Cal_index[9]);//Change to Group = 9
                            B_BH1730FVC_Ratio_before = ALS_Cal_index[9];
                        }
                    }
                    else
                    {
                        B_BH1730FVC_Ratio_before = ALS_Cal_index[10];//default Group = 10
                    }
                }
            }

            B_tempx = (B_Const_A) * (B_A_CH0);
            B_tempy = (B_Const_B) * (B_A_CH1);

            if (B_tempx >= B_tempy)
            {
                B_lux = (B_tempx - B_tempy) >> 8;  //(ch0 * x - ch1 * y) / 256
            }
        }
    }
    else
    {
        if ((B_A_CH0 <= D_Min_Value) || (B_A_CH1 <= D_Min_Value))
        {
            B_lux = 0;
        }

        if ((B_A_CH0 >= D_Max_Value) || (B_A_CH1 >= D_Max_Value))
        {
            B_lux = D_MAX_LUX;
        }
    }

    if (B_lux > D_MAX_LUX)
    {
        B_lux = D_MAX_LUX;
    }

    B_LUX_INTERGER = (unsigned short)B_lux;
    return B_LUX_INTERGER;
}

uint32_t Get_ALS(void)
{
    F_Get_ALS = 0;
    // Get ALS Data from I2C
    ALS_CH0.info.Data_L = I2C_ReadByteOneReg(I2C1, 0x39, 0x94);
    ALS_CH0.info.Data_H = I2C_ReadByteOneReg(I2C1, 0x39, 0x95);
    ALS_CH1.info.Data_L = I2C_ReadByteOneReg(I2C1, 0x39, 0x96);
    ALS_CH1.info.Data_H = I2C_ReadByteOneReg(I2C1, 0x39, 0x97);
    return FB_GET_LUX_Method_BH1730FVC(ALS_CH0.Data, ALS_CH1.Data);
}

