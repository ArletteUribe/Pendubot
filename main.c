/*
 * @Authors: José Ángel González Vázquez
 * 			 Arlette Gabriela Uribe Ventura
 */

#include <stdint.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "encoder.h"

/******************************************************************************
 * Definitions:
******************************************************************************/

#define SAMPLE_TIME pdMS_TO_TICKS(100)


/******************************************************************************
 * Static function prototypes:
******************************************************************************/

uint8_t count = 0;
float frecuencia;


/******************************************************************************
 * Static function prototypes:
******************************************************************************/

void encoder_sample_task(void *pvParameters);
void bg_task(void *pvParameters);


/******************************************************************************
 * Main function:
******************************************************************************/

int main(void)
{
	xTaskCreate(encoder_sample_task, "encoder", 100, NULL, 10, NULL);
	xTaskCreate(bg_task, "background", 100, NULL, 1, NULL);

	encoder_init_meas();

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
		frecuencia = encoder_get_freq();
		printf("Frecuencia redondeada: %d\n", (uint32_t)frecuencia);
		vTaskDelay(SAMPLE_TIME);
	}
}

void bg_task(void *pvParameters)
{
	while(1)
	{
	}
}
