/*
 * gpio.h
 *
 *  Created on: 10 may 2024
 *      Author: arlet
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "fsl_gpio.h"
#include "fsl_port.h"

void GPIO_callback_init(GPIO_Type * gpio, void (*handler));

void GPIO_init(void);

#endif /* GPIO_H_ */
