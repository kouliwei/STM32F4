/*
 * USER_TEST.h
 *
 *  Created on: 2017-4-5
 *      Author: kou
 */

#ifndef TEST_H
#define TEST_H


#include "user.h"


#if DEBUG == 0
extern void Test(void);
static void Commu_Test(void);
static void Led_Test(void);
static void Key_Test(void);
static void Pwm_Test(void);
static void Led_Mode1(void);
static void Led_Mode2(void);
static uint8_t SCI_Return(void);
static void Game_Over(void);
static void Game_Over_Pwm(void);
static void Game_Over_Key(void);
static void Game_Over_Led(void);
#endif



#endif
