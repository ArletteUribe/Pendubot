/*
 * ftm.h
 *
 *  Created on: 3 may 2024
 *      Author: arlet
 */

#ifndef FTM_H_
#define FTM_H_

/*******************************************************************************
 * Included files
 ******************************************************************************/

#include "fsl_ftm.h"
#include "MK66F18.h"

/*******************************************************************************
 * Definitions & structures
 ******************************************************************************/

#define PWM_FREQUENCY (10000U)
#define FTM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)


typedef struct{
	FTM_Type ftmSelected;
	ftm_chnl_t ftmChannel;
	ftm_pwm_level_select_t pwmLevel;
	uint32_t pwmFrequency;
	uint8_t pwmDutyCycle;
}PWM_config_t;


/*******************************************************************************
 * Functions prototypes
 ******************************************************************************/

void FTM_config(uint8_t Dutycycle,uint16_t pwm_frequency);

void FTM_setDutyCycle(uint8_t Dutycycle);

#endif /* FTM_H_ */
