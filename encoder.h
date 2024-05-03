/*
 * @Authors: José Ángel González Vázquez
 * 			 Arlette Gabriela Uribe Ventura
 *
 * @Brief: This driver serves the purpose of measuring the frequency of a
 * 		   signal entered through a GPIO pin. Created with the purpose of
 * 		   using a motor encoder.
 */

#ifndef ENCODER_H_
#define ENCODER_H_

#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_pit.h"
#include "fsl_clock.h"
#include "MK66F18.h"

/******************************************************************************
 * Function prototypes:
******************************************************************************/

#define ENCODER_PORT     PORTE
#define ENCODER_GPIO     GPIOE
#define ENCODER_PIN      9U
#define ENCODER_CLOCK    kCLOCK_PortE
#define ENCODER_IRQ_NUM  63 	// PORTE
#define ENCODER_IRQ_PRIO 1U
#define ENCODER_IRQ_NAME PORTE_IRQHandler

#define MEAS_PIT_CHNL  kPIT_Chnl_3


/******************************************************************************
 * Function prototypes:
******************************************************************************/

void capture_values(uint32_t flags);

void encoder_init_meas(void);

float encoder_get_freq(void);

#endif /* ENCODER_H_ */
