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

uint8_t count = 0;
float frecuencia1250;
float frecuencia600;
float corriente;


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
		frecuencia1250 = encoder_get_freq1250();
		frecuencia600  = encoder_get_freq600();
		corriente      = current_get_value();
		printf("Frecuencia redondeada: %d (/1250)\n", (uint32_t)frecuencia1250);
		printf("Frecuencia redondeada: %d (/600)\n", (uint32_t)frecuencia600);
		printf("Corriente (0-4095): %d", corriente);
		vTaskDelay(SAMPLE_TIME);
	}
}

void bg_task(void *pvParameters)
{
	while(1)
	{
	}
}
