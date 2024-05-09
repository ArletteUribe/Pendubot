/*
 * @Authors: José Ángel González Vázquez
 * 			 Arlette Gabriela Uribe Ventura
 *
 * @Brief: This driver uses ADC to measure current.
 */

#include "current.h"

/******************************************************************************
 * Global variables:
******************************************************************************/

adc16_channel_config_t adc16ChannelConfig;


/******************************************************************************
 * Function code:
******************************************************************************/

void current_init(void)
{
    adc16_config_t adc16Config;

    // Config ADC peripheral
    ADC16_GetDefaultConfig(&adc16Config);
    ADC16_Init(ADC_BASE, &adc16Config);
    ADC16_EnableHardwareTrigger(ADC_BASE, false);
    ADC16_DoAutoCalibration(ADC_BASE);

    // Set the config structure that will be used for conversion:
    adc16ChannelConfig.channelNumber                        = ADC_CHNL;
    adc16ChannelConfig.enableInterruptOnConversionCompleted = false;
    adc16ChannelConfig.enableDifferentialConversion         = false;
}

uint32_t current_get_value(void)
{
	// Start conversion:
	ADC16_SetChannelConfig(ADC_BASE, ADC_CHNL_GROUP, &adc16ChannelConfig);
	// Wait for conversion to finish:
    while (0U == (kADC16_ChannelConversionDoneFlag &
                  ADC16_GetChannelStatusFlags(ADC_BASE, ADC_CHNL_GROUP)))
    {
    }
    // Return value read by ADC:
    return ADC16_GetChannelConversionValue(ADC_BASE, ADC_CHNL_GROUP);

}

