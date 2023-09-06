/*
 * Created by Hossam Elwahsh on 8/31/2023.
*/


#include "pwm_linking_config.h"
#include "Led_Cfg.h"

uint8_t_ pwm_gs_pwm_pins[PWM_PINS_COUNT][PWM_PINS_ITEMS_COUNT] =
        {
                {LED_RED_ARGS},
                {LED_GREEN_ARGS},
                {LED_BLUE_ARGS},
                {LED_YELLOW_ARGS}
        };