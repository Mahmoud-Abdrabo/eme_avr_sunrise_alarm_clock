/*
 * pwm.h
 *
 * Created: 8/27/2023 1:51:08 PM
 *  Author: A M
 */ 


#ifndef PWM_H_
#define PWM_H_


#include "Timers.h"
#include "Led.h"


#define   NUMBEROFARRAY       4
#define   NUMBEROFELEMNTS     2
#define   MAXMUNREGNUMBER     256
#define   FIRSTINDEX          0
#define   SECONDINDEX         1

void Pwm_Init(uint8_t_ Led_Arr [][2]);
void timer_init();
void Pwm_Start();
void Pwm_Stop();
void Set_Dutycycle(uint8_t_ percentage);




#endif /* PWM_H_ */