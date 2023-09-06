/*
 * app_config.h
 *
 * Created: 8/26/2023 3:48:45 PM
 *  Author: Hossam Elwahsh
 */ 


#ifndef APP_CONFIG_H_
#define APP_CONFIG_H_


#ifndef F_CPU
#define F_CPU (16000000UL)
#endif

#define APP_LED_DIM_STEP                (5)
#define APP_LED_MINIMUM_DUTY            (5)
#define APP_LED_MAXIMUM_DUTY            (95)
#define APP_RING_CYCLES_DURATION_MS     (50)
#define APP_RING_CYCLES_TILL_BUZZ_ON    (200)
#define APP_SNOOZE_1_DURATION_IN_MIN    (1)
#define APP_SNOOZE_2_DURATION_IN_MIN    (2)


/* App Strings */
#define APP_STR_TITLE               ((uint8_t_ *)"    Team GUCALEX")
#define APP_STR_SET_ALARM           ((uint8_t_ *)"1. Set Alarm")
#define APP_STR_CANCEL_ALARM        ((uint8_t_ *)"2. Cancel Alarm")
#define APP_STR_SHOW_ALARM          ((uint8_t_ *)"3. Show Alarm")
#define APP_STR_ALARM_IS_CANCELLED  ((uint8_t_ *)"Alarm Is Cancelled")
#define APP_STR_CHOOSE_ALARM        ((uint8_t_ *)"Choose Alarm: (0-9)")
#define APP_STR_ALARM               ((uint8_t_ *)"Alarm ")
#define APP_STR_ALARM_RINGING       ((uint8_t_ *)"WakeUp!")
#define APP_STR_ALARM_SNOOZE        ((uint8_t_ *)"Snooze (C) or (D)")
#define APP_STR_ALARM_SET           ((uint8_t_ *)"Alarm Set")
#define APP_STR_ALARM_DISABLED      ((uint8_t_ *)"Alarm is  disabled")
#define APP_STR_ALARM_PLACEHOLDER   ((uint8_t_ *)"_*:**")
#define APP_STR_ALARM_SLICER        (':')

/* App Delays */
#define APP_DELAY_MS_MSG_TIMEOUT (1000)

#define KPD_SET_ALARM       ('1')
#define KPD_CANCEL_ALARM    ('2')
#define KPD_SHOW_ALARM      ('3')
#define KPD_EXIT            ('A')
#define KPD_SNOOZE_1_MIN    ('C')
#define KPD_SNOOZE_2_MIN    ('D')

#endif /* APP_CONFIG_H_ */
