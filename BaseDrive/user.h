#ifndef USER_H
#define USER_H

#ifdef __cplusplus
extern "C" {
#endif
	



	
//#define DEBUG  1
#define  RX_BUFFER_SIZE 100 

/**************system include ******************/
#include "stm32f4xx.h"
#include <stdio.h>
#include "string.h"
#include <stdlib.h>

typedef struct{
	int8_t data[11];
	int data1[3];
}SLiding_Para;//???????

/***************user-define include*************/
#include "led.h"
#include "test.h"
#include "usart.h"
#include "test.h"
#include "timer.h"
#include "packet.h"
#include "imu_data_decode.h"
#include "common.h"
#include "motor.h"
#include "slidingmode.h"
#include "adc.h"
#include "gps.h"  


extern	uint8_t ID;
extern	int16_t Acc[3];
extern	int16_t Gyo[3];
extern  int16_t Mag[3];
extern	float Eular[3];
extern	float Quat[4];
extern	int32_t Pressure;

extern uint16_t gl_jsflag[5];
extern char Depth_Buff[20];
extern uint8_t gl_flag;
extern uint8_t Motor_Init[4];
extern uint8_t  Buff_Real[20];
extern uint8_t  Control_Time;
extern uint8_t	Buff_Total[200][20];
extern uint8_t 	gl_TimeFlag[5];
extern uint8_t	gl_TempFlag[10];
extern uint8_t 	gl_Dis[2];
extern float depth;
extern float AHRS_Data[12];
extern uint8_t gl_MotionFlag;
extern int Force[4];
extern float KP[3];
extern float KI[3];
extern float KD[3];
extern uint16_t AHRS_Rx_Buffer[RX_BUFFER_SIZE]; //???????
extern uint16_t PC_Rx_Buffer[RX_BUFFER_SIZE]; //???????
extern uint16_t gl_FlagMotionCycle;
extern uint16_t Real_PcBuff[100];
extern uint16_t Real_AHRSBuff[100];
extern uint16_t AHRS_RX_Flag;
extern uint16_t PC_RX_Flag;
extern uint16_t AUV_State;
extern uint16_t Last_AUV_State;



#ifdef __cplusplus
}
#endif
#endif
