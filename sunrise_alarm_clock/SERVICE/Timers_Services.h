/*
 * Timers_Services.h
 *
 * Created: 8/29/2023 10:33:40 AM
 *  Author: ahmed
 */ 


#ifndef TIMERS_SERVICES_H_
#define TIMERS_SERVICES_H_

#define OCR2_8FREQ      2
#define OCR2_64TIMER    4

#include "Timers.h"
void Timer2_SetInterruptTime_ms (uint16_t_ time,void(*LocalFptr)(void));
void Timer2_SetInterruptTime_us (uint16_t_ time,void(*LocalFptr)(void));


#endif /* TIMERS_SERVICES_H_ */