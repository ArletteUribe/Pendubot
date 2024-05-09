/*
 * @Authors: José Ángel González Vázquez
 * 			 Arlette Gabriela Uribe Ventura
 *
 * @Brief: This driver uses ADC to measure current.
 */

#ifndef CURRENT_H_
#define CURRENT_H_

#include "fsl_adc16.h"

/******************************************************************************
 * Definitions:
******************************************************************************/

#define ADC_BASE          ADC1
#define ADC_CHNL_GROUP    0U
#define ADC_CHNL          23U


/******************************************************************************
 * Function prototypes:
******************************************************************************/

void current_init(void);

uint32_t current_get_value(void);

#endif /* CURRENT_H_ */
