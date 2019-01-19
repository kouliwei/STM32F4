
#include "usart.h"
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
uint16_t USART3_RX_STA = 0;
uint8_t USART3_Buff[USART3_MAX_LEN]; 
//#if DEBUG != 0
////????????
////A5 5A 12 A1 08 83 80 43 06 9A 80 03 01 50 27 3E 02 41 1D AA
////A5 5A 16 A2 87 CB 0C 00 BE CB 00 00 00 00 00 00 81 14 00 B6 01 3B 26 AA
extern void Send_Datatest(void)
{
        Scib_Xmit(0xA5);
        Scib_Xmit(0x5A);
        Scib_Xmit(0x06);
        Scib_Xmit(0x01);
        Scib_Xmit(0x01);
        Scib_Xmit(0x32);
        Scib_Xmit(0x3a);
        Scib_Xmit(0xAA);
}
extern void UART_SendDis(void)
{
        uint16_t sum_check = 0;
        Scib_Xmit(0xA5);
        Scib_Xmit(0x5A);
        Scib_Xmit(0x06);
        Scib_Xmit(0x03);
        Scib_Xmit(gl_Dis[0]);
        Scib_Xmit(gl_Dis[1]);
        sum_check=0x06+0x03+gl_Dis[0]+gl_Dis[1];
        sum_check=sum_check%256;
        Scib_Xmit(sum_check);
        Scib_Xmit(0xAA);
}
extern void UART_Send(void)
{
        uint16_t sum_check = 0,i = 0;
        Scib_Xmit(0xA5);
        Scib_Xmit(0x5A);
        for(i = 0;i < Real_PcBuff[0] - 2;i++)
        {
                Scib_Xmit(Real_PcBuff[i]);
                sum_check += Real_PcBuff[i];
        }
        sum_check &= 0x00FF;
        Scib_Xmit(sum_check);
        Scib_Xmit(0xAA);
}
extern void Send_Data(void)
{
        uint16_t i = 0,j = 0;
        uint16_t sum_check = 0;
        for(i = 0;i < 200;i++)
        {
                Scib_Xmit(0xA5);
                Scib_Xmit(0x5A);
                Scib_Xmit(0x18);
                Scib_Xmit(0xA1);
                sum_check = 0;
                for(j = 0;j < 20;j++)
                {
                        Scib_Xmit(Buff_Total[i][j]);
                        sum_check = sum_check + Buff_Total[i][j];
                        Buff_Total[i][j] = 0; //reset after send
                }
                sum_check = sum_check + 0x18 + 0xA1;
                sum_check = sum_check % 256;
                Scib_Xmit(sum_check);
                Scib_Xmit(0xAA);
                Delay_Nms(100);
        }
}
extern void Send_VoltageAlert(void)
{
        Scib_Xmit(0xA5);
        Scib_Xmit(0x5A);
        Scib_Xmit(0x04);
        Scib_Xmit(0x0D);
        Scib_Xmit(0x11);
        Scib_Xmit(0xAA);
}
extern void Send_TimeOver(void)
{
        Scib_Xmit(0xA5);
        Scib_Xmit(0x5A);
        Scib_Xmit(0x04);
        Scib_Xmit(0x08);
        Scib_Xmit(0x0C);
        Scib_Xmit(0xAA);
}
//A5 5A 12 A1 08 83 80 43 06 9A 80 03 01 50 27 3E 02 41 1D AA
//A5 5A 16 A2 87 CB 0C 00 BE CB 00 00 00 00 00 00 81 14 00 B6 01 3B 26 AA
extern void Send_SingleSensor(void)
{
        uint16_t i = 0;
        uint16_t sum_check = 0;
        Scib_Xmit(0xA5 );
        Scib_Xmit(0x5A );
        Scib_Xmit(0x18 );
        Scib_Xmit(0xA1 );
        for(i = 0;i < 20;i++)
        {
                Scib_Xmit(Buff_Real[i] );
                sum_check += Buff_Real[i];
        }
        sum_check = sum_check + 0x18 + 0xA1;
        sum_check = sum_check % 256;
        Scib_Xmit(sum_check );
        Scib_Xmit(0xAA );
}
extern void UART_SendSensor(void)
{
        uint16_t sum_check = 0,i = 0;
        Scib_Xmit(0xA5);
        Scib_Xmit(0x5A);
        for(i = 0;i < 16;i++)
        {
                Scib_Xmit(Real_AHRSBuff[i]);
                sum_check += Real_AHRSBuff[i];
        }
        sum_check &= 0x00FF;
        Scib_Xmit(sum_check);
        Scib_Xmit(0xAA);
        sum_check = 0;
        Scib_Xmit(0xA5);
        Scib_Xmit(0x5A);
        for(i = 16;i < 36;i++)
        {
                Scib_Xmit(Real_AHRSBuff[i]);
                sum_check += Real_AHRSBuff[i];
        }
        sum_check &= 0x00FF;
        Scib_Xmit(sum_check);
        Scib_Xmit(0xAA);
}
extern void Sci_Init()
{
        Scia_Init();
        Scib_Init();
        Scic_Init();
        SciGPS_Init();
}
static void SciGPS_Init(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;	
        USART_InitTypeDef USART_InitStructure;  //?????????
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
        /*USART6_TX ->PC6*/	
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP; //?????????? 
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //???????? 
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//????9???  
        GPIO_Init(GPIOD, &GPIO_InitStructure);           //????,?????????????		 
        /*USART6_RX ->PC7*/
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;  
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//????
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
        GPIO_Init(GPIOD, &GPIO_InitStructure);
        GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_USART3);
        GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART3);
        /*????????*/
        USART_InitStructure.USART_BaudRate = USART_Baud; //???
        USART_InitStructure.USART_WordLength = USART_WordLength_8b; //???8?
        USART_InitStructure.USART_StopBits = USART_StopBits_1;	//???1?
        USART_InitStructure.USART_Parity = USART_Parity_No;		//??? ?
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//????
        USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//?????????
        USART_Init(USART3, &USART_InitStructure); 
        USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
        USART_Cmd(USART3, ENABLE);
}
static void Scid_Init(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;	
        USART_InitTypeDef USART_InitStructure;  //?????????
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
        /*USART2_TX ->PA2*/	
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP; //?????????? 
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //???????? 
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//????9???  
        GPIO_Init(GPIOA, &GPIO_InitStructure);           //????,?????????????		 
        /*USART2_RX ->PA3*/
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	        
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;  
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//????
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
        GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);
        /*????????*/
        USART_InitStructure.USART_BaudRate = USART_Baud; //???
        USART_InitStructure.USART_WordLength = USART_WordLength_8b; //???8?
        USART_InitStructure.USART_StopBits = USART_StopBits_1;	//???1?
        USART_InitStructure.USART_Parity = USART_Parity_No;		//??? ?
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//????
        USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//?????????
        //???????????????USART_Init?????
        USART_Init(USART2, &USART_InitStructure); 
        //	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
        //	USART_ClearITPendingBit(USART2, USART_IT_RXNE);
        USART_Cmd(USART2, ENABLE);//??USART1,?????RCC_APB2PeriphClockCmd()?????
}
// PC
static void Scib_Init(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;	
        USART_InitTypeDef USART_InitStructure;  //?????????
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
        /*USART1_TX ->PA9*/	
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP; //?????????? 
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //???????? 
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//????9???  
        GPIO_Init(GPIOA, &GPIO_InitStructure);           //????,?????????????		 
        /*USART1_RX ->PA10*/
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	        
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;  
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//????
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
        GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
        /*????????*/
        USART_InitStructure.USART_BaudRate = USART_Baud; //???
        USART_InitStructure.USART_WordLength = USART_WordLength_8b; //???8?
        USART_InitStructure.USART_StopBits = USART_StopBits_1;	//???1?
        USART_InitStructure.USART_Parity = USART_Parity_No;		//??? ?
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//????
        USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//?????????
        //???????????????USART_Init?????
        USART_Init(USART1, &USART_InitStructure); 
        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
        USART_Cmd(USART1, ENABLE);//??USART1,?????RCC_APB2PeriphClockCmd()?????
}
// IMU 
static void Scia_Init(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;	
        USART_InitTypeDef USART_InitStructure;  //?????????
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
        /*USART6_TX ->PC6*/	
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP; //?????????? 
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //???????? 
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//????9???  
        GPIO_Init(GPIOC, &GPIO_InitStructure);           //????,?????????????		 
        /*USART6_RX ->PC7*/
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	        
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;  
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//????
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
        GPIO_Init(GPIOC, &GPIO_InitStructure);
        GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6);
        GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6);
        /*????????*/
        USART_InitStructure.USART_BaudRate = USART_Baud; //???
        USART_InitStructure.USART_WordLength = USART_WordLength_8b; //???8?
        USART_InitStructure.USART_StopBits = USART_StopBits_1;	//???1?
        USART_InitStructure.USART_Parity = USART_Parity_No;		//??? ?
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//????
        USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//?????????
        USART_Init(USART6, &USART_InitStructure); 
        USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);
        USART_ClearITPendingBit(USART6, USART_IT_RXNE);
        USART_Cmd(USART6, ENABLE);
}
static void Scic_Init(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;	
        USART_InitTypeDef USART_InitStructure;  //?????????
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
        /*USART4_TX ->PA0*/	
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP; //?????????? 
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //???????? 
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//????9???  
        GPIO_Init(GPIOA, &GPIO_InitStructure);           //????,?????????????		 
        /*USART4_RX ->PA1*/
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	        
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;  
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//????
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_UART4);
        GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_UART4);
        /*????????*/
        USART_InitStructure.USART_BaudRate = USART_Baud; //???
        USART_InitStructure.USART_WordLength = USART_WordLength_8b; //???8?
        USART_InitStructure.USART_StopBits = USART_StopBits_1;	//???1?
        USART_InitStructure.USART_Parity = USART_Parity_No;		//??? ?
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//????
        USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//?????????
        USART_Init(UART4, &USART_InitStructure); 
        USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
        USART_ClearITPendingBit(UART4, USART_IT_RXNE);
        USART_Cmd(UART4, ENABLE);
}
extern void Scib_Xmit(uint8_t a)
{
        while(RESET ==USART_GetFlagStatus(USART1,USART_FLAG_TXE));
        USART_SendData(USART1,a);
}
extern void Scib_Msg(char * msg)
{
        int i;
        i = 0;
        //	TX_EN;
        while(msg[i] != '\0')
        {
                Scib_Xmit(msg[i]);
                i++;
        }
        //	RX_EN;
}
// PC communication
void USART1_IRQHandler(void)
{
        static uint16_t state = 0;
        static uint16_t length = 0;
        static uint16_t lencnt = 0;
        static uint16_t sum_check = 0;
        unsigned char ReceivedChar = 0;
        if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==SET) //????
        {
                USART_ClearFlag(USART1, USART_FLAG_RXNE);
                ReceivedChar = USART_ReceiveData(USART1);
                switch(state)
                {
                        case 0:
                                if(ReceivedChar == 0xA5)
                                {
                                        state = 1;
                                }
                                else
                                {
                                        state = 0;
                                }
                                length = 0;
                                lencnt = 0;
                                sum_check = 0;
                                break;
                        case 1:
                                if(ReceivedChar == 0x5A)
                                { 
                                        state = 2;
                                }
                                else
                                {
                                        state = 0;
                                }
                                break;
                        case 2:
                                length = ReceivedChar;
                                PC_Rx_Buffer[0] = ReceivedChar;
                                lencnt = 1;
                                sum_check += ReceivedChar;
                                state = 3;
                                break;
                        case 3:
                                PC_Rx_Buffer[lencnt++] = ReceivedChar;
                                sum_check += ReceivedChar;
                                if(lencnt == length - 2)
                                {
                                        state = 4;
                                }
                                break;
                        case 4:
                                if(ReceivedChar == (sum_check & 0x00FF))  //???????
                                {
                                        PC_RX_Flag = 1;
                                }
                                state = 0;
                                break;
                }
        }
}
// IMU communication
void USART6_IRQHandler(void)
{
        unsigned char ReceivedChar = 0;
        if(USART_GetFlagStatus(USART6,USART_FLAG_RXNE)==SET) //????
        {
                USART_ClearFlag(USART6, USART_FLAG_RXNE);
                ReceivedChar = USART_ReceiveData(USART6);
                Packet_Decode(ReceivedChar);
        }
}
// for GPS sensor data receive
void USART3_IRQHandler(void)
{
        uint8_t res;
        if(USART_GetFlagStatus(USART3,USART_FLAG_RXNE) == SET )
        {
                res  = USART_ReceiveData(UART3);
                if (USART3_RX_STA & (1<<15) == 0 )
                {
                        TIM7->CR1 = 0;
                        if() 
                }  
        }  
}  
// depth communication
void UART4_IRQHandler(void)
{
        unsigned char ReceivedChar = 0;
        static uint8_t state = 0;
        static uint8_t i = 0;
        if(USART_GetFlagStatus(UART4,USART_FLAG_RXNE)==SET) //????
        {
                USART_ClearFlag(UART4, USART_FLAG_RXNE);
                ReceivedChar = USART_ReceiveData(UART4);
                //		Depth_Buff[i++] = ReceivedChar;
                //		if (i == 20)
                //			i = 0;
                switch(state)  
                {
                        case 0:
                                if(ReceivedChar == 'D')  //'D'
                                {
                                        state = 1;
                                        i = 0;
                                }
                                else
                                {
                                        state = 0;
                                }
                                break;
                        case 1:
                                if(ReceivedChar == '=')	// '='
                                { 
                                        state = 2;
                                }
                                else
                                {
                                        state = 0;
                                }
                                break;
                        case 2:
                                if(ReceivedChar == '-')
                                {
                                        depth = 0;
                                        state = 0;
                                }
                                else
                                {
                                        Depth_Buff[i++] = ReceivedChar;
                                        state = 3;
                                }
                                break;
                        case 3:
                                if (ReceivedChar == 0x0D)
                                {
                                        state = 0;
                                        Depth_Buff[i] ='\0';
                                        depth = atof (Depth_Buff) * 100;  //depth unit: cm
                                }
                                else
                                {
                                        Depth_Buff[i++] = ReceivedChar;
                                }
                                break;
                }
        }
}
// depth1 communication
void USART2_IRQHandler(void)
{
        unsigned char ReceivedChar = 0;
        if(USART_GetFlagStatus(USART2,USART_FLAG_RXNE)==SET) //????
        {
                USART_ClearFlag(USART2, USART_FLAG_RXNE);
                ReceivedChar = USART_ReceiveData(USART2);
        }
}
extern void DataInter_Pc(void)
{
        if(PC_RX_Flag == 1)    //data have bee received from the computer.
        {
                //Delay_Nms(50);
                LED1_TOG();
                PC_RX_Flag = 0;
                Store_PcData(Real_PcBuff,PC_Rx_Buffer);
                if(Real_PcBuff[1] == 2)
                {
                        gl_MotionFlag = 0;
                        gl_flag = 1;
                }
                else
                {
                        gl_MotionFlag = 1;
                }
                switch(Real_PcBuff[1])
                {
                        case 1:                      //??????
                                SingleMotor_Control();
                                break;
                        case 2: 						//??????
                                Motion_Type();
                                break;
                        case 3:      				//??????
                                UART_SendDis();
                                break;
                        case 4:						//???????
                                TotalMotor_Control();
                                break;
                        case 6:                     //PID????
                                Set_PID();
                                break;
                        case 7:
                                Set_SlidingPara();
                                break;
                        case 0xb1:                   //???????
                                UART_SendSensor();
                        case 0x08:
                                PC_ReadVol();
                        case 0x09:
                                Motor_InitSet();
                                break;
                }
        }
}
extern void DataInter_AHRS(void)
{
        //???????,??????????????,?????????
        if(AHRS_RX_Flag  == 1)   //data have bee received from the sensor.
        {
                AHRS_RX_Flag = 0;
                get_raw_acc(Acc);
                get_raw_gyo(Gyo);
                get_raw_mag(Mag);
                get_eular(Eular);
                get_quat(Quat);
                get_id(&ID);
                Store_SensorData();
                //Send_SingleSensor();
                //		   temp = (AHRS_Rx_Buffer[1]<<8) + AHRS_Rx_Buffer[2];
                //		   temp = temp / 10;
                //		   scic_msg("yaw_angle:");
                //		   TXD_USART_DEC(temp);
                //		   scic_msg("\t");
                //		   temp1 = 0;
                //		   temp1 = AHRS_Rx_Buffer[3];
                //		   temp1 <<= 8;
                //		   temp1 |= AHRS_Rx_Buffer[4];
                //		   if(temp1 & 0x8000)
                //		   {
                //			   temp = (temp1&0x7fff) / 10;
                //			   temp1 = 0-(temp1&0x7fff);
                //			   scic_msg("pitch_angle:-");
                //		   }else
                //		   {
                //			   temp1 = (temp1&0x7fff);
                //			   temp = temp1 / 10;
                //			   scic_msg("pitch_angle:");
                //		   }
                //		   temp1 = temp1 / 10;   //??
                //		   TXD_USART_DEC(temp);
                //		   scic_msg("\t");
                ////
                //		   temp1 = 0;
                //		   temp1 = AHRS_Rx_Buffer[5];
                //		   temp1 <<= 8;
                //		   temp1 |= AHRS_Rx_Buffer[6];
                //		   if(temp1 & 0x8000)
                //		   {
                //			   temp = (temp1&0x7fff) / 10;
                //			   temp1 = 0-(temp1&0x7fff);
                //			   scic_msg("roll_angle:-");
                //		   }else
                //		   {
                //			   temp1 = (temp1&0x7fff);
                //			   temp = temp1 / 10;
                //			   scic_msg("roll_angle:");
                //		   }
                //		   temp1 = temp1 / 10;   //??
                //		   TXD_USART_DEC(temp);
        }
}
int fputc(int ch, FILE *f)
{
        /* Place your implementation of fputc here */
        /* e.g. write a character to the USART */
        USART_SendData(USART1, (uint8_t) ch);
        /* Loop until the end of transmission */
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
        {}
        return ch;
}
