/*
 * ftm.c
 *
 *  Created on: 3 may 2024
 *      Author: José González &
 *      		Arlette Uribe
 */


#include "ftm.h"

void FTM3_IRQHandler(void){

    /* Clear interrupt flag.*/
    FTM_ClearStatusFlags(FTM3,FTM_GetStatusFlags(FTM3));

    __DSB();
}


void FTM_config(uint8_t Dutycycle,uint16_t pwm_frequency){

    ftm_config_t ftmInfo;
    ftm_chnl_pwm_signal_param_t ftmParam;
    ftm_pwm_level_select_t pwmLevel = kFTM_HighTrue;

    FTM_GetDefaultConfig(&ftmInfo);
    ftmInfo.prescale = FTM_CalculateCounterClkDiv(FTM3, pwm_frequency, FTM_SOURCE_CLOCK);
    FTM_Init(FTM3, &ftmInfo);

    /* Configure ftm params */
    ftmParam.chnlNumber            = kFTM_Chnl_1;
    ftmParam.level                 = pwmLevel;
    ftmParam.dutyCyclePercent      = Dutycycle;
    ftmParam.firstEdgeDelayPercent = 0U;
    ftmParam.enableComplementary   = false;
    ftmParam.enableDeadtime        = false;

    FTM_SetupPwm(FTM3, &ftmParam, 1U, kFTM_CenterAlignedPwm, pwm_frequency, FTM_SOURCE_CLOCK);
    /* Enable channel interrupt flag.*/
    FTM_EnableInterrupts(FTM3, kFTM_Chnl1InterruptEnable);
    /* Enable at the NVIC */
    EnableIRQ(FTM3_IRQn);

    FTM_StartTimer(FTM3, kFTM_SystemClock);
}

void FTM_setDutyCycle(uint8_t Dutycycle){

	/* Disable channel output before updating the dutycycle */
	FTM_UpdateChnlEdgeLevelSelect(FTM3, kFTM_Chnl_1, 0U);

	/* Update PWM duty cycle */
	FTM_UpdatePwmDutycycle(FTM3, kFTM_Chnl_1, kFTM_CenterAlignedPwm, Dutycycle);

	/* Software trigger to update registers */
	FTM_SetSoftwareTrigger(FTM3, true);

	/* Start channel output with updated dutycycle */
	FTM_UpdateChnlEdgeLevelSelect(FTM3, kFTM_Chnl_1, kFTM_HighTrue);
}


