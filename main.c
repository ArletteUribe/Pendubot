/*
 * @Authors: José Ángel González Vázquez
 * 			 Arlette Gabriela Uribe Ventura
 */

#include "encoder1250.h"
#include "encoder600.h"
#include "current.h"
#include <stdint.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

/******************************************************************************
 * Definitions:
******************************************************************************/

#define SAMPLE_TIME pdMS_TO_TICKS(100)


/******************************************************************************
 * Static function prototypes:
******************************************************************************/


/******************************************************************************
 * Static function prototypes:
******************************************************************************/

void encoder_sample_task(void *pvParameters);


/******************************************************************************
 * Main function:
******************************************************************************/

int main(void)
{
	xTaskCreate(encoder_sample_task, "encoder", 100, NULL, 2, NULL);

	encoder_init_meas1250();
	encoder_init_meas600();
	current_init();

	vTaskStartScheduler();

    while(1)
    {
    }
    return 0 ;
}

/******************************************************************************
 * Static function code:
******************************************************************************/

void encoder_sample_task(void *pvParameters)
{
	float frecuencia_motor;
	float frecuencia_union;
	float posicion_motor[2] = {-PI, 0};
	float posicion_union[2] = {0, 0};

	while (1)
	{
		posicion_motor[1] = posicion_motor[0];
		posicion_motor[0] = encoder_get_pos600() - PI;
		frecuencia_motor = (posicion_motor[0] - posicion_motor[1])/0.1f;

		posicion_union[1] = posicion_union[0];
		posicion_union[0] = encoder_get_pos1250();
		frecuencia_union = (posicion_union[0] - posicion_union[1])/0.1f;

		vTaskDelay(SAMPLE_TIME);
	}
}

