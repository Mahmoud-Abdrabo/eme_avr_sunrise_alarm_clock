/*
 * pwm.c
 *
 * Created: 8/27/2023 1:50:36 PM
 *  Author: A M
 */ 
#include "pwm.h"
static uint8_t_ Led_Arr_Saved[4][2];
void Pwm_Init(uint8_t_** Led_Arr){
    for (int i = 0; i <4; ++i) {
        for (int j = 0; j <2; ++j) {
            Led_Arr_Saved[i][j]=Led_Arr[i][j];
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
    Timer0_change(0);
}
void Set_Dutycycle(uint8_t_ percentage){
    OCR0=256*(percentage/100);
}
ISR(TIMER0_OVF_vect)
{
    for (int i = 0; i < 4; ++i) {
            Led_TurnOn(Led_Arr_Saved[i][0],Led_Arr_Saved[i][1]);

    }

}
ISR(TIMER0_OC_vect)
{
    for (int i = 0; i < 4; ++i) {
        Led_TurnOff(Led_Arr_Saved[i][0],Led_Arr_Saved[i][1]);

    }

}