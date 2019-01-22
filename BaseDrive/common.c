/*
 * USER_Common.c
 *
 *  Created on: 2016-5-16
 *      Author: koukou
 */
#include "common.h"



extern void Para_Init(void)
{
        uint8_t i = 0,j = 0;
        for(i = 0; i<100; i++)
        {
                Real_PcBuff[i] = 0;
                Real_AHRSBuff[i] = 0;
        }

        for(i=0;i<4;i++)
        {
                Force[i] = 0;
        }

        for(i = 0;i<200;i++)
        {
                for(j = 0;j<18;j++)
                {
                        Buff_Total[i][j] = 0;
                }
        }


}

extern void Store_PcData(uint16_t* Data_To,uint16_t* Data2_From)
{
        uint16_t i = 0;
        for(i = 0;i<Data2_From[0] - 2;i++)
        {
                Data_To[i] = Data2_From[i];
        }
}

extern void Store_SensorData(void)
{
        ////	uint16_t i = 0;
        ////	uint8_t j = 0,k = 0;
        ////	int16 temp = 0;
        ////	for(i = 0;i < 36;i++)
        ////	{
        ////		Real_AHRSBuff[i] = AHRS_Rx_Buffer[i];
        ////	}
        //	Uint8 	*p = RxPkt.Buffer;
        //	Uint8 	j = 0,k = 0;
        //	uint16_t 	temp = 0;
        //	for(j = 0;j < 28;j++)
        //	{
        //		Real_AHRSBuff[j] = p[j];
        //	}
        //	k=1;
        //	for(j=0; j<3;j++)
        //	{
        //		temp =  Real_AHRSBuff[k] + (Real_AHRSBuff[k+1]<<8);
        //		if (temp&0x8000)
        //		{
        //			temp = (temp-1)^ 0x7FFF;
        //		}
        //		Buff_Real[k-1] = (temp & 0xFF00) >> 8;
        //		Buff_Real[k] = temp & 0x00FF;
        //		k=k+2;
        //	}
        //	temp =  Real_AHRSBuff[22] + (Real_AHRSBuff[23]<<8);  //pitch
        //	if (temp&0x8000)
        //	{
        //		temp = (temp-1)^ 0x7FFF;
        //	}
        //	Buff_Real[8] = (temp & 0xFF00) >>8;
        //	Buff_Real[9] = temp & 0x00FF;
        //
        //	temp =  Real_AHRSBuff[24] + (Real_AHRSBuff[25]<<8);  //roll
        //	if (temp&0x8000)
        //	{
        //		temp = (temp-1)^ 0x7FFF;
        //	}
        //	Buff_Real[6] = (temp & 0xFF00) >> 8;
        //	Buff_Real[7] = temp & 0x00FF;
        //
        //
        //	temp = (int16)(Real_AHRSBuff[26] + (Real_AHRSBuff[27]<<8)) + 1800;  //yaw
        //	Buff_Real[10] = (temp & 0xFF00) >>8;
        //	Buff_Real[11] = temp & 0x00FF;
        //
        //	temp =  Real_AHRSBuff[11] + (Real_AHRSBuff[12]<<8);  //gz
        //	if (temp&0x8000)
        //	{
        //		temp = (temp-1)^ 0x7FFF;
        //	}
        //	Buff_Real[12] = (temp & 0xFF00) >>8;
        //	Buff_Real[13] = temp & 0x00FF;

        //	Buff_Real[0] = Real_AHRSBuff[18]; //the acceleration of x-axis
        //	Buff_Real[1] = Real_AHRSBuff[19];
        //	Buff_Real[2] = Real_AHRSBuff[20]; //the acceleration of y-axis
        //	Buff_Real[3] = Real_AHRSBuff[21];
        //	Buff_Real[4] = Real_AHRSBuff[22]; //the acceleration of z-axis
        //	Buff_Real[5] = Real_AHRSBuff[23];
        //
        //	Buff_Real[6] = Real_AHRSBuff[6]; //roll angle
        //	Buff_Real[7] = Real_AHRSBuff[7];
        //	Buff_Real[8] = Real_AHRSBuff[4]; //pitch angle
        //	Buff_Real[9] = Real_AHRSBuff[5];
        //	Buff_Real[10] = Real_AHRSBuff[2]; //yaw
        //	Buff_Real[11] = Real_AHRSBuff[3];
        //
        //	Buff_Real[12] = Real_AHRSBuff[28]; //Gz
        //	Buff_Real[13] = Real_AHRSBuff[29];
        //	if(acc[0] & 0x8000)
        //	{
        //		temp = (temp-1)^ 0x7FFF;
        //	}
        //	else
        //	{
        //		Buff_Real[0] = (acc[0] & 0xFF00) >>8;
        //		Buff_Real[1] =  acc[0] & 0x00FF;
        //	}
        Eular[0] = Eular[0] + 90;
        Buff_Real[6] =  (int)Eular[0] / 256;
        Buff_Real[7] =  (int)Eular[0] % 256;

        Eular[1] = Eular[1] + 90;
        Buff_Real[8] =  (int)Eular[1] / 256;
        Buff_Real[9] =  (int)Eular[1] % 256;

        Eular[2] = Eular[2] + 180;
        Buff_Real[10] =  (int)Eular[2] / 256;
        Buff_Real[11] =  (int)Eular[2] % 256;



        //	Buff_Real[14] = (uint8_t)(Force[0]+100);
        //	Buff_Real[15] = (uint8_t)(Force[1]+100);
        //	Buff_Real[16] = (uint8_t)(Force[2]+100);
        //	Buff_Real[17] = (uint8_t)(Force[3]+100);
        //
        Buff_Real[18] = (int)depth / 256;
        Buff_Real[19] = (int)depth % 256;
        //
        //	temp = (Real_AHRSBuff[2] <<8 ) + Real_AHRSBuff[3];    //yaw ?
        //	Real_Yaw = (float)temp/10;
        //
        //	//store the yaw, pitch and roll angle into the AHRS_Data[0],[1],[2]
        //	k = 2;
        //	for(j=0;j<=2;j++)
        //	{
        //		temp = 0;
        //		temp = (Real_AHRSBuff[k] <<8 ) + Real_AHRSBuff[k+1];
        //		if(temp&0x8000)
        //		{
        //			temp = 0 - temp&0x7fff;
        //		}
        //		else
        //		{
        //			temp = (temp&0x7fff);
        //		}
        //		AHRS_Data[j]=(float)temp/10;
        //		k = k+2;
        //	}
        //
        //
        //	//18 3 20 4 22 5 24 6 26 7 28 8 30 9 32 10 34 11
        //	/*store the ax, ay, az into the AHRS_Data[3],[4],[5]
        //	 *store the GYx, GYy, GYz into the AHRS_Data[6],[7],[8]
        //	 *store the Mx, My, Mz into the AHRS_Data[9],[10],[11]
        //	 */
        //	k = 18;
        //	for(j=3;j<=11;j++)
        //	{
        //		temp = 0;
        //		temp = (Real_AHRSBuff[k] <<8 ) + Real_AHRSBuff[k+1];    //ax ?
        //		if(temp&0x8000)
        //		{
        //			temp = 0 - temp&0x7fff;
        //		}
        //		else
        //		{
        //			temp = (temp&0x7fff);
        //		}
        //		AHRS_Data[j]=(float)temp;
        //		k = k+2;
        //	}
}
