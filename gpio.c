/*
 * gpio.c
 *
 *  Created on: 10 may 2024
 *      Author: arlet
 */


#include "gpio.h"

static void (*gpio_B_callback)(void) = 0;


void GPIO_callback_init(GPIO_Type * gpio, void (*handler))
{
	if(GPIOB == gpio)
	{
		gpio_B_callback = handler;
	}
}

void PORTB_IRQHandler(void)
{
	if(gpio_B_callback){
		gpio_B_callback();
	}
	GPIO_PortClearInterruptFlags(GPIOB, 0xFFFFFFFF);

}

void GPIO_init(void)
{

	gpio_pin_config_t gpio_input_config = {
			 kGPIO_DigitalInput,
			 0,
	};

	CLOCK_EnableClock(kCLOCK_PortB);

	const port_pin_config_t button_config = {
			kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
			kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
			kPORT_PassiveFilterEnable,                               /* Passive filter is disabled */
			kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
			kPORT_HighDriveStrength,                                 /* High drive strength is configured */
			kPORT_MuxAsGpio,                                         /* Pin is configured as PTA4 */
			kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
		  };

	GPIO_PinInit(GPIOB, 6u, &gpio_input_config);
	PORT_SetPinConfig(PORTB, 6u, &button_config);
	PORT_SetPinInterruptConfig(PORTB, 6u, kPORT_InterruptFallingEdge);
}
