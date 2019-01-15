#ifndef MOTOR_H
#define MOTOR_H



#include "user.h"




#define	MAX_YAWF	200
#define	MIN_YAWF	-200

extern void Set_PID(void);
extern void SingleMotor_Control(void);
extern void TotalMotor_Control(void);
extern void Motion_Type(void);
extern void Motion_Control(void);

static void Advance_Control(void);
static void Back_Control(void);
static void Up_Control(void);
static void Down_Control(void);
static void ShangYang_Control(void);
static void XiaYang_Control(void);
static void Right_Control(void);
static void Left_Control(void);
static void Stop_Control(void);
static int Yaw_Control(void);
static uint16_t Num_Change(uint16_t s,uint8_t n);
static int	Forcez_Change(uint8_t s);
static int	Forcex_Change(uint8_t s);
static void Saturation(uint16_t *temp,uint16_t min,uint16_t max);
extern void Store_Data(void);
extern void Store_Data1(void);
extern void Store_Data2(void);
extern void ReadMotorSpeed(uint8_t i);
void MOTOR_STOP(void);
void Motor_InitSet(void);
void Motor14_RelayOn(void);
void Motor23_RelayOn(void);


#endif
