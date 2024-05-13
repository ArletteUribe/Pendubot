/*
 * @Authors: José Ángel González Vázquez
 * 			 Arlette Gabriela Uribe Ventura
 */

#include "encoder1250.h"
#include "encoder600.h"
#include <stdint.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "gpio.h"
#include "controlador.h"
#include "ftm.h"
#include "arm_math.h"

/******************************************************************************
 * Definitions:
******************************************************************************/

#define SAMPLE_TIME pdMS_TO_TICKS(10)


/******************************************************************************
 * Global variables:
******************************************************************************/

xTaskHandle control_task;
uint8_t apply_controller = 0;

/******************************************************************************
 * Static function prototypes:
******************************************************************************/

void init_task(void *pvParameters);

void encoder_sample_task(void *pvParameters);

void start_control(void);

/*
void arm_float_to_q15(
const float32_t * pSrc,
      q15_t * pDst,
      uint32_t blockSize);
*/


/******************************************************************************
 * Main function:
******************************************************************************/

int main(void)
{

	xTaskCreate(init_task, "init", 100, NULL, 5, NULL);
	xTaskCreate(encoder_sample_task, "encoder", 100, NULL, 2, &control_task);

	//vTaskSuspend(control_task);

	vTaskStartScheduler();

    while(1)
    {
    }
    return 0;
}

/******************************************************************************
 * Static function code:
******************************************************************************/

void init_task(void *pvParameters)
{
	encoder_init_meas1250();
	encoder_init_meas600();

	FTM_config(0U, 400U);

	GPIO_init();
	GPIO_callback_init(GPIOD, start_control);

	vTaskSuspend(NULL);
}

void encoder_sample_task(void *pvParameters)
{
	float frecuencia_motor;
	float frecuencia_union;
	float posicion_motor[2] = {-PI, 0};
	float posicion_union[2] = {0, 0};
	float ldc;
	float dc;

	q15_t q15_posicion_motor;
	q15_t q15_1_5 = 0x0200;
	q15_t q15_menos_1_5 = 0xFF80;

	while (1)
	{

		posicion_motor[1] = posicion_motor[0];
		posicion_motor[0] = encoder_get_pos600() - PI;
		frecuencia_motor = (posicion_motor[0] - posicion_motor[1])/0.01f;

		posicion_union[1] = posicion_union[0];
		posicion_union[0] = encoder_get_pos1250();
		frecuencia_union = (posicion_union[0] - posicion_union[1])/0.01f;

		if (apply_controller)
		{
			// En caso de que los brazos se caigan, abortar:

			arm_float_to_q15(&posicion_motor[0],&q15_posicion_motor,1);

			if ((q15_posicion_motor < q15_menos_1_5) || (q15_posicion_motor > q15_1_5))
			{
				GPIO_PinWrite(GPIOB, 7U, 0U);
				FTM_setDutyCycle(0U);
				while (1)
				{
				}
			}
			ldc = LQR(posicion_motor[0], posicion_union[0], frecuencia_motor, frecuencia_union);
			if (ldc > 3.3f)
			{
				ldc = 3.3f;
			}
			else if (ldc < -3.3f)
			{
				ldc = -3.3f;
			}
			if (ldc < 0)
			{
				GPIO_PinWrite(GPIOB, 7U, 1U);
				dc = (100.0f/3.3f)*(3.3f+ldc);
				FTM_setDutyCycle((uint8_t)dc);
			}
			else
			{
				GPIO_PinWrite(GPIOB, 7U, 0U);
				dc = (ldc*100.0f)/3.3f;
				FTM_setDutyCycle((uint8_t)dc);
			}

		}

		vTaskDelay(SAMPLE_TIME);
	}
}

void start_control(void)
{
	apply_controller = 1;
}
