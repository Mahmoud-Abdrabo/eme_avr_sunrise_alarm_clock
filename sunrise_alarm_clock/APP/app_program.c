/*
 * app_program.c
 *
 * Created: 8/26/2023 3:29:07 PM
 *  Author: Team GUCALEX
 */


#include "Led.h"
#include "Timers.h"
#include "pwm.h"
#include "lcd_interface.h"
#include "app_interface.h"
#include "app_config.h"
void app_init()
{
	
	Led_Init(LED_RED_PORT, LED_RED_PIN);
	Led_Init(LED_BLUE_PORT, LED_BLUE_PIN);
	Led_Init(LED_GREEN_PORT, LED_GREEN_PIN);
	Led_Init(LED_YELLOW_PORT, LED_YELLOW_PIN);
	uint8_t_  Led_Arr[][2]= {{LED_RED_ARGS} ,{LED_GREEN_ARGS},{LED_BLUE_ARGS},{LED_YELLOW_ARGS}};
	
	Pwm_Init(Led_Arr);
	timer_init();
	Timer1_Init(TIMER1_NORMAL_MODE);
	Timer1_OVF_InterruptEnable();
	
	
}
void app_start()
{
	
}


void led_dimming(void)
{
	Pwm_Start();
	uint8_t_ loop;
	Pwm_Start();
	for (loop = MINMUN_DUTY ; loop<=MAXMUM_DUTY ; loop = loop+5)
	{
		Set_Dutycycle(loop);
		_delay_ms(30);
	}
	
	
	for (loop = MAXMUM_DUTY ; loop>=MINMUN_DUTY ; loop = loop-5)
	{
		Set_Dutycycle(loop);
		_delay_ms(30);
	}
	
	
}