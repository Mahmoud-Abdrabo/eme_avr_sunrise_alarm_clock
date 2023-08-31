/*
 * app_config.h
 *
 * Created: 8/26/2023 3:48:45 PM
 *  Author: hacke
 */ 


#ifndef APP_CONFIG_H_
#define APP_CONFIG_H_


#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define  MINIMUM_DUTY    5
#define  MAXIMUM_DUTY    95
#define CURRENT_TIME (uint16_alarm_total_seconds - uint16_seconds_elapsed)

/* App Strings */
#define APP_STR_TITLE               "    Team GUCALEX"
#define APP_STR_SET_ALARM           "1. Set Alarm"
#define APP_STR_CANCEL_ALARM        "2. Cancel Alarm"
#define APP_STR_SHOW_ALARM          "3. Show Alarm"
#define APP_STR_ALARM_IS_CANCELLED  "Alarm Is Cancelled"
#define APP_STR_ALARM_SET           "Alarm Set"
#define APP_STR_ALARM_PLACEHOLDER   "_*:**"

/* App Delays */
#define APP_DELAY_MS_MSG_TIMEOUT 2000

#define KPD_SET_ALARM       '1'
#define KPD_CANCEL_ALARM    '2'
#define KPD_SHOW_ALARM      '3'
#define KPD_EXIT            'A'

#endif /* APP_CONFIG_H_ */