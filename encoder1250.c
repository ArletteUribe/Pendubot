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
int32_t g_pit_period = 0;
int32_t g_position = 0;
int8_t g_sign = 0;


/******************************************************************************
 * Function code:
******************************************************************************/

void capture_position1250(void)
{
	if (GPIO_PinRead(CHNL_B_GPIO, CHNL_B_PIN))
	{
		g_position--;
	}
	else
	{
		g_position++;
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
	return (PI/625.0f)*((float)g_position);
}

void ENCODER_IRQ_NAME(void)
{
	GPIO_PortClearInterruptFlags(ENCODER_GPIO, 0xFFFF);
	capture_position1250();
}
