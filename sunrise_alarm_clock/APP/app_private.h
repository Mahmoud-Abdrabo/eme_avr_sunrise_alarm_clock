/*
 * Created by Hossam Elwahsh on 8/31/2023.
*/

#ifndef EME_AVR_SUNRISE_ALARM_CLOCK_APP_PRIVATE_H
#define EME_AVR_SUNRISE_ALARM_CLOCK_APP_PRIVATE_H

/* Includes */

/* HAL */
#include "Led.h"
#include "pwm_interface.h"
#include "lcd_interface.h"
#include "ldr_interface.h"
#include "buzz_interface.h"
#include "kpd_interface.h"

/* COMMON */
#include "std.h"
#include "interrupts.h"
#include "timer_interface.h"
#include "Timers_Services.h"

/* Macros */
#define APP_ALARM_DIGITS                (4)
#define APP_SUPPORTED_ALARMS_COUNT      (10)

/* Code Beautify Macros */
#define APP_INIT_SYS_TICK_TIMER()   timer1_init     (TIMER1_NORMAL_MODE); \
                                    timer1_ovf_interrupt_enable()
#define APP_SET_SYS_TICK_TIMER_CALLBACK(callback_fun_ptr) timer1_ovf_set_callback(callback_fun_ptr)
#define APP_INIT_DELAY_TIMER()      timer2_init     (TIMER2_NORMAL_MODE)
#define APP_START_SYS_TICK_TIMER()  timer1_change   (TIMER1_SCALER_256)

#define CURRENT_ALARM_SECONDS       st_gs_app_alarms_config.st_arr_app_alarms[uint8_gs_current_alarm_index].uint16_alarm_seconds
#define CURRENT_ALARM_IS_RINGING    st_gs_app_alarms_config.st_arr_app_alarms[uint8_gs_current_alarm_index].bool_alarm_ringing
#define CURRENT_ALARM_IS_ENABLED    st_gs_app_alarms_config.st_arr_app_alarms[uint8_gs_current_alarm_index].bool_alarm_enabled

#define GET_ALARM_SECONDS(index)       st_gs_app_alarms_config.st_arr_app_alarms[index].uint16_alarm_seconds
#define GET_ALARM_IS_RINGING(index)    st_gs_app_alarms_config.st_arr_app_alarms[index].bool_alarm_ringing
#define GET_ALARM_IS_ENABLED(index)    st_gs_app_alarms_config.st_arr_app_alarms[index].bool_alarm_enabled

#define SET_CURRENT_ALARM(val)          uint8_gs_current_alarm_index = val
#define GET_CURRENT_RINGING_ALARM()     uint8_gs_current_ringing_alarm
#define SET_CURRENT_RINGING_ALARM(val)  uint8_gs_current_ringing_alarm = val

#define GET_ENABLED_ALARMS_COUNT()        st_gs_app_alarms_config.uint8_enabled_alarms

#define CONVERT_CHAR_TO_DIGIT(char_val) (char_val - '0')
#define CONVERT_DIGIT_TO_CHAR(digit_val) (digit_val + '0')

/* Typedefs */

typedef struct
{
    uint16_t_ uint16_alarm_seconds;
    boolean bool_alarm_enabled;
    boolean bool_alarm_ringing;
}st_app_alarm_t;

typedef struct
{
    st_app_alarm_t st_arr_app_alarms[APP_SUPPORTED_ALARMS_COUNT];
    uint8_t_ uint8_enabled_alarms;
}st_app_alarms_config_t;

#endif/* EME_AVR_SUNRISE_ALARM_CLOCK_APP_PRIVATE_H */
