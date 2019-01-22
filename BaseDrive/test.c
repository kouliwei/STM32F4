#include "test.h"



#if	DEBUG == 0
extern void Test(void)
{
        uint8_t ch;
        uint8_t exit_flag = 0;
        uint8_t flag = 0;
        while(1)
        {
                if(0 == flag){
                        printf("\r\n**************welcome to the menu****************\r\n");
                        printf("press b or B to enter the Led test\r\n");
                        printf("press d or D to enter the pwm test \r\n");
                        printf("press e or E to exit the menu\r\n");
                        flag = 1;
                }
                ch = SCI_Return();



                switch(ch){
                        case 'b':
                        case 'B':
                                Led_Test();
                                flag = 0;
                                break;
                        case 'd':
                        case 'D':
                                Pwm_Test();
                                flag = 0;
                                break;
                        case 'e':
                        case 'E':
                                Game_Over();
                                exit_flag = 1;
                                break;
                }
                if(exit_flag == 1)
                        break;
        }

}

static void Led_Test(void)
{
        uint8_t ch = 0;
        uint8_t exit_flag = 0;
        while(1){
                printf("press a or A to the drive the led_mode1\r\n");
                printf("press w or W to the drive the led_mode2\r\n");
                printf("press e or E to exit the Led_Test\r\n");
                ch = SCI_Return();


                switch(ch){
                        case 'a':
                        case 'A':
                                Led_Mode1();
                                break;
                        case 'w':
                        case 'W':
                                Led_Mode2();
                                break;
                        case 'e':
                        case 'E':
                                Game_Over_Led();
                                exit_flag = 1;
                }
                if(exit_flag == 1)
                        break;
        }
}


/**********跑马灯模式***************/
void Led_Mode1()
{
        uint8_t i = 0;
        for(i = 0;i < 4; i++)
        {
                switch(i)
                {
                        case 0:
                                Led_Driver(0x01);
                                break;
                        case 1:
                                Led_Driver(0x02);
                                break;
                        case 2:
                                Led_Driver(0x04);
                                break;
                        case 3:
                                Led_Driver(0x08);
                                break;
                }
                Delay_Nms(500);
        }
}

/*********全亮全灭模式**************/
void Led_Mode2()
{
        uint8_t i = 0;
        for(i = 0;i < 4;i ++)
        {
                if(i % 2)
                {
                        Led_Driver(0x0f);
                }
                else
                {
                        Led_Driver(0x00);
                }
                Delay_Nms(500);
        }
}



static void Pwm_Test()
{
        uint8_t ch = 0;
        uint8_t exit_flag = 0;
        uint16_t high_level = PWM_STOP;
        while(1){
                printf("\r\n******welcome to the Pwm_Test *********\r\n");
                printf("press d or D to stop         \r\n");
                printf("press w or W to forward max  \r\n");
                printf("press s or S to backward max \r\n");
                printf("press q or Q to slow down    \r\n");
                printf("press r or R to speedup      \r\n");
                printf("press e or E to exit Pwm test\r\n");
                ch = SCI_Return();


                switch(ch){
                        case 'd':
                        case 'D':
                                high_level = PWM_STOP;
                                break;
                        case 'w':
                        case 'W':
                                high_level = PWM_FORWARD_MAX;
                                break;
                        case 's':
                        case 'S':
                                high_level = PWM_BACK_MAX;
                                break;
                        case 'r':
                        case 'R':
                                high_level += 100;
                                if(high_level >= PWM_FORWARD_MAX)
                                {
                                        high_level = PWM_FORWARD_MAX;
                                }
                                break;
                        case 'q':
                        case 'Q':
                                high_level -= 100;
                                if(high_level <= PWM_BACK_MAX)
                                {
                                        high_level = PWM_BACK_MAX;
                                }
                                break;
                        case 'e':
                        case 'E':
                                Game_Over_Pwm();
                                exit_flag = 1;
                                break;
                }
                EPwmSetup1(high_level);
                EPwmSetup2(high_level);
                EPwmSetup3(high_level);
                EPwmSetup4(high_level);
                if(exit_flag == 1)
                {
                        high_level = PWM_STOP;
                        EPwmSetup1(high_level);
                        EPwmSetup2(high_level);
                        EPwmSetup3(high_level);
                        EPwmSetup4(high_level);
                        break;
                }
        }

}

static uint8_t SCI_Return(void)
{
        uint8_t ch = 0;
        while (USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == RESET);
        ch = USART_ReceiveData(USART1);
        return ch;

}

static void Game_Over(void)
{
        printf("test over\t\n");
}

static void Game_Over_Led(void)
{
        printf("exit the Led_Test Led\r\n");
}


static void Game_Over_Pwm(void)
{
        printf("exit the Pwm_Test Led\r\n");
}


#endif
