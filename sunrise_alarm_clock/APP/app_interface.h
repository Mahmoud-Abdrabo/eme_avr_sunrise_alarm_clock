/*
 * app_interface.h
 *
 * Created: 8/26/2023 3:29:16 PM
 *  Author: Team GUCALEX
 */ 


#ifndef APP_INTERFACE_H_
#define APP_INTERFACE_H_

#include "app_config.h"
#include "std.h"

#define CONVERT_CHAR_TO_DIGIT(char_val) (char_val - '0')
#define CONVERT_DIGIT_TO_CHAR(digit_val) (digit_val + '0')

void app_init();
void app_start();

#endif /* APP_INTERFACE_H_ */