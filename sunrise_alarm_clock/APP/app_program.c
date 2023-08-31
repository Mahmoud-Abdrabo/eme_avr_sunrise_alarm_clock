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
#include "Led.h"
#include "Timers.h"
#include "pwm.h"
#include "ldr.h"
#include "lcd_interface.h"
#include "buzz_interface.h"
#include "kpd_interface.h"
#include "app_interface.h"
#include "app_config.h"




typedef enum
{
	init= 0,
	show_options,
	set_alarm,
	cancel_alarm,
	show_alarms
} app_state;

static void app_led_dimming(void);
static void app_switch_state(app_state state);
static void app_timer_tick_event(void);
static void app_keypad_initial_states(uint8_t value);
static void app_reset_timer();


uint8_t check=0;
/* Time elapsed */
static uint16_t uint16_seconds_elapsed= 0;

/* pressed/unpressed flag */
static boolean bool_is_pressed= FALSE;

/* Global alarm index */
static uint8_t_ alarm_digit_index = 4;

/* Initial Alarm state */
app_state g_app_state= init;

/* Global alarm value */
uint16_t uint16_alarm_total_seconds= 0;

void app_init()
{
	
	/* Init LCD */
	LCD_init();

	/* Init LDR */
	ldr_init();

	/* Init LEDs */
	Led_Init(LED_RED_PORT, LED_RED_PIN);
	Led_Init(LED_BLUE_PORT, LED_BLUE_PIN);
	Led_Init(LED_GREEN_PORT, LED_GREEN_PIN);
	Led_Init(LED_YELLOW_PORT, LED_YELLOW_PIN);
	uint8_t_  Led_Arr[][2]= {{LED_RED_ARGS} ,{LED_GREEN_ARGS},{LED_BLUE_ARGS},{LED_YELLOW_ARGS}};

	/* Init PWM */
	Pwm_Init(Led_Arr);

	/* Init Buzz */
	buzz_init();

	/* Init KPD */
	KeyPad_Init();

	/* Init Timer 0 with Fast PWM mode */
	timer_init();

	/* Init Timer 1 with normal mode */
	Timer1_Init(TIMER1_NORMAL_MODE);

	/* Enable Timer 1 Overflow */
	Timer1_OVF_InterruptEnable();

	/* Enable Interrupt */
	sei();

	/* Set Timer Callback */
	Timer1_OVF_SetCallBack(app_timer_tick_event);
	
}
void app_start()
{
	uint8_t kpd_value='A';
	//uint8_t time='A';

	while (TRUE)
	{
		/* Alarm has finished and Leds start dimming for some seconds then buzz will start */
		if(uint16_seconds_elapsed == uint16_alarm_total_seconds && uint16_seconds_elapsed !=0){
			app_reset_timer();
			for (int i = 1; i <=8 ; ++i)
			{
				app_led_dimming();
			}
			buzz_start();
		}

		/* get keypad current value */
		kpd_value = KeyPad_GetValue();

		/* Read LDR */
		ldr_read();

		/* Update is_pressed flag */
		
		bool_is_pressed= LDR_PRESSED_THRESHOLD > LDR_VALUE;


		if(TRUE == bool_is_pressed)
		{
			Pwm_Stop();
			Led_TurnOff(LED_RED_PORT, LED_RED_PIN);
			Led_TurnOff(LED_BLUE_PORT, LED_BLUE_PIN);
			Led_TurnOff(LED_GREEN_PORT, LED_GREEN_PIN);
			Led_TurnOff(LED_YELLOW_PORT, LED_YELLOW_PIN);
			buzz_stop();
			g_app_state= show_options;
		}
		else
		{
			/* Do Nothing */
		}

		switch (g_app_state)
		{
			case init:
			{
				app_switch_state(init);
				break;
			}
			case show_options:
			{
				if(check==0){
					app_switch_state(show_options);
					check=1;
				}
				else
				{
					/* if set button is pressed */
					if(SET_ALARM == kpd_value)
					{
						check=0;
					//	kpd_value='A';
						/* Switch state to set alarm */
						app_switch_state(set_alarm);
						
						
					}
					else if(CANCEL_ALARM == kpd_value)
					{
						check=0;
						//kpd_value='A';
						/* Switch state to cancel alarm */
						app_switch_state(cancel_alarm);
						
						
					}
					else if(SHOW_ALARM == kpd_value)
					{
						check=0;
						//kpd_value='A';
						/* Switch state to show alarm */
						app_switch_state(show_alarms);
						
					}
					else
					{
						/* Do Nothing */
					}
					break;
				}
				
			}
			case set_alarm:
			{
				/* get keypad current value */
				//kpd_value = KeyPad_GetValue();
				/* Check for key press */
				
				if('A' != kpd_value)
				{
					switch (kpd_value)
					{
						case '0':
						case '1':
						case '2':
						case '3':
						case '4':
						case '5':
						case '6':
						case '7':
						case '8':
						case '9':
						{
							
							
							/* Parse Numbers */
							if(4 == alarm_digit_index)
							{
								/* Send Pressed CHAR */
								LCD_sendChar(kpd_value);
								uint16_alarm_total_seconds = CONVERT_CHAR_TO_DIGIT(kpd_value) * 10 * 60;
								/* update index */
								
							//	kpd_value='A';

								
							}
							else if(3 == alarm_digit_index)
							{
								/* Send Pressed CHAR */
								LCD_sendChar(kpd_value);
								LCD_sendChar(':');
								uint16_alarm_total_seconds += CONVERT_CHAR_TO_DIGIT(kpd_value) * 60;
								/* update index */
								
								//kpd_value='A';
								
							}
							else if(2 == alarm_digit_index)
							{
								/* Send Pressed CHAR */
								LCD_sendChar(kpd_value);
								uint16_alarm_total_seconds += CONVERT_CHAR_TO_DIGIT(kpd_value) * 10;
								/* update index */
								
								//kpd_value='A';
								
							}
							else if(1 == alarm_digit_index)
							{
								/* Send Pressed CHAR*/
								LCD_sendChar(kpd_value);
								uint16_alarm_total_seconds += CONVERT_CHAR_TO_DIGIT(kpd_value);
								/* update index */
								
								//kpd_value='A';
								
							}

							DECREMENT(alarm_digit_index);

							if(0 == alarm_digit_index)
							{
								Timer1_change(TIMER1_SCALER_256);
								g_app_state= show_options;
							}
							else
							{
								/* Do Nothing */
							}
							break;
						}
						default:
						{
							/* Skip */
							break;
						}
					}
				}
				else
				{
					/* Do Nothing no key is pressed */
				}
				break;
			}
			case cancel_alarm:
			{
				app_switch_state(cancel_alarm);
				break;
			}
			case show_alarms:
			{
				app_switch_state(show_alarms);

				LCD_sendChar(CONVERT_DIGIT_TO_CHAR(CURRENT_TIME / 600));

				LCD_setCursor(LCD_LINE2, LCD_COL8);
				LCD_sendChar(CONVERT_DIGIT_TO_CHAR((CURRENT_TIME / 60) % 10));

				LCD_setCursor(LCD_LINE2, LCD_COL9);
				LCD_sendChar(':');

				LCD_setCursor(LCD_LINE2, LCD_COL10);
				LCD_sendChar(CONVERT_DIGIT_TO_CHAR((CURRENT_TIME % 60) /10));

				LCD_setCursor(LCD_LINE2, LCD_COL11);
				LCD_sendChar(CONVERT_DIGIT_TO_CHAR((CURRENT_TIME % 60) % 10));

				break;
			}
			default:
			{
				/* Reset state to initial state */
				app_switch_state(init);
				break;
			}
		}
	}
}

static void app_switch_state(app_state state)
{
	switch (state)
	{
		case init:
		{
			LCD_clear();
			LCD_sendString(APP_STR_TITLE);
			g_app_state= show_options;
			break;
		}
		case show_options:
		{
			/* LCD show options Menu */
			LCD_setCursor(LCD_LINE1, LCD_COL0);
			LCD_sendString(APP_STR_SET_ALARM);
			LCD_setCursor(LCD_LINE2, LCD_COL0);
			LCD_sendString(APP_STR_CANCEL_ALARM);
			LCD_setCursor(LCD_LINE3, LCD_COL0);
			LCD_sendString(APP_STR_SHOW_ALARM);
			g_app_state= show_options;
			break;
		}
		case set_alarm:
		{
			
			LCD_clear();
			
			LCD_setCursor(LCD_LINE2, LCD_COL7);

			/* reset index flag */
			alarm_digit_index = 4;

			g_app_state= set_alarm;
			break;
		}
		case cancel_alarm:
		{
			
			LCD_clear();
			LCD_sendString(APP_STR_TITLE);
			app_reset_timer();
			g_app_state= show_options;
			break;
		}
		case show_alarms:
		{
			_delay_ms(500);
			LCD_clear();
			
			LCD_setCursor(LCD_LINE2, LCD_COL7);
			g_app_state= show_alarms;
			break;
		}
		default:
		{
			/* Do Nothing */
			break;
		}
	}
}

static void app_timer_tick_event(void)
{
	/* Timer ticked 1 second */
	uint16_seconds_elapsed++;
}

static void app_reset_timer()
{
	/* Stop Timer */
	TIMER1_STOP();

	/* Reset Elapsed Time */
	uint16_seconds_elapsed=0;
}

static void app_led_dimming(void)
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