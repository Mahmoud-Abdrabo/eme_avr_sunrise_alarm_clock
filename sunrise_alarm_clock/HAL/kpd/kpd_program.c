/*
 * kpd_program.c
 *
 * Created: 8/18/2023 2:53:46 PM
 *  Author: Hossam Elwahsh
 */ 

#include "kpd_config.h"
#include "kpd_interface.h"
#include "timer_interface.h"

const uint8_t_ keypad_values[4][4] = {
        {'1','2','3','4'},
        {'5','6','7','8'},
        {'9','A','B','C'},
        {'D','E','F','G'},
};

const enu_dio_pin_t_ keypad_cols[4] = {
    DIO_PIN_7,
     DIO_PIN_6,
     DIO_PIN_5,
     DIO_PIN_3
};

const enu_dio_pin_t_ keypad_rows[4] = {
        DIO_PIN_5,
        DIO_PIN_4,
        DIO_PIN_3,
        DIO_PIN_2
};


void KeyPad_Init(void)
{
    dio_port_init( KEYPAD_COLUMN_PORT , DIO_PORT_IN , KEYPAD_COL_MASK);
    dio_port_init( KEYPAD_ROW_PORT , DIO_PORT_IN , KEYPAD_ROW_MASK);

    dio_port_write(KEYPAD_ROW_PORT , DIO_PORT_HIGH , KEYPAD_ROW_MASK);

    TIMER_timer0NormalModeInit(DISABLED);
}


uint8_t_ KeyPad_GetValue(void) {
    uint8_t_ uint8_val_retval = 0;

    for (int row = 0; row < KEYPAD_ROWS_TOTAL; ++row)
    {

        /* set row as output -> low */
        dio_init(KEYPAD_ROW_PORT, keypad_rows[row], DIO_OUT);
        dio_write(KEYPAD_ROW_PORT, keypad_rows[row], DIO_PIN_LOW);


        for (int col = 0; col < KEYPAD_COLS_TOTAL; ++col)
        {
            enu_dio_pin_val_t_ enu_dio_pin_val = DIO_PIN_HIGH;

            dio_read(KEYPAD_COLUMN_PORT, keypad_cols[col], &enu_dio_pin_val);

            if (enu_dio_pin_val == DIO_PIN_LOW) {

                /* debounce */
                TIMER_delay_ms(100);

                dio_read(KEYPAD_COLUMN_PORT, keypad_cols[col], &enu_dio_pin_val);
                if (enu_dio_pin_val == DIO_PIN_LOW)
                {
                    /* revert row to input */
                    dio_init(KEYPAD_ROW_PORT, keypad_rows[row], DIO_IN);


                    /* return */
                    return keypad_values[row][col];
                }
            }
        }

        /* revert row to input */
        dio_init(KEYPAD_ROW_PORT, keypad_rows[row], DIO_IN);
    }

    return uint8_val_retval;
}