#include "user.h"


uint8_t ID;
int16_t Acc[3];
int16_t Gyo[3];
int16_t Mag[3];
float Eular[3];
float Quat[4];
int32_t Pressure;


uint8_t gl_GPSInf[10] = {0,0,0,0,0,0,0,0,0,0};
uint16_t gl_jsflag[5] = {0,0,0,0,0};
char Depth_Buff[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t gl_flag = 0;
uint8_t Motor_Init[4] = {100,100,100,100};  //based on the force of the 1st motor.
uint8_t Buff_Real[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t Control_Time = 10;
uint8_t	Buff_Total[200][20];
uint8_t gl_TimeFlag[5]  = {0,0,0,0,0};
uint8_t	gl_TempFlag[10] = {0,0,0,0,0,0,0,0,0,0};
uint8_t gl_Dis[2] = {0,0};
float depth = 0.0;
float	AHRS_Data[12]={0,0,0,0,0,0,0,0,0,0,0,0};  //yaw,pitch,roll,ax ay az gx gy gz ????
uint8_t gl_MotionFlag = 0;
int Force[4] = {0,0,0,0};
uint16_t SPI_Tx_Buffer[10]= {0,1,2,3,4,5,6,7,8,9};//???????
uint16_t SPI_Rx_Buffer[10]= {9,8,7,6,5,4,3,2,1,0};//???????
float KP[3] = {0,0,0};
float KI[3] = {0,0,0};
float KD[3] = {0,0,0};
uint16_t PC_Rx_Buffer[RX_BUFFER_SIZE]; //???????
uint16_t AHRS_Rx_Buffer[RX_BUFFER_SIZE]; //???????
uint16_t gl_FlagMotionCycle = 0;
uint16_t Real_PcBuff[RX_BUFFER_SIZE] = {0};
uint16_t Real_AHRSBuff[RX_BUFFER_SIZE] = {0};
uint16_t AHRS_RX_Flag = 0;
uint16_t PC_RX_Flag = 0;
uint16_t AUV_State = 1;
uint16_t Last_AUV_State = 1;

int main(void)
{
        SystemInitUser();
        //	Test();
        //	while(1){}



        while (1)
        {
                DataInter_GPS(); 
                DataInter_AHRS();
                DataInter_Pc();
                if(AUV_State != 1)
                {
                        Motion_Control();
                }

        }
}








