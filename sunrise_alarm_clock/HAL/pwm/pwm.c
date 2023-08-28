/*
 * pwm.c
 *
 * Created: 8/27/2023 1:50:36 PM
 *  Author: A M
 */ 
#include "pwm.h"


static uint8_t_ Led_Arr_Saved[NUMBEROFARRAY][NUMBEROFELEMNTS];

void Pwm_Init(uint8_t_ Led_Arr [][2]){
	uint8_t_ loop_on_array;
	uint8_t_ loop_on_elments;
	for (loop_on_array = 0; loop_on_array <NUMBEROFARRAY; ++loop_on_array) {
		for ( loop_on_elments = 0; loop_on_elments <NUMBEROFELEMNTS; ++loop_on_elments) {
			Led_Arr_Saved[loop_on_array][loop_on_elments] = Led_Arr[loop_on_array][loop_on_elments];
		}
	}
}
void timer_init(){
    TIMER0_Init(TIMER0_FASTPWM_MODE);
}
void Pwm_Start(){
    sei();
    TIMER0_OV_InterruptEnable();
    TIMER0_OC_InterruptEnable();
    Timer0_change(TIMER0_SCALER_8);


}

void Pwm_Stop(){
    Timer0_change(TIMER0_STOP);
}


void Set_Dutycycle(uint8_t_ percentage){
    OCR0=MAXMUNREGNUMBER*(percentage/100);
}

ISR(TIMER0_OVF_vect)
{
    uint8_t_ loop_on_array;
    for (loop_on_array= 0; loop_on_array < NUMBEROFARRAY; ++loop_on_array) {
	    Led_TurnOn(Led_Arr_Saved[loop_on_array][FIRSTINDEX],Led_Arr_Saved[loop_on_array][SECONDINDEX]);
	}

}

ISR(TIMER0_OC_vect)
{
    uint8_t_ loop_on_array;
    for (loop_on_array= 0; loop_on_array < NUMBEROFARRAY; ++loop_on_array) {
	    
	    Led_TurnOff(Led_Arr_Saved[loop_on_array][FIRSTINDEX],Led_Arr_Saved[loop_on_array][SECONDINDEX]);
	}

}