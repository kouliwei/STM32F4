#include "timer.h"


//  100ms timer
void Timer7_Init(void)
{
        TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);  


        TIM_TimeBaseInitStructure.TIM_Period = 9999; 	
        TIM_TimeBaseInitStructure.TIM_Prescaler = 419;  
        TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //??????
        TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
        TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//???TIM3

        TIM_ARRPreloadConfig(TIM7,ENABLE);

        TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);
        TIM_Cmd(TIM7, DISABLE); 
}
//定时器7中断服务程序		    
void TIM7_IRQHandler(void)
{ 	    		    
        USART3_RX_STA|=1<<15;	//标记接收完成
        TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //???????
        TIM_Cmd(TIM7, DISABLE); 
} 


//  50ms timer
void Timer_Init(void)
{
        TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  


        TIM_TimeBaseInitStructure.TIM_Period = 9999; 	
        TIM_TimeBaseInitStructure.TIM_Prescaler = 419;  
        TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //??????
        TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
        TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//???TIM3

        TIM_ARRPreloadConfig(TIM3,ENABLE);

        TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE );


        TIM_Cmd(TIM3, ENABLE); 
}


extern void  EPwm_Init(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;
        TIM_OCInitTypeDef  TIM_OCInitStructure;
        TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);

        GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4); //??GPIOA_Pin8?TIM1_Ch1, 
        GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4); //??GPIOA_Pin11?TIM1_Ch4,????CH4N 
        GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
        GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;           //GPIO
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //????
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//??100MHz
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //??????
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //??
        GPIO_Init(GPIOD,&GPIO_InitStructure);              //???P



        TIM_TimeBaseInitStructure.TIM_Period = 59999; 	
        TIM_TimeBaseInitStructure.TIM_Prescaler = 27;  
        TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //??????
        TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 

        TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//???TIM3

        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 

        TIM_OCInitStructure.TIM_Pulse = PWM_STOP; 
        TIM_OC1Init(TIM4, &TIM_OCInitStructure); 

        TIM_OCInitStructure.TIM_Pulse = PWM_STOP; 
        TIM_OC2Init(TIM4, &TIM_OCInitStructure);

        TIM_OCInitStructure.TIM_Pulse = PWM_STOP;
        TIM_OC3Init(TIM4, &TIM_OCInitStructure);//??4

        TIM_OCInitStructure.TIM_Pulse = PWM_STOP;
        TIM_OC4Init(TIM4, &TIM_OCInitStructure);//??4

        TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  
        TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable); 
        TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable); 
        TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable); 

        TIM_ARRPreloadConfig(TIM4,ENABLE);

        TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE );


        TIM_Cmd(TIM4, ENABLE);  

        //	Delay_Nms(5000);
        Motor14_RelayOn();		//number 1st and 4th motors power on
        Delay_Nms(3000);
        Motor23_RelayOn();		//number 2nd and 3rd motors power on

}



void TIM4_IRQHandler(void)
{
        static uint8_t i = 0;
        if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //????
        {
                i++;
                if(i>50)
                {
                        i = 0;
                        LED0_TOG();
                }
        }
        TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //???????
}



#define VOLTAGE_RATE1	12.59/2.605
#define VOLTAGE_RATE2	12.00/2.534

void Voltage_Storage(void)
{
        uint16_t temp[2] = {0,0};
        float temp1[2] = {0,0};
        temp[0] = uAD_Buff_B;
        temp[1] = uAD_Buff_A;

        temp1[0] = temp[0] * 3.30 / 4095 * VOLTAGE_RATE1;
        temp1[1] = temp[1] * 3.30 / 4095 * VOLTAGE_RATE2;

        Voltage[0] = ((uint16_t)(temp1[0]*100)) / 100;
        Voltage[1] = ((uint16_t)(temp1[0]*100)) % 100;
        Voltage[2] = ((uint16_t)(temp1[1]*100)) / 100;
        Voltage[3] = ((uint16_t)(temp1[1]*100)) % 100;
        // judge the battery voltage is enougn or not
        if(temp1[0] > 11.0 && temp1[1] > 11.0)
        {
                Voltage[4] = 0; 
        }
        else
        {
                Voltage[4] = 1; 
        }
        gl_GPSInf[0] = ( gpsx.latitude && ( 0XFF000000) ) >> 24;
        gl_GPSInf[1] = ( gpsx.latitude && ( 0X00FF0000) ) >> 16;
        gl_GPSInf[2] = ( gpsx.latitude && ( 0X0000FF00) ) >> 8;
        gl_GPSInf[3] = ( gpsx.latitude && ( 0X000000FF) ) ;
        gl_GPSInf[4] = gpsx.nshemi; 

        gl_GPSInf[5] = ( gpsx.longitude && ( 0XFF000000) ) >> 24;
        gl_GPSInf[6] = ( gpsx.longitude && ( 0X00FF0000) ) >> 16;
        gl_GPSInf[7] = ( gpsx.longitude && ( 0X0000FF00) ) >> 8;
        gl_GPSInf[8] = ( gpsx.longitude && ( 0X000000FF) ) ;
        gl_GPSInf[9] = gpsx.ewhemi; 
        
        PC_ReadVol();


}


// 50ms timer
void TIM3_IRQHandler(void)
{
        gl_FlagMotionCycle = 1;
        if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //????
        {
                if(gl_jsflag[4] > 200)  //10s send voltage value 
                {
                        //Voltage_Storage();
                        gl_jsflag[4] = 0;
                }
                else
                {
                        gl_jsflag[4]++;
                }

                if(gl_TempFlag[3] == 0)  // single sensor data 2s/c
                {
                        if(gl_jsflag[3] > 0)
                        {
                                gl_jsflag[3] = 0;
                                Send_SingleSensor();
                        }
                        else
                        {
                                gl_jsflag[3]++;
                        }
                }
                else
                {
                        gl_jsflag[3] = 0;
                }


                if(AUV_State == 8 || AUV_State == 9)
                {
                        if(gl_jsflag[0] >= 20*sliding_para.data[8])  //time is over, reset the state of the AUV
                        {
                                gl_jsflag[0] = 0;
                                gl_TempFlag[0] = 1;
                        }
                        else
                        {
                                gl_jsflag[0]++;
                        }
                }
                else
                {
                        gl_TempFlag[0] = 0;
                        gl_jsflag[0] = 0;
                }

                if(AUV_State == 7)
                {
                        if(Control_Time == 10)  //if Control_Time == 100, infinite time motion.
                        {
                                gl_jsflag[1] = 0;
                        }
                        if(gl_jsflag[1] >= 20*Control_Time)  //time is over, reset the state of the AUV
                        {
                                gl_jsflag[1] = 0;
                                gl_TempFlag[1] = 1;
                                Send_Datatest();  //flag
                        }
                        else
                        {
                                gl_jsflag[1]++;
                        }
                }
                else
                {
                        gl_TempFlag[1] = 0;
                        gl_jsflag[1] = 0;
                }


                if(AUV_State == 4)
                {
                        if(gl_jsflag[2] >= 200)  //time is over, reset the state of the AUV
                        {
                                gl_jsflag[2] = 0;

                                gl_TempFlag[2] = 1;
                        }
                        else
                        {
                                gl_jsflag[2]++;
                        }
                }
                else
                {
                        gl_TempFlag[2] = 0;
                        gl_jsflag[2] = 0;
                }


        }
        TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //???????
}







void EPwmSetup1(uint16_t duty11)
{
        TIM4->CCR3 = duty11;
}

void EPwmSetup2(uint16_t duty22)
{
        TIM4->CCR4 = duty22;
}

void EPwmSetup3(uint16_t duty33)
{
        TIM4->CCR1 = duty33;
}

void EPwmSetup4(uint16_t duty44)
{
        TIM4->CCR2 = duty44;
}


uint16_t EPwmRead(uint8_t i)
{
        uint16_t j = 0;
        switch(i){
                case 1:
                        j = TIM4->CCR3;
                        break;
                case 2:
                        j = TIM4->CCR4;
                        break;
                case 3:
                        j = TIM4->CCR1;
                        break;
                case 4:
                        j = TIM4->CCR2;
                        break;
        }
        return j;
}




