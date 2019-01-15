#############################################

	author:	kouliwei
	date:	2019-01-15
	
#############################################
The project is to control the yaw angle and depth of an AUV according to the PC's commands, here, we select STM32F407ZGT6 as the master chip, and the main functions are explained as follows:

(1)BaseDrive: the user driver

(2)Libraries: STM32F4 library

(3)User: function: main

More specific:

SystemInitUser() 
---------------------------------------------------------------------------------
system init function, it mainly includes: 
NVIC_PriorityGroupConfig(NVIC_PriorityGroup2) 	--> setting interrupt group
SystemInit(): 					--> clock init
LED_Init(): 					--> LED GPIO init
Relay_Init(): 					--> Relay GPIO init
Sci_Init(): 					--> usart init
USER_ADC_Init(): 				--> ADC init
EPwm_Init(): 					--> pwm init. 
In order to control the designed thruster, the period should be set as 20ms. 
If the high level is 1.5ms, the thruster stops; 
if the high level is 2.0ms, the thruster positively rotates at the maximum rate; 
if the hign level is 1.0ms, the thruster negatively rotates at the maximum rate.
Timer_Init(): 					--> timer init. For timing task.
Para_Init(): 					--> system parameter init.
imu_data_decode_init(): 			-->imu init.
NVIC_Config(): 					-->STM32 interrupt configuration
---------------------------------------------------------------------------------



DataInter_AHRS()
---------------------------------------------------------------------------------
imu sensor data receive function
AHRS_RX_Flag : 
flag to judge whether the imu data is received or not, and it is set in the interrupt function USART6_IRQHandler(void).
get_raw_acc(Acc): 				--> restore the acceleration data
get_raw_gyo(Gyo):				--> restore the gyroscope data
get_raw_mag(Mag):				--> restore the magnatic data
get_eular(Eular):				--> restore the eular data
get_quad(Quat):					--> restore the Quat data
Store_SensorData():				--> restore the key data to Buff_Real[20]

