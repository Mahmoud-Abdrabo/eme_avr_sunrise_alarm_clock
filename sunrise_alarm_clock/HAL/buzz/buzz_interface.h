/*
 * buzz_interface.h
 *
 * Created: 8/29/2023 1:32:44 AM
 *  Author: ahmed
 */ 


#ifndef BUZZ_INTERFACE_H_
#define BUZZ_INTERFACE_H_

#include "gpio.h"

#define  BUZZ_PORT         PORTC_ID 
#define  BUZZ_PIN          PIN6_ID 

void buzz_init(void);
void buzz_on(void);
void buzz_stop(void);


#endif /* BUZZ_INTERFACE_H_ */