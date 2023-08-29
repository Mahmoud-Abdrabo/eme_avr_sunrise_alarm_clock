/*
 * Timers_Services.c
 *
 * Created: 8/29/2023 10:33:08 AM
 *  Author: ahmed
 */ 



#include "mem_map.h"
#include "std.h"
#include "Timers_Services.h"




static void timer_func(void);
uint16_t_ no_of_overflow = 0;
static void(*fptr_timer)(void);
void Timer2_SetInterruptTime_ms (uint16_t_ time,void(*LocalFptr)(void))
{
	OCR2 = 250; //count 1m
	
	no_of_overflow = time; //convert from ms to us
	TIMER2_OC_InterruptEnable();
	Timer2_change(TIMER2_SCALER_64);	fptr_timer=LocalFptr;
	Timer2_OC_SetCallBack(timer_func);
	
}

static void timer_func()
{
	static uint16_t_ c=0;
	c++;
	if (c==no_of_overflow)
	{
		c=0;
		fptr_timer();
	}
}



void Timer2_SetInterruptTime_us (uint16_t_ time,void(*LocalFptr)(void))
{
	 if(time <= 64)
	 {
		 OCR2 = (time * OCR2_8FREQ)-1;
		 TIMER2_OC_InterruptEnable();
		 Timer2_change(TIMER2_SCALER_8);
		 Timer2_OC_SetCallBack(LocalFptr);
		 
		 
	 }
	 
	 else
	 {
		 OCR2 = ((time/ OCR2_64TIMER))-1;
		 TIMER2_OC_InterruptEnable();
		 Timer2_change(TIMER2_SCALER_64);
		 Timer2_OC_SetCallBack(LocalFptr);
		 
	 }
}



