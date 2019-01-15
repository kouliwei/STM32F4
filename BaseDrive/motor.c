#include "motor.h"

unsigned int number=0;
unsigned int speed=0;
unsigned int motion=0;
float error_sum = 0;
float debug = 0.0;

const uint8_t rotorD1[4] = {60,60,60,60};
const uint8_t rotorD2[4] = {120,120,120,120};

// AUV reference parameter
float Ref_Yaw = 0;
uint16_t Ref_Speed = 0;
uint16_t Ref_Depth = 0;

extern void Set_PID(void)
{
	uint8_t slPID = 0;
	slPID = Real_PcBuff[2];
	switch(slPID){
			case 0:
				KP[0] = (float)Real_PcBuff[3]/10.0;  //KP Range:0-25.5
				KI[0] = (float)Real_PcBuff[4]/100.0; //KI Range:0-2.55
				KD[0] = (float)Real_PcBuff[5];		 //KD Range:0-255
				break;
			case 1:
				KP[1] = (float)Real_PcBuff[3]/10.0;
				KI[1] = (float)Real_PcBuff[4]/100.0;
				KD[1] = (float)Real_PcBuff[5];
				break;
			case 2:
				KP[2] = (float)Real_PcBuff[3]/10.0;
				KI[2] = (float)Real_PcBuff[4]/100.0;
				KD[2] = (float)Real_PcBuff[5];
				break;
			default: break;
	}
	UART_Send();
}

extern void SingleMotor_Control(void)
{
//	int temp = 0;
//	for(temp = -300; temp < 600;temp++)
//	{
//		if(temp == -140) temp = 140;
//		EPwmSetup1(3500+temp);
//		Delay_Nms(3);
//	}
//	EPwmSetup1(3500);
	uint16_t position = 0,speed = 0,sp[4] = {0},i=0;
	position = Real_PcBuff[2];
	speed  = Real_PcBuff[3];
	for(i = 0;i<4;i++) 
	{
		sp[i] = Num_Change(speed,i);
	}

	switch(position)
	{
		case 0x01:
			EPwmSetup1(sp[0]);
			EPwmSetup2(PWM_STOP);
			EPwmSetup3(PWM_STOP);
			EPwmSetup4(PWM_STOP);
			break;
		case 0x02:
			EPwmSetup2(sp[1]);
			EPwmSetup1(PWM_STOP);
			EPwmSetup3(PWM_STOP);
			EPwmSetup4(PWM_STOP);

			break;
		case 0x03:
			EPwmSetup3(sp[2]);
			EPwmSetup1(PWM_STOP);
			EPwmSetup2(PWM_STOP);
			EPwmSetup4(PWM_STOP);
			break;
		case 0x04:
			EPwmSetup4(sp[3]);
			EPwmSetup1(PWM_STOP);
			EPwmSetup2(PWM_STOP);
			EPwmSetup3(PWM_STOP);
			break;
		default :
			EPwmSetup1(sp[0]);
			EPwmSetup2(PWM_STOP);
			EPwmSetup3(PWM_STOP);
			EPwmSetup4(PWM_STOP);
			break;
	}
	AUV_State = 1;
	UART_Send(); //A5 5A 06 01 01 32 38 AA //A5 5A 06 01 02 64 6D AA //A5 5A 06 01 03 00 0A AA // A5 5A 06 01 04 19 24 AA
}


extern void TotalMotor_Control(void)
{
	uint16_t rotor[4] = {0,0,0,0},i = 0;
	for(i = 0; i < 4;i++)
	{
		rotor[i] = Num_Change(Real_PcBuff[i+2],i);
	}

	EPwmSetup1(rotor[0]);
	EPwmSetup2(rotor[1]);
	EPwmSetup3(rotor[2]);
	EPwmSetup4(rotor[3]);
	AUV_State = 1;
	UART_Send();
}

void XiangyangParastore(void)
{
	uint16_t temp = 0;
	temp = (Real_PcBuff[5] <<8 ) + Real_PcBuff[6];    //yaw ?
	Saturation(&temp,0,360);
	Ref_Yaw = (float)temp;
	Ref_Speed = Real_PcBuff[3];
	Ref_Depth = Real_PcBuff[4];
	Control_Time = Real_PcBuff[7];
	gl_jsflag[1] = 0;
}

extern void Motion_Type(void)
{
	uint16_t type = 0;
	type = Real_PcBuff[2];
	if(AUV_State != 10 && AUV_State != 1)
	{
		Last_AUV_State = AUV_State;
	}
	switch(type)
	{
	case 0x36://qianjin
		AUV_State = 2;
		break;
	case 0x34://houtui
		AUV_State = 3;
		break;
	case 0x38://shang
		AUV_State = 4;
		break;
	case 0x32://xia
		AUV_State = 5;
		break;
	case 0x31://shangyang,henggunzuoxian
		AUV_State = 6;
		break;
	case 0x33://xiayang,henggunyouxian
		AUV_State = 7;
		XiangyangParastore();
		break;
	case 0x39://youzhuan
		AUV_State = 8;
		break;
	case 0x37://zuozhuan
		AUV_State = 9;
		break;
	case 0x35://??
		AUV_State = 10;
		break;
	}
	UART_Send();
}

extern void Motion_Control(void)
{
	while(gl_FlagMotionCycle == 1)
	{
		gl_FlagMotionCycle = 0;
		switch(AUV_State)
		{
		case 2:
			Advance_Control();
			break;
		case 3:
			Back_Control();
			break;
		case 4:
			Up_Control();
			break;
		case 5:
			Down_Control();
			break;
		case 6:
			ShangYang_Control();
			break;
		case 7:
			XiaYang_Control();
			break;
		case 8:
			Right_Control();
			break;
		case 9:
			Left_Control();
			break;
		case 10:
			Stop_Control();
			break;
		}
	}
}


static void Advance_Control(void)
{
	uint16_t Temp1[4]={0,0,0,0};
	if(gl_flag)
	{
		Temp1[0] = EPwmRead(1);
		Temp1[1] = EPwmRead(2);
		Temp1[2] = EPwmRead(3);
		Temp1[3] = EPwmRead(4);


		Temp1[0] = Temp1[0] + 15;
		if(Temp1[0] <PWM_STOP+200 && Temp1[0]  > PWM_STOP-200 )
		{
			Temp1[0] = PWM_STOP + 200;
		}
		Temp1[1] = Temp1[1] - 15;
		if(Temp1[1] <PWM_STOP+200 && Temp1[1]  > PWM_STOP-200 )
		{
			Temp1[1] = PWM_STOP - 200;
		}

		Temp1[2] = Temp1[2] +15;
		if (Temp1[2] <PWM_STOP+200 && Temp1[2]  > PWM_STOP-200 )
		{
			Temp1[2] = PWM_STOP+200;
		}

		Temp1[3] = Temp1[3] -15;
		if (Temp1[3] <PWM_STOP+200 && Temp1[3]  > PWM_STOP-200 )
		{
			Temp1[3] = PWM_STOP-200;
		}
		
		EPwmSetup1(Temp1[0]);
		EPwmSetup2(Temp1[1]);
		EPwmSetup3(Temp1[2]);
		EPwmSetup4(Temp1[3]);
		gl_flag = 0;

	}

}
static void Back_Control(void)
{
	uint16_t Temp1[4]={0,0,0,0};
	if(gl_flag)
	{
		Temp1[0] = EPwmRead(1);
		Temp1[1] = EPwmRead(2);
		Temp1[2] = EPwmRead(3);
		Temp1[3] = EPwmRead(4);


		Temp1[0] = Temp1[0] - 15;
		if(Temp1[0] <PWM_STOP+200 && Temp1[0]  > PWM_STOP-200 )
		{
			Temp1[0] = PWM_STOP - 200;
		}
		Temp1[1] = Temp1[1] + 15;
		if(Temp1[1] <PWM_STOP+200 && Temp1[1]  > PWM_STOP-200 )
		{
			Temp1[0] = PWM_STOP + 200;
		}

		Temp1[2] = Temp1[2] + 15;
		if (Temp1[2] <PWM_STOP+200 && Temp1[2]  > PWM_STOP-200 )
		{
			Temp1[2] = PWM_STOP-200;
		}

		Temp1[3] = Temp1[3] + 15;
		if (Temp1[3] <PWM_STOP+200 && Temp1[3]  > PWM_STOP-200 )
		{
			Temp1[3] = PWM_STOP+200;
		}

		EPwmSetup1(Temp1[0]);
		EPwmSetup2(Temp1[1]);
		EPwmSetup3(Temp1[2]);
		EPwmSetup4(Temp1[3]);
		gl_flag = 0;
	}
}
static void Up_Control(void)
{
//	Store_Data2();
		EPwmSetup1(PWM_STOP + 250);
		EPwmSetup2(PWM_STOP - 250);
		EPwmSetup3(PWM_STOP + 250);
		EPwmSetup4(PWM_STOP - 250);

}



static void Down_Control(void)
{

		EPwmSetup1(PWM_STOP - 250);
		EPwmSetup2(PWM_STOP + 250);
		EPwmSetup3(PWM_STOP - 250);
		EPwmSetup4(PWM_STOP + 250);
}


static void ShangYang_Control(void)
{

}


static int Yaw_Control(void)
{
	float u_temp = 0, error = 0;
	static	float last_error = 0;
	error = Ref_Yaw - Eular[2];


	if(error < -180)
	{
		error = error + 360;
	}
	else if(error > 180)
	{
		error = error - 360;
	}
//	error = error/18.0;  //???  ??-10  ~  10???
	//????

	if(error > -90 && error < 90)  //??-45? ~45?  45/18=2.5
	{
		error_sum = error_sum + KI[0]*error;
	}

	//????
	if(error_sum > 150)
	{
		error_sum = 150;
	}
	if(error_sum < -150)
	{
		error_sum = -150;
	}
	u_temp = KP[0] * error + error_sum + KD[0]*(last_error - error);
	last_error = error;

	if(error > -1 && error < 1) {
		u_temp = 0;
		error_sum = 0;
	}

	if(gl_TempFlag[1] == 1) {
		error_sum = 0;
	}
	return (int)u_temp;
}

static int Depth_Control(void) 
{
		float u_temp = 0, error = 0;
		static	float last_error = 0;
		error = Ref_Depth - depth;

		if(error > -10 && error < 10)  {
			error_sum = error_sum + KI[1]*error;
		}

		if(error_sum > 150) {
			error_sum = 150;
		}
		if(error_sum < -150) {
			error_sum = -150;
		}
		u_temp = KP[1] * error + error_sum + KD[1]*(last_error - error);
		last_error = error;

		if(error > -3 && error < 3) {
			u_temp = 0;
			error_sum = 0;
		}

		if(gl_TempFlag[1] == 1)  //?????,???0,???????????
		{
			error_sum = 0;
		}

		return (int)u_temp;
}

static void XiaYang_Control(void)    //?????????yaw???
{
	uint16_t Temp1[4]={0,0,0,0};
	int u = 0;
	int ud = 0;
	uint8_t i = 0;
	u = Yaw_Control();
	ud = Depth_Control();

	Temp1[3] = PWM_STOP + 2*(Ref_Speed-50) - u - ud;
	Temp1[2] = PWM_STOP - 2*(Ref_Speed-50) + u - ud;
	Temp1[1] = PWM_STOP - 2*(Ref_Speed-50) - u + ud;
	Temp1[0] = PWM_STOP + 2*(Ref_Speed-50) + u + ud;
	
	
	// deadzone adjustment
	for(i = 0 ; i < 4;i++)
	{
		if (Temp1[i] > PWM_STOP)
			Temp1[i] = Temp1[i] + rotorD1[i];
		else
			Temp1[i] = Temp1[i] - rotorD2[i];
	}
	

	EPwmSetup1(Temp1[0]);
	EPwmSetup2(Temp1[1]);
	EPwmSetup3(Temp1[2]);
	EPwmSetup4(Temp1[3]);
	
	Buff_Real[4] = (Temp1[0] & 0xFF00) >> 8;
	Buff_Real[5] = (Temp1[0] & 0x00FF);
	
	Buff_Real[12] = (Temp1[1] & 0xFF00) >> 8;
	Buff_Real[13] = (Temp1[1] & 0x00FF);
	
	Buff_Real[14] = (Temp1[2] & 0xFF00) >> 8;
	Buff_Real[15] = (Temp1[2] & 0x00FF);
	
	Buff_Real[16] = (Temp1[3] & 0xFF00) >> 8;
	Buff_Real[17] = (Temp1[3] & 0x00FF);
	
	//Send_SingleSensor();

	Store_Data1();
}
static void Right_Control(void)
{
	spacecontrol2();
	Store_Data();
}
static void Left_Control(void)
{
	spacecontrol1();
	Store_Data();

}
static void Stop_Control(void) //force to stop the AUV, and reset the relevant flags and variants.
{
	uint8_t i = 0;
	Last_AUV_State = 1;
	AUV_State = 1;
	error_sum = 0;
	for(i = 0;i < 10;i++)
	{
		gl_TempFlag[i] = 0;
	}
	for(i = 0;i < 5;i++)
	{
		gl_TimeFlag[i] = 0;
	}
	MOTOR_STOP();
}







static uint16_t Num_Change(uint16_t s,uint8_t n)
{
	uint16_t sp;

	if(s<=50)
	{
		sp=PWM_STOP - (PWM_STOP - PWM_BACK_MAX)/50*(50-s)*Motor_Init[n]/100;
	}
	else if(s<=100)
	{
		sp=PWM_STOP + (PWM_FORWARD_MAX - PWM_STOP)/50*(s-50)*Motor_Init[n]/100;
	}
	else
	{
		sp = PWM_STOP;
	}

	//****?sp????*******
	if(sp <= PWM_BACK_MAX)
	{
		sp = PWM_BACK_MAX;
	}
	if(sp >= PWM_FORWARD_MAX)
	{
		sp = PWM_FORWARD_MAX;
	}
	return sp;
}

static int	Forcez_Change(uint8_t s)
{
	float temp = 0;
	temp = (float)s/255*80;
	return (int)temp;
}


static int	Forcex_Change(uint8_t s)
{
	float temp = 0;
	temp = (float)s/255*130;
	return (int)temp;
}

static void Saturation(uint16_t *temp,uint16_t min,uint16_t max)
{
	if(*temp < min)
	{
		*temp = min;
	}
	else if(*temp > max)
	{
		*temp = max;
	}
}

extern void Store_Data1(void)
{
	static uint8_t i = 0;
	if(Last_AUV_State != AUV_State)
	{
		i = 0;
		Last_AUV_State = AUV_State;
	}

	if(gl_TempFlag[1] == 0)
	{
		if(i > 199)
		{
			i = 199;
		}
		Buff_Total[i][0] = Real_AHRSBuff[18];
		Buff_Total[i][1] = Real_AHRSBuff[19];
		Buff_Total[i][2] = Real_AHRSBuff[20];
		Buff_Total[i][3] = Real_AHRSBuff[21];
		Buff_Total[i][4] = Real_AHRSBuff[22];
		Buff_Total[i][5] = Real_AHRSBuff[23];

		Buff_Total[i][6] = Real_AHRSBuff[6];
		Buff_Total[i][7] = Real_AHRSBuff[7];
		Buff_Total[i][8] = Real_AHRSBuff[4];
		Buff_Total[i][9] = Real_AHRSBuff[5];
		Buff_Total[i][10] = Real_AHRSBuff[2];
		Buff_Total[i][11] = Real_AHRSBuff[3];

		Buff_Total[i][12] = Real_AHRSBuff[28];
		Buff_Total[i][13] = Real_AHRSBuff[29];


		Buff_Total[i][14] = (uint8_t)(Force[0]+100);
		Buff_Total[i][15] = (uint8_t)(Force[1]+100);
		Buff_Total[i][16] = (uint8_t)(Force[2]+100);
		Buff_Total[i][17] = (uint8_t)(Force[3]+100);

		Buff_Total[i][18] = gl_Dis[0];
		Buff_Total[i][19] = gl_Dis[1];
		i++;
	}
	else
	{
		i = 0;
		Last_AUV_State = AUV_State;
		AUV_State = 1;
		Send_TimeOver();
		MOTOR_STOP();
	}

}

extern void Store_Data(void)
{
	static uint8_t i = 0;
	if(Last_AUV_State != AUV_State)
	{
		i = 0;
		Last_AUV_State = AUV_State;
	}

	if(gl_TempFlag[0] == 0)
	{
		if(i > 199)
		{
			i = 199;
		}
		Buff_Total[i][0] = Real_AHRSBuff[18];
		Buff_Total[i][1] = Real_AHRSBuff[19];
		Buff_Total[i][2] = Real_AHRSBuff[20];
		Buff_Total[i][3] = Real_AHRSBuff[21];
		Buff_Total[i][4] = Real_AHRSBuff[22];
		Buff_Total[i][5] = Real_AHRSBuff[23];

		Buff_Total[i][6] = Real_AHRSBuff[6];
		Buff_Total[i][7] = Real_AHRSBuff[7];
		Buff_Total[i][8] = Real_AHRSBuff[4];
		Buff_Total[i][9] = Real_AHRSBuff[5];
		Buff_Total[i][10] = Real_AHRSBuff[2];
		Buff_Total[i][11] = Real_AHRSBuff[3];

		Buff_Total[i][12] = Real_AHRSBuff[28];
		Buff_Total[i][13] = Real_AHRSBuff[29];


		Buff_Total[i][14] = (uint8_t)(T[0]+100);
		Buff_Total[i][15] = (uint8_t)(T[1]+100);
		Buff_Total[i][16] = (uint8_t)(T[2]+100);
		Buff_Total[i][17] = (uint8_t)(T[3]+100);

		Buff_Total[i][18] = gl_Dis[0];
		Buff_Total[i][19] = gl_Dis[1];
		i++;
	}
	else
	{
		i = 0;
		Last_AUV_State = AUV_State;
		AUV_State = 1;
		Send_TimeOver();
		MOTOR_STOP();
	}

}

extern void Store_Data2(void)
{
	static uint8_t i = 0;
	if(Last_AUV_State != AUV_State)
	{
		i = 0;
		Last_AUV_State = AUV_State;
	}

	if(gl_TempFlag[2] == 0)
	{
		if(i > 199)
		{
			i = 199;
		}
		Buff_Total[i][0] = Real_AHRSBuff[18];
		Buff_Total[i][1] = Real_AHRSBuff[19];
		Buff_Total[i][2] = Real_AHRSBuff[20];
		Buff_Total[i][3] = Real_AHRSBuff[21];
		Buff_Total[i][4] = Real_AHRSBuff[22];
		Buff_Total[i][5] = Real_AHRSBuff[23];

		Buff_Total[i][6] = Real_AHRSBuff[6];
		Buff_Total[i][7] = Real_AHRSBuff[7];
		Buff_Total[i][8] = Real_AHRSBuff[4];
		Buff_Total[i][9] = Real_AHRSBuff[5];
		Buff_Total[i][10] = Real_AHRSBuff[2];
		Buff_Total[i][11] = Real_AHRSBuff[3];

		Buff_Total[i][12] = Real_AHRSBuff[28];
		Buff_Total[i][13] = Real_AHRSBuff[29];


		Buff_Total[i][14] = (uint8_t)(T[0]+100);
		Buff_Total[i][15] = (uint8_t)(T[1]+100);
		Buff_Total[i][16] = (uint8_t)(T[2]+100);
		Buff_Total[i][17] = (uint8_t)(T[3]+100);

		Buff_Total[i][18] = gl_Dis[0];
		Buff_Total[i][19] = gl_Dis[1];
		i++;
	}
	else
	{
		i = 0;
		Last_AUV_State = AUV_State;
		AUV_State = 1;
		Send_TimeOver();
		MOTOR_STOP();
	}

}


void MOTOR_STOP(void)
{
	uint8_t i = 0;
	for(i = 0;i < 4;i++)
	{
		Force[i] = 0;
		T[i] = 0;

	}
		EPwmSetup1(PWM_STOP);
		EPwmSetup2(PWM_STOP);
		EPwmSetup3(PWM_STOP);
		EPwmSetup4(PWM_STOP);
}


void Motor_InitSet(void)
{
	Motor_Init[0] = Real_PcBuff[2];		//second motor
	Motor_Init[1] = Real_PcBuff[3];		//third  motor
	Motor_Init[2] = Real_PcBuff[4];		//forth  motor
	UART_Send();
}

void Motor14_RelayOn(void)
{
	Relay4_On();
}

void Motor23_RelayOn(void)
{
	Relay3_On();
}
