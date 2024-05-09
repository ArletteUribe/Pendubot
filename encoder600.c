/*
 * @Authors: José Ángel González Vázquez
 * 			 Arlette Gabriela Uribe Ventura
 *
 * @Brief: This driver serves the purpose of measuring the frequency of a
 * 		   signal entered through a GPIO pin. Created with the purpose of
 * 		   using a motor encoder.
 */

#include "encoder600.h"

/******************************************************************************
 * Global variables:
******************************************************************************/

uint8_t g_flag600 = 0;
uint32_t g_pit_period600 = 0;


/******************************************************************************
 * Function code:
******************************************************************************/

void capture_values600(uint32_t flags)
{
	if(g_flag600 == 0)
	{
		PIT_StartTimer(PIT, MEAS_PIT_CHNL2);
		g_flag600 = 1;
	}
	else
	{
		g_pit_period600 = USEC_TO_COUNT(5000000U,21000000U) - PIT_GetCurrentTimerCount(PIT, MEAS_PIT_CHNL2);
		PIT_StopTimer(PIT, MEAS_PIT_CHNL2);
		PIT_StartTimer(PIT, MEAS_PIT_CHNL2);
	}
}

void encoder_init_meas600(void)
{
	const port_pin_config_t input_config = {
			kPORT_PullUp,
			kPORT_SlowSlewRate,
			kPORT_PassiveFilterEnable,
			kPORT_OpenDrainDisable,
			kPORT_LowDriveStrength,
			kPORT_MuxAsGpio,
			kPORT_UnlockRegister
		  };

	gpio_pin_config_t gpio_input_config = {
			        kGPIO_DigitalInput,
			        0
			    };

	// PIT config:
	// PIT_Init function should only be executed once per program:
#ifndef PIT_INIT_DONE
#define PIT_INIT_DONE
	pit_config_t pit_config;
	PIT_GetDefaultConfig(&pit_config);
	PIT_Init(PIT, &pit_config);
#endif
	PIT_SetTimerPeriod(PIT, MEAS_PIT_CHNL2, USEC_TO_COUNT(5000000U,21000000U));

	// Config encoder channel pin as input with falling edge interrupt:
	CLOCK_EnableClock(ENCODER2_CLOCK);
	PORT_SetPinConfig(ENCODER2_PORT, ENCODER2_PIN, &input_config);
	GPIO_PinInit(ENCODER2_GPIO, ENCODER2_PIN, &gpio_input_config);
	PORT_SetPinInterruptConfig(ENCODER2_PORT, ENCODER2_PIN, kPORT_InterruptFallingEdge);
	// Enable the pin's IRQ:
	NVIC_EnableIRQ(ENCODER2_IRQ_NUM);
	NVIC_SetPriority(ENCODER2_IRQ_NUM, ENCODER2_IRQ_PRIO);

	// Config encoder channel B pin as input with no interrupt:
	CLOCK_EnableClock(CHNL_B_CLOCK2);
	PORT_SetPinConfig(CHNL_B_PORT2, CHNL_B_PIN2, &input_config);
	GPIO_PinInit(CHNL_B_GPIO2, CHNL_B_PIN2, &gpio_input_config);
	PORT_SetPinInterruptConfig(CHNL_B_PORT2, CHNL_B_PIN2, kPORT_InterruptOrDMADisabled);

}

float encoder_get_freq600(void)
{
	// TODO: revisar el canal B para determinar el sentido
	float frequency = 0.0f;
	if (g_pit_period600)
	{
		frequency = ((float)CLOCK_GetFreq(kCLOCK_BusClk)) / g_pit_period600;
	}

	return frequency;
}

void ENCODER2_IRQ_NAME(void)
{
	GPIO_PortClearInterruptFlags(ENCODER2_GPIO, 0xFFFF);
	capture_values600(0);
}
