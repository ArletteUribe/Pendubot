/*
 * @Authors: José Ángel González Vázquez
 * 			 Arlette Gabriela Uribe Ventura
 *
 * @Brief: This driver serves the purpose of measuring the frequency of a
 * 		   signal entered through a GPIO pin. Created with the purpose of
 * 		   using a motor encoder.
 */

#include "encoder1250.h"

/******************************************************************************
 * Global variables:
******************************************************************************/

uint8_t g_flag = 0;
uint32_t g_pit_period = 0;
uint32_t g_position = 0;
uint8_t g_sign = 0;


/******************************************************************************
 * Function code:
******************************************************************************/

void capture_values1250(uint32_t flags)
{
	if(g_flag == 0)
	{
		PIT_StartTimer(PIT, MEAS_PIT_CHNL);
		g_flag = 1;
	}
	else
	{
		g_pit_period = USEC_TO_COUNT(5000000U,21000000U) - PIT_GetCurrentTimerCount(PIT, MEAS_PIT_CHNL);
		if (GPIO_PinRead(CHNL_B_GPIO, CHNL_B_PIN))
		{
			g_sign = -1;
			g_position--;
		}
		else
		{
			g_sign = 1;
			g_position++;
		}
		PIT_StopTimer(PIT, MEAS_PIT_CHNL);
		PIT_StartTimer(PIT, MEAS_PIT_CHNL);
	}
	if (1250U == g_position)
	{
		g_position = 0;
	}
}

void encoder_init_meas1250(void)
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
	PIT_SetTimerPeriod(PIT, MEAS_PIT_CHNL, USEC_TO_COUNT(5000000U,21000000U));

	// Config encoder channel A pin as input with falling edge interrupt:
	CLOCK_EnableClock(ENCODER_CLOCK);
	PORT_SetPinConfig(ENCODER_PORT, ENCODER_PIN, &input_config);
	GPIO_PinInit(ENCODER_GPIO, ENCODER_PIN, &gpio_input_config);
	PORT_SetPinInterruptConfig(ENCODER_PORT, ENCODER_PIN, kPORT_InterruptFallingEdge);
	// Enable the pin's IRQ:
	NVIC_EnableIRQ(ENCODER_IRQ_NUM);
	NVIC_SetPriority(ENCODER_IRQ_NUM, ENCODER_IRQ_PRIO);

	// Config encoder channel B pin as input with no interrupt:
	CLOCK_EnableClock(CHNL_B_CLOCK);
	PORT_SetPinConfig(CHNL_B_PORT, CHNL_B_PIN, &input_config);
	GPIO_PinInit(CHNL_B_GPIO, CHNL_B_PIN, &gpio_input_config);
	PORT_SetPinInterruptConfig(CHNL_B_PORT, CHNL_B_PIN, kPORT_InterruptOrDMADisabled);

}

float encoder_get_freq1250(void)
{
	float frequency = 0.0f;
	if (g_pit_period)
	{
		frequency = ((float)CLOCK_GetFreq(kCLOCK_BusClk)) / g_pit_period;
	}

	return frequency*((float)g_sign);
}

float encoder_get_pos1250(void)
{
	return ((float)g_position)/1250.0f;
}

void ENCODER_IRQ_NAME(void)
{
	GPIO_PortClearInterruptFlags(ENCODER_GPIO, 0xFFFF);
	capture_values1250(0);
}
