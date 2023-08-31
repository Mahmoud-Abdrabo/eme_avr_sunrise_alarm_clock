/*
 * app_program.c
 *
 * Created: 8/26/2023 3:29:07 PM
 *  Author: Team GUCALEX
 */


#include "Led.h"
#include "Timers.h"
#include "pwm_interface.h"
#include "lcd_interface.h"
#include "app_interface.h"
#include "app_config.h"
#include "ldr_interface.h"
#include "buzz_interface.h"
#include "kpd_interface.h"
#include "app_interface.h"
#include "app_config.h"
#include "app_private.h"


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


boolean bool_g_check_flag = FALSE;
/* Time elapsed */
static uint16_t uint16_seconds_elapsed = 0;

/* pressed/unpressed flag */
static boolean bool_is_pressed= FALSE;

/* Global alarm index */
static uint8_t_ alarm_digit_index = 4;

/* Initial Alarm state */
app_state g_app_state= init;

/* Global alarm value */
uint16_t uint16_alarm_total_seconds = 0;

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

	/* Init PWM */
    pwm_init();

	/* Init Buzz */
	buzz_init();

	/* Init KPD */
    keypad_init();

	/* Init Timer 1 with normal mode */
	Timer1_Init(TIMER1_NORMAL_MODE);

    INIT_DELAY_TIMER();

	/* Enable Timer 1 Overflow */
	Timer1_OVF_InterruptEnable();

	/* Enable Interrupt */
	sei();

	/* Set Timer Callback */
	Timer1_OVF_SetCallBack(app_timer_tick_event);
	
}
void app_start()
{
	uint8_t kpd_value = NULL;

	while (TRUE)
	{
		/* Alarm has finished and Leds start dimming for some seconds then buzz will start */
		if(
                (uint16_seconds_elapsed == uint16_alarm_total_seconds) &&
                (uint16_seconds_elapsed != ZERO)
                )
        {
            /* Reset timers */
			app_reset_timer();

            /* Loop over */
			for (int i = 1; i <= 8 ; ++i)
			{
				app_led_dimming();
			}
			buzz_start();
		}

		/* get keypad current value */
		kpd_value = keypad_read();

		/* Read LDR */
		ldr_read();

		/* Update is_pressed flag */
		bool_is_pressed = LDR_PRESSED_THRESHOLD > LDR_VALUE;

		if(TRUE == bool_is_pressed)
		{
            /* Stop PWM */
            pwm_stop();

            /* Turn off LEDs */
			Led_TurnOff(LED_RED_PORT, LED_RED_PIN);
			Led_TurnOff(LED_BLUE_PORT, LED_BLUE_PIN);
			Led_TurnOff(LED_GREEN_PORT, LED_GREEN_PIN);
			Led_TurnOff(LED_YELLOW_PORT, LED_YELLOW_PIN);

            /* Stop Buzzer */
            buzz_stop();

            /* Switch to show options UI/state */
            app_switch_state(show_options);
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
				if(FALSE == bool_g_check_flag)
                {
					app_switch_state(show_options);
                    bool_g_check_flag = TRUE;
				}
				else
				{
					/* if set button is pressed */
					if(KPD_SET_ALARM == kpd_value)
					{
                        bool_g_check_flag = FALSE;

						/* Switch state to set alarm */
						app_switch_state(set_alarm);
						
						
					}
					else if(KPD_CANCEL_ALARM == kpd_value)
					{
                        bool_g_check_flag = FALSE;
						/* Switch state to cancel alarm */
						app_switch_state(cancel_alarm);
						
						
					}
					else if(KPD_SHOW_ALARM == kpd_value)
					{
                        bool_g_check_flag = FALSE;
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
				/* Check for key press */
				if(NULL != kpd_value)
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
                                /* Show pressed key on LCD */
                                LCD_sendChar(kpd_value);
								uint16_alarm_total_seconds = CONVERT_CHAR_TO_DIGIT(kpd_value) * 10 * 60;

							}
							else if(3 == alarm_digit_index)
							{
								/* Show pressed key on LCD */
								LCD_sendChar(kpd_value);
								LCD_sendChar(':');
								uint16_alarm_total_seconds += CONVERT_CHAR_TO_DIGIT(kpd_value) * 60;
							}
							else if(2 == alarm_digit_index)
							{
                                /* Show pressed key on LCD */
								LCD_sendChar(kpd_value);
								uint16_alarm_total_seconds += CONVERT_CHAR_TO_DIGIT(kpd_value) * 10;

							}
							else if(1 == alarm_digit_index)
							{
                                /* Show pressed key on LCD */
								LCD_sendChar(kpd_value);
								uint16_alarm_total_seconds += CONVERT_CHAR_TO_DIGIT(kpd_value);
							}

							DECREMENT(alarm_digit_index);

							if(0 == alarm_digit_index)
							{
								Timer1_change(TIMER1_SCALER_256);
								g_app_state = show_options;


                                /* Show alarm set message */
                                LCD_setCursor(LCD_LINE3, LCD_COL0);
                                LCD_sendString(APP_STR_ALARM_SET);

                                /* Delay show msg to user alarm is set */
                                delay_ms(APP_DELAY_MS_MSG_TIMEOUT);

                                app_switch_state(show_options);
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
                /* Check if cancel is pressed */
                if(KPD_EXIT == kpd_value)
                {
                    app_switch_state(show_options);

                    /* Break current loop run */
                    break;
                }
                else
                {
                    /* Set Cursor */
                    LCD_setCursor(LCD_LINE2, LCD_COL7);

                    /* Update Timer Counter [UI] */
                    LCD_sendChar(CONVERT_DIGIT_TO_CHAR(CURRENT_TIME / 600));

                    /* Write Minutes Tenth Digit */
                    LCD_sendChar(CONVERT_DIGIT_TO_CHAR((CURRENT_TIME / 60) % 10));

                    /* Write Minutes Unit Digit */
//                    LCD_setCursor(LCD_LINE2, LCD_COL9);
                    LCD_sendChar(':');

                    /* Write Seconds Tenth Digit */
                    LCD_sendChar(CONVERT_DIGIT_TO_CHAR((CURRENT_TIME % 60) /10));

                    /* Write Seconds Unit Digit */
                    LCD_sendChar(CONVERT_DIGIT_TO_CHAR((CURRENT_TIME % 60) % 10));
                }

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

            /* Switch to show options */
            app_switch_state(show_options);
			break;
		}
		case show_options:
		{
            /* Clear LCD */
//            LCD_clear();

			/* LCD show options Menu */
			LCD_setCursor(LCD_LINE1, LCD_COL0);
			LCD_sendString(APP_STR_SET_ALARM);

			LCD_setCursor(LCD_LINE2, LCD_COL0);
			LCD_sendString(APP_STR_CANCEL_ALARM);

			LCD_setCursor(LCD_LINE3, LCD_COL0);
			LCD_sendString(APP_STR_SHOW_ALARM);

            /* Update global app state flag */
			g_app_state = show_options;
			break;
		}
		case set_alarm:
		{
			/* Clear LCD */
			LCD_clear();


            /* Show Title */
            LCD_sendString(APP_STR_TITLE);

            /* Set LCD Cursor Position */
			LCD_setCursor(LCD_LINE2, LCD_COL7);
            LCD_sendString("_*:**");
            LCD_setCursor(LCD_LINE2, LCD_COL7);

			/* reset index flag */
			alarm_digit_index = 4;

			g_app_state = set_alarm;
			break;
		}
		case cancel_alarm:
		{
            /* Clear LCD */
			LCD_clear();

            /* Inform user that alarm is cancelled */
			LCD_sendString(APP_STR_ALARM_IS_CANCELLED);

            /* Delay msg */
            _delay_ms(APP_DELAY_MS_MSG_TIMEOUT);

            /* LCD show title */
            LCD_sendString(APP_STR_TITLE);

            /* Reset app timers */
            app_reset_timer();

            /* Show options UI */
            app_switch_state(show_options);
			break;
		}
		case show_alarms:
		{
            /* Clear Screen */
			LCD_clear();

            /* Show Title */
            LCD_sendString(APP_STR_TITLE);

            /* Set LCD Cursor Position */
			LCD_setCursor(LCD_LINE2, LCD_COL7);

            /* Update global app state flag */
			g_app_state = show_alarms;
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
	uint16_seconds_elapsed = ZERO;

    /* Reset alarm time */
    uint16_alarm_total_seconds = ZERO;
}

static void app_led_dimming(void)
{
	/* Init variables */
    uint8_t_ u8_l_counter = MINIMUM_DUTY;

    /* Set PWM duty */
    pwm_set_duty(u8_l_counter);

    /* Start PWM */
    pwm_start();

    /* Lit LEDs up */
	for (u8_l_counter = MINIMUM_DUTY ; u8_l_counter <= MAXIMUM_DUTY ; u8_l_counter = u8_l_counter + 5)
	{
        pwm_set_duty(u8_l_counter);
		_delay_ms(50);
	}

    /* Dim LEDs down */
	for (u8_l_counter = MAXIMUM_DUTY ; u8_l_counter >= MINIMUM_DUTY ; u8_l_counter = u8_l_counter - 5)
	{
        pwm_set_duty(u8_l_counter);
		_delay_ms(50);
	}
}