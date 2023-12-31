cmake_minimum_required(VERSION 3.25)
project(eme_avr_sunrise_alarm_clock C)

set(CMAKE_C_STANDARD 11)

include_directories(sunrise_alarm_clock/APP)
include_directories(sunrise_alarm_clock/COMMON)
include_directories(sunrise_alarm_clock/HAL/lcd)
include_directories(sunrise_alarm_clock/HAL/ldr)
include_directories(sunrise_alarm_clock/HAL/led)
include_directories(sunrise_alarm_clock/HAL/kpd)
include_directories(sunrise_alarm_clock/HAL/buzz)
include_directories(sunrise_alarm_clock/HAL/pwm)
include_directories(sunrise_alarm_clock/MCAL/adc)
include_directories(sunrise_alarm_clock/COMMON/timer)
include_directories(sunrise_alarm_clock/MCAL/gpio)
include_directories(sunrise_alarm_clock/SERVICE/)

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
        sunrise_alarm_clock/HAL/ldr/ldr_program.c
        sunrise_alarm_clock/HAL/ldr/ldr_interface.h
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

        sunrise_alarm_clock/HAL/kpd/kpd_program.c
        sunrise_alarm_clock/HAL/kpd/kpd_interface.h
        sunrise_alarm_clock/HAL/kpd/kpd_config.h
        sunrise_alarm_clock/COMMON/timer/Timers.c
        sunrise_alarm_clock/HAL/pwm/pwm_program.c
        sunrise_alarm_clock/HAL/pwm/pwm_interface.h
        sunrise_alarm_clock/HAL/buzz/buzz_interface.h
        sunrise_alarm_clock/HAL/pwm/pwm_linking_config.c
        sunrise_alarm_clock/HAL/pwm/pwm_linking_config.h
        sunrise_alarm_clock/APP/app_private.h
        sunrise_alarm_clock/SERVICE/Timers_Services.h
        sunrise_alarm_clock/SERVICE/Timers_Services.c
        sunrise_alarm_clock/HAL/buzz/buzz_program.c

)
