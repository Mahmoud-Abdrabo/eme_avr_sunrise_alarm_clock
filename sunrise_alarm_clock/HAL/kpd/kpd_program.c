/*
 * kpd_program.c
 *
 * Created: 8/18/2023 2:53:46 PM
 *  Author: Hossam Elwahsh
 */ 

#include "kpd_config.h"
#include "kpd_interface.h"
#include "timer_interface.h"
#include "Timers_Services.h"

#define  F_CPU    16000000UL

#define KPD_INIT_DELAY_TIMER()  timer2_init(TIMER2_NORMAL_MODE)
#define KPD_TIMER_MS_DELAY(ms_delay_val) delay_ms(ms_delay_val)

const uint8_t_ keypad_values[4][4] = {
        {'1','2','3','A'},
        {'4','5','6','B'},
        {'7','8','9','C'},
        {'+','0','-','D'},
};

const uint8_t_ keypad_cols[KEYPAD_COLS_TOTAL] = {
        PIN7_ID,
        PIN6_ID,
        PIN5_ID,
        PIN3_ID
};

const uint8_t_ keypad_rows[KEYPAD_ROWS_TOTAL] = {
        PIN5_ID,
        PIN4_ID,
        PIN3_ID,
        PIN2_ID
};


void keypad_init(void)
{
    /* Declare local variables */
    uint8_t_ uint8_l_loop_counter;

    /* Init keypad */
    KPD_INIT_DELAY_TIMER();

    /* init rows */
    for (uint8_l_loop_counter = 0; uint8_l_loop_counter < KEYPAD_ROWS_TOTAL; ++uint8_l_loop_counter) {
        /* init pin as input */
        GPIO_setupPinDirection(KEYPAD_ROW_PORT , keypad_rows[uint8_l_loop_counter] , PIN_INPUT);
        /* enable pin pull up */
        GPIO_writePin(KEYPAD_ROW_PORT , keypad_rows[uint8_l_loop_counter] , LOGIC_HIGH);
    }

    /* init columns */
    for (uint8_l_loop_counter = 0; uint8_l_loop_counter < KEYPAD_ROWS_TOTAL; ++uint8_l_loop_counter)
    {
        /* init pin as input */
        GPIO_setupPinDirection(KEYPAD_COLUMN_PORT , keypad_rows[uint8_l_loop_counter] , PIN_INPUT);
    }

}


uint8_t_ keypad_read(void)
{
    /* Declare local variables */
    uint8_t_ uint8_l_rows_loop_index;
    uint8_t_ uint8_l_cols_loop_index;
    uint8_t_ uint8_val_retval;

    uint8_val_retval = NULL;

    for (uint8_l_rows_loop_index = 0; uint8_l_rows_loop_index < KEYPAD_ROWS_TOTAL; ++uint8_l_rows_loop_index)
    {

        /* set row as output -> low */
        GPIO_setupPinDirection(KEYPAD_ROW_PORT, keypad_rows[uint8_l_rows_loop_index], PIN_OUTPUT);
        GPIO_writePin(KEYPAD_ROW_PORT, keypad_rows[uint8_l_rows_loop_index], LOGIC_LOW);

        for (uint8_l_cols_loop_index = 0; uint8_l_cols_loop_index < KEYPAD_COLS_TOTAL; ++uint8_l_cols_loop_index)
        {
            uint8_t_ u8_l_dio_pin_val = LOGIC_HIGH;

            u8_l_dio_pin_val = GPIO_readPin(KEYPAD_COLUMN_PORT, keypad_cols[uint8_l_cols_loop_index]);

            if (u8_l_dio_pin_val == LOGIC_LOW)
            {
                /* debounce */
                KPD_TIMER_MS_DELAY(KEYPAD_DEBOUNCE_DELAY_MS);

                u8_l_dio_pin_val = GPIO_readPin(KEYPAD_COLUMN_PORT, keypad_cols[uint8_l_cols_loop_index]);

                /* block till user release key */
                while (u8_l_dio_pin_val == LOGIC_LOW)
                {
                    u8_l_dio_pin_val = GPIO_readPin(KEYPAD_COLUMN_PORT, keypad_cols[uint8_l_cols_loop_index]);
                }

                /* revert row to input */
                GPIO_setupPinDirection(KEYPAD_ROW_PORT, keypad_rows[uint8_l_rows_loop_index], PIN_INPUT);


                return keypad_values[uint8_l_rows_loop_index][uint8_l_cols_loop_index];

                /*if (u8_l_dio_pin_val == LOGIC_LOW)
                {

                }*/
            }
        }

        /* revert row to input */
        GPIO_setupPinDirection(KEYPAD_ROW_PORT, keypad_rows[uint8_l_rows_loop_index], PIN_INPUT);
    }

    return uint8_val_retval;
}
