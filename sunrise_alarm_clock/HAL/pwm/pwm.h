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

void Pwm_Init(uint8_t_ **Led_Arr);
void timer_init();
void Pwm_Start();
void Pwm_Stop();
void Set_Dutycycle(uint8_t_ percentage);




#endif /* PWM_H_ */