/*
 * gpio.c
 *
 *  Created on: 10 may 2024
 *      Author: arlet
 */


#include "gpio.h"

static void (*gpio_D_callback)(void) = 0;


void GPIO_callback_init(GPIO_Type * gpio, void (*handler))
{
	if(GPIOD == gpio)
	{
		gpio_D_callback = handler;
	}
}

void PORTD_IRQHandler(void)
{
	if(gpio_D_callback)
	{
		gpio_D_callback();
	}
	GPIO_PortClearInterruptFlags(GPIOD, 0xFFFFFFFF);

}

void GPIO_init(void)
{

	gpio_pin_config_t gpio_input_config = {
			 kGPIO_DigitalInput,
			 0,
	};

	gpio_pin_config_t gpio_output_config = {
			 kGPIO_DigitalOutput,
			 0,
	};

	CLOCK_EnableClock(kCLOCK_PortD);
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

	GPIO_PinInit(GPIOD, 0u, &gpio_input_config);
	PORT_SetPinConfig(PORTD, 0u, &button_config);
	PORT_SetPinInterruptConfig(PORTD, 0u, kPORT_InterruptFallingEdge);

	PORT_SetPinMux(PORTB, 7U,kPORT_MuxAsGpio);
	GPIO_PinInit(GPIOB, 7U, &gpio_output_config);


	NVIC_EnableIRQ(62U);
	NVIC_SetPriority(62U, 4U);
	__enable_irq();
}
