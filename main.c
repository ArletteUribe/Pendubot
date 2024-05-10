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

#define SAMPLE_TIME pdMS_TO_TICKS(10)


/******************************************************************************
 * Static function prototypes:
******************************************************************************/

uint8_t count = 0;
float frecuencia1250;
float frecuencia600;
float posicion_motor = -PI;


/******************************************************************************
 * Static function prototypes:
******************************************************************************/

void encoder_sample_task(void *pvParameters);


/******************************************************************************
 * Main function:
******************************************************************************/

int main(void)
{
	xTaskCreate(encoder_sample_task, "encoder", 100, NULL, 10, NULL);

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
	while (1)
	{
		frecuencia600  = encoder_get_freq600();
		posicion_motor += (frecuencia600/600.0f)*0.01f;
		printf("Posición redondeada: %d (motor)\n", (int32_t)posicion_motor);
		vTaskDelay(SAMPLE_TIME);
	}
}

