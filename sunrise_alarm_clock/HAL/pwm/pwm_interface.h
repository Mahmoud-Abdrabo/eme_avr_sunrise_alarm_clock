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



#define   PWM_MAX_REG_COUNT     256
#define   PWM_PORT_INDEX        0
#define   PWM_PIN_INDEX         1

void pwm_init(void);
void pwm_start();
void pwm_stop();
void pwm_set_duty(uint8_t_ percentage);




#endif /* PWM_H_ */