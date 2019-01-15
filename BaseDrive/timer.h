#ifndef __TIMER_H
#define __TIMER_H

#include "user.h"


// timer 4 is configurated to   50hz   
//   84000000 / ((27 + 1) * (59999 + 1))
#define TIM4PSC 			27
#define TIM4Period 		59999

#define PWM_STOP			4500
#define PWM_FORWARD_MAX		6000
#define PWM_BACK_MAX		3000

extern void EPwmSetup1(uint16_t duty11);
extern void EPwmSetup2(uint16_t duty22);
extern void EPwmSetup3(uint16_t duty33);
extern void EPwmSetup4(uint16_t duty44);
extern uint16_t EPwmRead(uint8_t i);

extern void EPwm_Init(void);
void Timer_Init(void);


#endif
