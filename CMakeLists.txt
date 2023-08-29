cmake_minimum_required(VERSION 3.25)
project(eme_avr_sunrise_alarm_clock C)

set(CMAKE_C_STANDARD 11)

include_directories(sunrise_alarm_clock/APP)
include_directories(sunrise_alarm_clock/COMMON)
include_directories(sunrise_alarm_clock/HAL/lcd)
include_directories(sunrise_alarm_clock/HAL/ldr)
include_directories(sunrise_alarm_clock/HAL/led)
include_directories(sunrise_alarm_clock/HAL/kpd)
include_directories(sunrise_alarm_clock/HAL/pwm)
include_directories(sunrise_alarm_clock/HAL/buzz)
include_directories(sunrise_alarm_clock/MCAL/adc)
include_directories(sunrise_alarm_clock/MCAL/timers)
include_directories(sunrise_alarm_clock/MCAL/gpio)
include_directories("C:/WinAVR-20100110/avr/include/")

add_executable(eme_avr_sunrise_alarm_clock
        sunrise_alarm_clock/APP/app_config.h
        sunrise_alarm_clock/APP/app_interface.h
        sunrise_alarm_clock/APP/app_program.c
        sunrise_alarm_clock/COMMON/bit_math.h
        sunrise_alarm_clock/COMMON/interrupts.h
        sunrise_alarm_clock/COMMON/mem_map.h
        sunrise_alarm_clock/COMMON/std.h
        sunrise_alarm_clock/HAL/lcd/lcd_config.h
        sunrise_alarm_clock/HAL/lcd/lcd_interface.h
        sunrise_alarm_clock/HAL/lcd/lcd_private.h
        sunrise_alarm_clock/HAL/lcd/lcd_program.c
        sunrise_alarm_clock/HAL/ldr/ldr.c
        sunrise_alarm_clock/HAL/ldr/ldr.h
        sunrise_alarm_clock/HAL/led/Led.c
        sunrise_alarm_clock/HAL/led/Led.h
        sunrise_alarm_clock/HAL/led/Led_Cfg.h
        sunrise_alarm_clock/MCAL/adc/adc_config.h
        sunrise_alarm_clock/MCAL/adc/adc_interface.h
        sunrise_alarm_clock/MCAL/adc/adc_lconfig.c
        sunrise_alarm_clock/MCAL/adc/adc_private.h
        sunrise_alarm_clock/MCAL/adc/adc_program.c
        sunrise_alarm_clock/MCAL/gpio/gpio.c
        sunrise_alarm_clock/MCAL/gpio/gpio.h
        sunrise_alarm_clock/MCAL/gpio/gpio_private.h
        sunrise_alarm_clock/main.c

        "C:/WinAVR-20100110/avr/include/util/delay.h"
        sunrise_alarm_clock/HAL/kpd/kpd_program.c
        sunrise_alarm_clock/HAL/kpd/kpd_interface.h
        sunrise_alarm_clock/HAL/kpd/kpd_config.h
        sunrise_alarm_clock/MCAL/timers/Timers.c
        sunrise_alarm_clock/HAL/pwm/pwm.c
        sunrise_alarm_clock/HAL/pwm/pwm.h

        sunrise_alarm_clock/HAL/buzz/buzz_interface.h)
