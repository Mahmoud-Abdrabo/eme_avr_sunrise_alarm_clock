/*
 * Timers_Services.c
 *
 * Created: 8/29/2023 10:33:08 AM
 *  Author: ahmed
 */ 



#include "mem_map.h"
#include "std.h"
#include "Timers_Services.h"




void Timer2_SetInterruptTime_ms (uint16_t_ time)
{
	TIMER2_OC_InterruptDisable();
	OCR2 = OCR2_1U_OVERFLOW; //count 1m
	Timer2_change(TIMER2_SCALER_64);	
	while(time!=LOW)
	{
		 while(GET_BIT(TIFR,OCF2)==LOW);
		 SET_BIT(TIFR,OCF2);
		 
		 time--;
	}
}





void Timer2_SetInterruptTime_us (uint16_t_ time)
{
	TIMER2_OC_InterruptDisable();
	
	 if(time <= OVERFLOW_TIME_8PRESCALER)
	 {
		 
		 OCR2 = (time * OCR2_8FREQ)-1;
		
		 Timer2_change(TIMER2_SCALER_8);
		 while(GET_BIT(TIFR,OCF2)==LOW);
		 SET_BIT(TIFR,OCF2);
		
		 
	 }
	 
	 else
	 {
		 
		 OCR2 = ((time/ OCR2_64TIMER))-1;
		 Timer2_change(TIMER2_SCALER_64);
		 while(GET_BIT(TIFR,OCF2)==LOW);
		  SET_BIT(TIFR,OCF2);
		 
	 }
	
}



