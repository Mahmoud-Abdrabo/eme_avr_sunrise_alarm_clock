/*
 * app_program.c
 *
 * Created: 8/26/2023 3:29:07 PM
 *  Author: Team GUCALEX
 */

/* App Includes */
#include "app_interface.h"
#include "app_config.h"
#include "app_private.h"

typedef enum
{
	APP_STATE_INIT          =   0   ,
	APP_STATE_SHOW_OPTIONS          ,
	APP_STATE_CHOOSE_ALARM          ,
	APP_STATE_SET_ALARM             ,
	APP_STATE_CANCEL_ALARM          ,
	APP_STATE_SHOW_ALARM            ,
    APP_STATE_ALARM_RINGING         ,
    APP_STATE_TOTAL
}en_app_state_t;

static void app_switch_state        (en_app_state_t state)  ;
static void app_timer_tick_event    (void)                  ;

static uint8_t_                 uint8_gs_current_alarm_index    =   ZERO            ;
static uint8_t_                 uint8_gs_current_ringing_alarm  =   ZERO            ;
static uint16_t                 uint16_seconds_elapsed          =   ZERO            ;

/* Flags */
static boolean                  bool_gs_stop_alarm_pressed      =   FALSE           ;

/* App state */
static en_app_state_t           en_gs_app_state                 =   APP_STATE_INIT  ;
static en_app_state_t           en_gs_next_app_state            =   APP_STATE_INIT  ;

/* App Alarms */
static st_app_alarms_config_t   st_gs_app_alarms_config                             ;

/**
 * @brief   : Initializes Application
 */
void app_init()
{

	/* Init LCD */
	LCD_init();

	/* Init LDR */
	ldr_init();

	/* Init LEDs */
	Led_Init(LED_RED_PORT   , LED_RED_PIN   );
	Led_Init(LED_BLUE_PORT  , LED_BLUE_PIN  );
	Led_Init(LED_GREEN_PORT , LED_GREEN_PIN );
	Led_Init(LED_YELLOW_PORT, LED_YELLOW_PIN);

	/* Init PWM */
    pwm_init();

	/* Init Buzz */
	buzz_init();

	/* Init KPD */
    keypad_init();

    /* Init timers */
    APP_INIT_SYS_TICK_TIMER();
    APP_INIT_DELAY_TIMER();

    /* Set callbacks */
    APP_SET_SYS_TICK_TIMER_CALLBACK(app_timer_tick_event);

	/* Enable Interrupt */
	sei();
}

/**
 * @brief   :   Start Application Logic / Superloop
 */
void app_start(void)
{
    /* Local Variables */
    uint8_t uint8_kpd_value         ,
            uint8_current_pwm_duty  ,
            uint8_loop_counter      ,
            uint8_alarm_digit_index ;

    /* Loop counter for simulating LEDs */
    uint16_t_ uint16_led_dimmer_counter ;

    /* Flag for LEDs dimming, TRUE: LEDs fading out, FALSE: LEDs lighting up */
    boolean bool_is_dimming_down;

    /* Init Local Variables */
    uint8_kpd_value             = NULL                      ;
    uint16_led_dimmer_counter   = ZERO                      ;
    uint8_alarm_digit_index     = APP_ALARM_DIGITS          ;
    uint8_current_pwm_duty      = APP_LED_MINIMUM_DUTY      ;
    bool_is_dimming_down        = FALSE                     ;

    /* Start sys tick timer */
    APP_START_SYS_TICK_TIMER();

	while (TRUE)
	{
        /* Check and update alarms ticks */
        for (uint8_loop_counter = 0; uint8_loop_counter < APP_SUPPORTED_ALARMS_COUNT; ++uint8_loop_counter)
        {
            if(
                    (TRUE == GET_ALARM_IS_ENABLED(uint8_loop_counter)) &&
                    (FALSE == GET_ALARM_IS_RINGING(uint8_loop_counter))
                    )
            {
                if(     (ZERO == GET_ALARM_SECONDS(uint8_loop_counter)) &&
                        (en_gs_app_state != APP_STATE_ALARM_RINGING) /* no other alarm is currently ringing */
                        )
                {
                    /* Update current Alarm */
                    SET_CURRENT_RINGING_ALARM(uint8_loop_counter);

                     /* Set alarm to ringing */
                    GET_ALARM_IS_RINGING(uint8_loop_counter) = TRUE;

                     /* Reset led dimming flags */
                    uint16_led_dimmer_counter   = ZERO                  ;
                    uint8_current_pwm_duty      = APP_LED_MINIMUM_DUTY  ;
                    bool_is_dimming_down        = FALSE                 ;

                     /* Set PWM duty */
                    pwm_set_duty(uint8_current_pwm_duty);

                     /* Start PWM */
                    pwm_start();

                     /* Switch to ringing mode */
                    app_switch_state(APP_STATE_ALARM_RINGING);
                    break;

                }
                else
                {
                     /* Do Nothing */
                }
            } /* end if */
            else
            {
                /* Continue */
            } /* end else */
        } /* end for loop */

        /* get keypad current value */
		uint8_kpd_value = keypad_read();

		/* Read LDR */
		ldr_read();

		switch (en_gs_app_state)
		{
			case APP_STATE_INIT:
			{
				app_switch_state(APP_STATE_INIT);
				break;
			}
			case APP_STATE_SHOW_OPTIONS:
			{
                /* if set button is pressed */
                if(KPD_SET_ALARM == uint8_kpd_value)
                {
                    /* Update flag */
                    en_gs_next_app_state = APP_STATE_SET_ALARM;

                    /* Switch state to choose alarm to select which alarm to set */
                    app_switch_state(APP_STATE_CHOOSE_ALARM);
                }
                else if(KPD_CANCEL_ALARM == uint8_kpd_value)
                {
                    /* Update flag */
                    en_gs_next_app_state = APP_STATE_CANCEL_ALARM;

                    /* Switch app state  */
                    app_switch_state(APP_STATE_CHOOSE_ALARM);

                }
                else if(KPD_SHOW_ALARM == uint8_kpd_value)
                {
                    /* Update flag */
                    en_gs_next_app_state = APP_STATE_SHOW_ALARM;

                    /* Switch app state  */
                    app_switch_state(APP_STATE_CHOOSE_ALARM);

                }
                else
                {
                    /* Do Nothing */
                }
                break;
            }
			case APP_STATE_SET_ALARM:
			{
				/* Check for key press */
				if(NULL != uint8_kpd_value)
				{
                    if((uint8_kpd_value >= '0') && (uint8_kpd_value <= '9'))
                    {
                        /* Parse Numbers */
                        if(4 == uint8_alarm_digit_index)
                        {
                            /* Show pressed key on LCD */
                            LCD_sendChar(uint8_kpd_value);
                            CURRENT_ALARM_SECONDS = CONVERT_CHAR_TO_DIGIT(uint8_kpd_value) * 10 * 60;

                            /* Decrement Digit index */
                            DECREMENT(uint8_alarm_digit_index);
                        }
                        else if(3 == uint8_alarm_digit_index)
                        {
                            /* Show pressed key on LCD */
                            LCD_sendChar(uint8_kpd_value);
                            LCD_sendChar(':');
                            CURRENT_ALARM_SECONDS += CONVERT_CHAR_TO_DIGIT(uint8_kpd_value) * 60;

                            /* Decrement Digit index */
                            DECREMENT(uint8_alarm_digit_index);
                        }
                        else if((2 == uint8_alarm_digit_index) && (uint8_kpd_value <= '5'))
                        {
                            /* Show pressed key on LCD */
                            LCD_sendChar(uint8_kpd_value);
                            CURRENT_ALARM_SECONDS += CONVERT_CHAR_TO_DIGIT(uint8_kpd_value) * 10;

                            /* Decrement Digit index */
                            DECREMENT(uint8_alarm_digit_index);
                        }
                        else if(1 == uint8_alarm_digit_index)
                        {
                            /* Show pressed key on LCD */
                            LCD_sendChar(uint8_kpd_value);
                            CURRENT_ALARM_SECONDS += CONVERT_CHAR_TO_DIGIT(uint8_kpd_value);

                            /* Decrement Digit index */
                            DECREMENT(uint8_alarm_digit_index);
                        }

                        if(0 == uint8_alarm_digit_index)
                        {
                            /* Increment enabled alarms */
                            INCREMENT(GET_ENABLED_ALARMS_COUNT());

                            /* Enable Alarm */
                            CURRENT_ALARM_IS_ENABLED = TRUE;
                            CURRENT_ALARM_IS_RINGING = FALSE;

                            /* Show alarm set message */
                            LCD_setCursor(LCD_LINE3, LCD_COL0);
                            LCD_sendString(APP_STR_ALARM_SET);

                            /* Delay show msg to user alarm is set */
                            delay_ms(APP_DELAY_MS_MSG_TIMEOUT);

                            app_switch_state(APP_STATE_SHOW_OPTIONS);
                        }
                        else
                        {
                            /* Do Nothing */
                        }
                    }
                    else
                    {
                        /* Do Nothing */
                    }
				}
				else
				{
					/* Do Nothing no key is pressed */
				}
				break;
			}
			case APP_STATE_CANCEL_ALARM:
			{
                /* Do Nothing */
				break;
			}
			case APP_STATE_SHOW_ALARM:
			{
                /* Check if cancel is pressed */
                if(KPD_EXIT == uint8_kpd_value)
                {
                    /* Clear LCD */
                    LCD_shiftClear();

                    app_switch_state(APP_STATE_SHOW_OPTIONS);

                    /* Break current loop run */
                    break;
                }
                else
                {
                    /* Set Cursor */
                    LCD_setCursor(LCD_LINE2, LCD_COL7);

                    /* Update Timer Counter [UI] */
                    LCD_sendChar(CONVERT_DIGIT_TO_CHAR(CURRENT_ALARM_SECONDS / 600));

                    /* Write Minutes Tenth Digit */
                    LCD_sendChar(CONVERT_DIGIT_TO_CHAR((CURRENT_ALARM_SECONDS / 60) % 10));

                    /* Write Minutes Unit Digit */
//                    LCD_setCursor(LCD_LINE2, LCD_COL9);
                    LCD_sendChar(':');

                    /* Write Seconds Tenth Digit */
                    LCD_sendChar(CONVERT_DIGIT_TO_CHAR((CURRENT_ALARM_SECONDS % 60) /10));

                    /* Write Seconds Unit Digit */
                    LCD_sendChar(CONVERT_DIGIT_TO_CHAR((CURRENT_ALARM_SECONDS % 60) % 10));
                }

				break;
			}
            case APP_STATE_CHOOSE_ALARM:
            {
                /* Reset digit index to prepare for input */
                uint8_alarm_digit_index = APP_ALARM_DIGITS;

                /* Validate keypad input */
                if((uint8_kpd_value >= '0') && (uint8_kpd_value <= '9'))
                {
                    /* set current alarm with keypad input */
                    SET_CURRENT_ALARM(CONVERT_CHAR_TO_DIGIT(uint8_kpd_value));

                    if(APP_STATE_SET_ALARM == en_gs_next_app_state)
                    {
                        app_switch_state(APP_STATE_SET_ALARM);
                    }
                    else if(APP_STATE_SHOW_ALARM == en_gs_next_app_state)
                    {
                        app_switch_state(APP_STATE_SHOW_ALARM);
                    }
                    else if(APP_STATE_CANCEL_ALARM == en_gs_next_app_state)
                    {
                        app_switch_state(APP_STATE_CANCEL_ALARM);
                    }
                }
                else
                {
                    /* Do Nothing no correct key is pressed */
                }
                break;
            }
            case APP_STATE_ALARM_RINGING:
            {
                /* Wait for user to stop alarm */
                /* Update is_pressed flag */
                bool_gs_stop_alarm_pressed = LDR_PRESSED_THRESHOLD > LDR_VALUE;

                if(TRUE == bool_gs_stop_alarm_pressed)
                {
                    /* Stop PWM */
                    pwm_stop();

                    /* Turn off LEDs */
                    Led_TurnOff(LED_RED_PORT, LED_RED_PIN);
                    Led_TurnOff(LED_BLUE_PORT, LED_BLUE_PIN);
                    Led_TurnOff(LED_GREEN_PORT, LED_GREEN_PIN);
                    Led_TurnOff(LED_YELLOW_PORT, LED_YELLOW_PIN);

                    /* Stop Buzzer */
                    buzz_off();

//                    /* stop ringing */
//                    bool_gs_alarm_ringing = FALSE;

                    /* Turn off and reset alarm */
                    GET_ALARM_SECONDS(GET_CURRENT_RINGING_ALARM())       = ZERO  ;
                    GET_ALARM_IS_ENABLED(GET_CURRENT_RINGING_ALARM())    = FALSE ;
                    GET_ALARM_IS_RINGING(GET_CURRENT_RINGING_ALARM())    = FALSE ;

                    /* Decrement enabled alarms count */
                    DECREMENT(GET_ENABLED_ALARMS_COUNT());

                    /* Switch to show options UI/state */
                    app_switch_state(APP_STATE_SHOW_OPTIONS);
                }
                else
                {
                    /* Keep Ringing Algorithm Running */

                    /* Increment Duty */
                    if(TRUE == bool_is_dimming_down)
                    {
                        uint8_current_pwm_duty -= APP_LED_DIM_STEP;
                    }
                    else
                    {
                        uint8_current_pwm_duty += APP_LED_DIM_STEP;
                    }

                    /* Update dim/lit direction if needed */
                    if(uint8_current_pwm_duty >= APP_LED_MAXIMUM_DUTY)
                    {
                        bool_is_dimming_down = TRUE;
                    }
                    else if(uint8_current_pwm_duty == ZERO)
                    {
                        bool_is_dimming_down = FALSE;
                    }

                    /* Update PWM Duty */
                    pwm_set_duty(uint8_current_pwm_duty);

                    INCREMENT(uint16_led_dimmer_counter);

                    if(uint16_led_dimmer_counter > APP_RING_CYCLES_TILL_BUZZ_ON)
                    {
                        /* start buzzer */
                        buzz_on();
                    }

                    /* Delay PWM */
                    delay_ms(APP_RING_CYCLES_DURATION_MS);
                }
                break;
            }
            case APP_STATE_TOTAL:
            {
                /* Reset state to initial state */
                app_switch_state(APP_STATE_INIT);
                break;
            }
            default:
            {
                /* Reset state to initial state */
                app_switch_state(APP_STATE_INIT);
                break;
            }
        }
	}
}

static void app_switch_state(en_app_state_t state)
{
	switch (state)
	{
		case APP_STATE_INIT:
		{
			LCD_clear();
			LCD_sendString(APP_STR_TITLE);

            /* Switch to show options */
            app_switch_state(APP_STATE_SHOW_OPTIONS);
			break;
		}
		case APP_STATE_SHOW_OPTIONS:
		{
            /* Clear LCD */
            LCD_shiftClear();

            /* Show Title */
            LCD_sendString(APP_STR_TITLE);

			/* LCD show options Menu */
			LCD_setCursor(LCD_LINE1, LCD_COL0);
			LCD_sendString(APP_STR_SET_ALARM);

			LCD_setCursor(LCD_LINE2, LCD_COL0);
			LCD_sendString(APP_STR_CANCEL_ALARM);

			LCD_setCursor(LCD_LINE3, LCD_COL0);
			LCD_sendString(APP_STR_SHOW_ALARM);

            /* Update global app state flag */
			en_gs_app_state = APP_STATE_SHOW_OPTIONS;
			break;
		}
		case APP_STATE_SET_ALARM:
		{
			/* Clear LCD */
			LCD_clear();


            /* Show Title */
            LCD_sendString(APP_STR_TITLE);

            /* Set LCD Cursor Position */
			LCD_setCursor(LCD_LINE2, LCD_COL7);
            LCD_sendString(APP_STR_ALARM_PLACEHOLDER);
            LCD_setCursor(LCD_LINE2, LCD_COL7);

            en_gs_app_state = APP_STATE_SET_ALARM;
			break;
		}
		case APP_STATE_CANCEL_ALARM:
		{
            /* Clear LCD */
			LCD_clear();

            /* Inform user that alarm is cancelled */
			LCD_sendString(APP_STR_ALARM_IS_CANCELLED);

            /* Delay msg */
            _delay_ms(APP_DELAY_MS_MSG_TIMEOUT);

            /* LCD show title */
            LCD_sendString(APP_STR_TITLE);

            /* Reset app alarm */
            CURRENT_ALARM_IS_ENABLED = FALSE;
            CURRENT_ALARM_IS_RINGING = FALSE;
            CURRENT_ALARM_SECONDS    = ZERO ;

            /* Show options UI */
            app_switch_state(APP_STATE_SHOW_OPTIONS);
			break;
		}
		case APP_STATE_SHOW_ALARM:
		{
            /* Clear Screen */
			LCD_clear();

            /* Show Title */
            LCD_sendString(APP_STR_TITLE);

            if(FALSE == CURRENT_ALARM_IS_ENABLED)
            {
                /* Set LCD Cursor Position */
                LCD_setCursor(LCD_LINE3, LCD_COL1);
                /* Show Alarm Disabled Msg */
                LCD_sendString(APP_STR_ALARM_DISABLED);
            }
            /* Update global app state flag */
			en_gs_app_state = APP_STATE_SHOW_ALARM;
			break;
		}
        case APP_STATE_CHOOSE_ALARM:
        {
            /* Clear Screen */
            LCD_clear();

            /* Show Title */
            LCD_sendString(APP_STR_TITLE);

            LCD_setCursor(LCD_LINE2, LCD_COL0);
            LCD_sendString(APP_STR_CHOOSE_ALARM);

            /* update global app state flag */
            en_gs_app_state = APP_STATE_CHOOSE_ALARM;
            break;
        }
        case APP_STATE_ALARM_RINGING:
        {
            /* Clear Screen */
            LCD_clear();

            /* Show Ringing Message */
            LCD_sendString(APP_STR_ALARM);
            LCD_sendChar(CONVERT_DIGIT_TO_CHAR(GET_CURRENT_RINGING_ALARM()));

            LCD_setCursor(LCD_LINE2, LCD_COL6);
            LCD_sendString(APP_STR_ALARM_RINGING);

            en_gs_app_state = APP_STATE_ALARM_RINGING;
            break;

        }
        case APP_STATE_TOTAL:
        {
            /* bad state reset */
            app_switch_state(APP_STATE_SHOW_OPTIONS);
            break;
        }
        default:
        {
            /* bad state reset */
            app_switch_state(APP_STATE_SHOW_OPTIONS);
            break;
        }
    }
}

static void app_timer_tick_event(void)
{
	/* Timer ticked 1 second */
	uint16_seconds_elapsed++;

    /* Update alarms times */
    for (uint8_t_ uint8_alarm_index = 0; uint8_alarm_index < APP_SUPPORTED_ALARMS_COUNT; ++uint8_alarm_index)
    {

        /* Check if alarm is enabled and not ringing */
        if(
                (TRUE == GET_ALARM_IS_ENABLED(uint8_alarm_index)) &&
                (FALSE == GET_ALARM_IS_RINGING(uint8_alarm_index))
                )
        {
            if(ZERO != GET_ALARM_SECONDS(uint8_alarm_index))
            {
                /* Decrement alarm seconds tick */
                DECREMENT(GET_ALARM_SECONDS(uint8_alarm_index));
            }
            else
            {
                /* Do Nothing */
            }
        }
    }
}
