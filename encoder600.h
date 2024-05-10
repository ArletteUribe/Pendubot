/*
 * @Authors: José Ángel González Vázquez
 * 			 Arlette Gabriela Uribe Ventura
 *
 * @Brief: This driver serves the purpose of measuring the frequency of a
 * 		   signal entered through a GPIO pin. Created with the purpose of
 * 		   using a motor encoder.
 */

#ifndef ENCODER600_H_
#define ENCODER600_H_

#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_pit.h"
#include "fsl_clock.h"
#include "MK66F18.h"
#include "arm_math.h"

/******************************************************************************
 * Definitions:
******************************************************************************/

#define ENCODER2_PORT     PORTC
#define ENCODER2_GPIO     GPIOC
#define ENCODER2_PIN      2U
#define ENCODER2_CLOCK    kCLOCK_PortC
#define ENCODER2_IRQ_NUM  61U 	// PORTC
#define ENCODER2_IRQ_PRIO 1U
#define ENCODER2_IRQ_NAME PORTC_IRQHandler

#define CHNL_B_PORT2     PORTA
#define CHNL_B_GPIO2     GPIOA
#define CHNL_B_PIN2      25U
#define CHNL_B_CLOCK2    kCLOCK_PortA


/******************************************************************************
 * Function prototypes:
******************************************************************************/

void capture_position600(void);

void encoder_init_meas600(void);

float encoder_get_freq600(void);

float encoder_get_pos600(void);

#endif /* ENCODER600_H_ */
