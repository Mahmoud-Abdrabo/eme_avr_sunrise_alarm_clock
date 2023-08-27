/*
 * app_config.h
 *
 * Created: 8/26/2023 3:48:45 PM
 *  Author: hacke
 */ 


#ifndef APP_CONFIG_H_
#define APP_CONFIG_H_

#define APP_IS_NIGHT    0           // set day/night

#define APP_INTERIOR_LIGHT_ARGS         LED_RED_ARGS
#define APP_INTERIOR_LIGHT_PORT         LED_RED_PIN
#define APP_INTERIOR_LIGHT_PIN          LED_RED_PORT

#define APP_CAR_FRONT_LEFT_LIGHT_ARGS   LED_BLUE_ARGS
#define APP_CAR_FRONT_LEFT_LIGHT_PIN    LED_BLUE_PIN
#define APP_CAR_FRONT_LEFT_LIGHT_PORT   LED_BLUE_PORT

#define APP_CAR_FRONT_RIGHT_LIGHT_ARGS  LED_GREEN_ARGS
#define APP_CAR_FRONT_RIGHT_LIGHT_PIN   LED_GREEN_PIN
#define APP_CAR_FRONT_RIGHT_LIGHT_PORT  LED_GREEN_PORT

/* App Strings */
#define APP_STR_TITLE       "    Team GUCALEX"
#define APP_STR_CAR_OFF     "Welcome .. GUCALEX"
#define APP_STR_CAR_READY   "Please start Motor"
#define APP_STR_CAR_RUNNING   "Car Motor Running."

#define APP_CAR_BATTERY_TIMEOUT_IN_SEC  10

#endif /* APP_CONFIG_H_ */