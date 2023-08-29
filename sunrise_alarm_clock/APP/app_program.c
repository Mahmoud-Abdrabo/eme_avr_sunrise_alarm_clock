/*
 * app_program.c
 *
 * Created: 8/26/2023 3:29:07 PM
 *  Author: Team GUCALEX
 */


#include "Led.h"
#include "../MCAL/timer/Timers.h"
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

/* Time elapsed */
static uint16_t uint8_seconds_elapsed= 0;

/* pressed/unpressed flag */
static boolean bool_is_pressed= FALSE;

/* Global alarm index */
static uint8_t_ alarm_digit_index = 4;

/* Initial Alarm state */
app_state g_app_state= init;

/* Global alarm value */
uint16_t uint16_alarm_total_seconds=-1;

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
    // app_switch_state(initial);

    uint8_t kpd_value;

    while (TRUE)
    {

        /* get keypad current value */
        kpd_value = KeyPad_GetValue();

//        app_keypad_initial_states(kpd_value);

        /* Read LDR */
        ldr_read();

        /* Update is_pressed flag */
        bool_is_pressed= LDR_PRESSED_THRESHOLD > LDR_VALUE;

        switch (g_app_state)
        {
            case init:
            {
                app_switch_state(show_options);
                break;
            }
            case show_options:
            {
                /* if set button is pressed */
                if(SET_ALARM == kpd_value)
                {
                    /* Switch state to set alarm */
                    app_switch_state(set_alarm);
                }
                else if(CANCEL_ALARM == kpd_value)
                {
                    /* Switch state to cancel alarm */
                    app_switch_state(cancel_alarm);
                }
                else if(SHOW_ALARM == kpd_value)
                {
                    /* Switch state to show alarm */
                    app_switch_state(show_alarms);
                }
                else
                {
                    /* Do Nothing */
                }

                break;
            }
            case set_alarm:
            {
                /* Check for key press */
                if(0 != kpd_value)
                {
                    switch (kpd_value)
                    {
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
                                uint16_alarm_total_seconds = CONVERT_CHAR_TO_DIGIT(kpd_value) * 10 * 60;

                                /* Send Pressed CHAR */
                                LCD_sendChar(kpd_value);
                            }
                            else if(3 == alarm_digit_index)
                            {
                                uint16_alarm_total_seconds += CONVERT_CHAR_TO_DIGIT(kpd_value) * 60;
                                /* Send Pressed CHAR */
                                LCD_sendChar(kpd_value);
                                LCD_sendChar(':');
                            }
                            else if(2 == alarm_digit_index)
                            {
                                uint16_alarm_total_seconds += CONVERT_CHAR_TO_DIGIT(kpd_value) * 10;
                                /* Send Pressed CHAR */
                                LCD_sendChar(kpd_value);
                            }
                            else if(1 == alarm_digit_index)
                            {
                                uint16_alarm_total_seconds += CONVERT_CHAR_TO_DIGIT(kpd_value);
                                /* Send Pressed CHAR*/
                                LCD_sendChar(kpd_value);
                            }

                            /* update index */
                            DECREMENT(alarm_digit_index);

                            if(uint16_alarm_total_seconds==uint8_seconds_elapsed)
                            {

                            }
                            else
                            {

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
                uint16_alarm_total_seconds+=1;
                uint16_alarm_total_seconds+= KeyPad_GetValue() * 10 * 60 ;
                uint16_alarm_total_seconds+= KeyPad_GetValue() * 60 ;
                uint16_alarm_total_seconds+= KeyPad_GetValue() * 10 ;
                uint16_alarm_total_seconds+= KeyPad_GetValue();

                break;
            }
            case cancel_alarm:
            {

                break;
            }
            case show_alarms:
            {

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
            break;
        }
        case set_alarm:
        {
            LCD_clear();
            LCD_setCursor(LCD_LINE2, LCD_COL7);

            /* reset index flag */
            alarm_digit_index = 4;
            break;
        }
        case cancel_alarm:
        {
            app_keypad_initial_states(CANCEL_ALARM);
            break;
        }
        case show_alarms:
        {
            app_keypad_initial_states(SHOW_ALARM);
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
    uint8_seconds_elapsed++;
}

static void app_reset_timer(){
    /* Stop Timer */
    TIMER1_STOP();

    /* Reset Elapsed Time */
    uint8_seconds_elapsed;
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

static void app_keypad_initial_states(uint8_t value)
{
    switch (value)
    {
        /* Set Alarm for specific amount of time */
        case SET_ALARM:
        {
            LCD_clear();
            LCD_setCursor(LCD_LINE1, LCD_COL7);

            /* Write Time on LCD*/
            //todo

            /* Take this time and put it on Timer 1 */
            // todo

            TIMER1_START();

            g_app_state= set_alarm;

            break;
        }
        /* Cancel alarm and reset timer 1 to zero */
        case CANCEL_ALARM:
        {
            LCD_clear();
            LCD_setCursor(LCD_LINE1, LCD_COL1);
            LCD_sendString(APP_STR_ALARM_IS_CANCELLED);
            app_reset_timer();
            break;
        }
        /* Show Alarm and display the time counting */
        case SHOW_ALARM:
        {
            LCD_clear();
            LCD_setCursor(LCD_LINE1, LCD_COL7);
            /* Show the Time counting as 04:56 */
            // todo
            break;
        }
        default:
        {
            /* Do Nothing */
            break;
        }
    }
}