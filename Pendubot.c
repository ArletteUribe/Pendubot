

/**
 * @file    	Pendubot.c
 *
 * @authors		José González &
 * 				Arlette Uribe
 *
 * @brief   	Application entry point.
 */

#include <stdio.h>
#include "MK66F18.h"
#include "ftm.h"
#include "pin_mux.h"
#include "fsl_clock.h"
#include "fsl_port.h"

uint8_t Dcycle = 0;

int main(void) {

	CLOCK_SetSimSafeDivs();

    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);

    /* PORTE6 (pin E1) is configured as FTM3_CH1 */
    PORT_SetPinMux(PORTD, 1U, kPORT_MuxAlt4);

	FTM_config(50,24000);


    while(1) {

    	SDK_DelayAtLeastUs(100000,CLOCK_GetFreq(kCLOCK_BusClk));

    	if(Dcycle < 100){
    		FTM_setDutyCycle(Dcycle);
        	Dcycle++;
    	}
    	else{
    		Dcycle = 0;
    	}

    	SDK_DelayAtLeastUs(100000,CLOCK_GetFreq(kCLOCK_BusClk));
    }
    return 0 ;
}
