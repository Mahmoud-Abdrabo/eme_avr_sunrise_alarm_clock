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
    initial= 0,
    set_alarm,
    cancel_alarm,
    show_alarms
} app_state;

static void led_dimming(void);
static void app_switch_state(app_state state);
static void app_timer_tick_event(void);
static void app_keypad_initial_states(uint8_t value);
static void app_reset_timer();

/* Time elapsed */
static uint8_t uint8_seconds_elapsed= 0;

/* pressed/unpressed flag */
static boolean bool_is_pressed= FALSE;

/* Initial Alarm state */
app_state g_app_state= initial;

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
    buzz_inti();

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
    app_switch_state(initial);

    while (TRUE){
        /* Read LDR */
        ldr_read();

        /* Update is pressed flag */
        bool_is_pressed= LDR_PRESSED_THRESHOLD > LDR_VALUE;

        switch (g_app_state)
        {
            case initial:
            {

                break;
            }
            case set_alarm:
            {

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
                app_switch_state(initial);
                break;
            }
        }
    }
}




static void app_switch_state(app_state state)
{
    switch (state)
    {
        case initial:
        {
            /* Init LCD UI */
            LCD_clear();
            LCD_sendString(APP_STR_TITLE);
            LCD_setCursor(LCD_LINE1, LCD_COL0);
            LCD_sendString(APP_STR_SET_ALARM);
            LCD_setCursor(LCD_LINE2, LCD_COL0);
            LCD_sendString(APP_STR_CANCEL_ALARM);
            LCD_setCursor(LCD_LINE3, LCD_COL0);
            LCD_sendString(APP_STR_SHOW_ALARM);
            uint8_t kpd_value= KeyPad_GetValue();
            app_keypad_initial_states(kpd_value);
            break;
        }
        case set_alarm:
        {
            app_keypad_initial_states(SET_ALARM);
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

static void led_dimming(void)
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