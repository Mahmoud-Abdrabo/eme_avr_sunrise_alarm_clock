/*
 * pwm.c
 *
 * Created: 8/27/2023 1:50:36 PM
 *  Author: A M
 */ 
#include "pwm_interface.h"
#include "pwm_linking_config.h"

void pwm_init(void)
{
    /* Declare local variable */
	uint8_t_ loop_on_array;
	uint8_t_ loop_on_elments;

    /* Init timer in fast pwm mode */
    TIMER0_Init(TIMER0_FASTPWM_MODE);

	for (loop_on_array = 0; loop_on_array < PWM_PINS_COUNT; ++loop_on_array)
    {
		for ( loop_on_elments = 0; loop_on_elments < PWM_PINS_ITEMS_COUNT; ++loop_on_elments)
        {
            /* Save LEDs */
			pwm_gs_pwm_pins[loop_on_array][loop_on_elments] = pwm_gs_pwm_pins[loop_on_array][loop_on_elments];
		}
	}
}

void pwm_start()
{
    sei();
    TIMER0_OV_InterruptEnable();
    TIMER0_OC_InterruptEnable();
    Timer0_change(TIMER0_SCALER_8);
}

void pwm_stop(){
    Timer0_change(TIMER0_STOP);
}


void pwm_set_duty(uint8_t_ percentage)
{
    OCR0= (PWM_MAX_REG_COUNT * (10 * percentage / 100)) / 10;
}

ISR(TIMER0_OVF_vect)
{
    uint8_t_ u8_l_counter;

    for (u8_l_counter= 0; u8_l_counter < PWM_PINS_COUNT; ++u8_l_counter)
    {
	    Led_TurnOn(pwm_gs_pwm_pins[u8_l_counter][PWM_PORT_INDEX], pwm_gs_pwm_pins[u8_l_counter][PWM_PIN_INDEX]);
	}

}

ISR(TIMER0_OC_vect)
{
    uint8_t_ u8_l_counter;

    for (u8_l_counter= 0; u8_l_counter < PWM_PINS_COUNT; ++u8_l_counter)
    {
	    Led_TurnOff(pwm_gs_pwm_pins[u8_l_counter][PWM_PORT_INDEX], pwm_gs_pwm_pins[u8_l_counter][PWM_PIN_INDEX]);
	}

}