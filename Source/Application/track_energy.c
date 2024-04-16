#include "track_energy.h"
#include "stm32f4xx.h"
#include <string.h>

uint8_t track_on[3];
static uint8_t track_energy_pin[3];
bool update_track_energy_io;

void init_track_energy_gpio(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
    memset(track_on,0,3);
    track_energy_pin[0] = GPIO_Pin_2;
    track_energy_pin[1] = GPIO_Pin_3;
    track_energy_pin[2] = GPIO_Pin_4;

    GPIO_InitTypeDef GPIO_InitStructure = {0};
    GPIO_ResetBits(GPIOF, GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
    return;
}

void track_energy_switch(void)
{
    for(int i = 0; i < 3; i++)
    {
        if(track_on[i])
            { GPIO_SetBits(GPIOF, track_energy_pin[i]);}
        else
            { GPIO_ResetBits(GPIOF, track_energy_pin[i]);}
    }
    return;
}

void init_track_analog_watch_gpio(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    //__HAL_RCC_ADC3_CLK_ENABLE();
    //__HAL_RCC_GPIOF_CLK_ENABLE();
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
    /**ADC3 GPIO Configuration
    PF8     ------> ADC3_IN6
    PF9     ------> ADC3_IN7
    PF10     ------> ADC3_IN8
    */
    GPIO_InitStruct.Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;
    GPIO_InitStruct.Mode = GPIO_Mode_AN;
    GPIO_InitStruct.Pull = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOF, &GPIO_InitStruct);

  ADC_AnalogWDGConfTypeDef AnalogWDGConfig = {0};
  ADC_ChannelConfTypeDef sConfig = {0};
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc3.Instance = ADC3;
  hadc3.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc3.Init.Resolution = ADC_RESOLUTION_12B;
  hadc3.Init.ScanConvMode = DISABLE;
  hadc3.Init.ContinuousConvMode = DISABLE;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc3.Init.NbrOfConversion = 1;
  hadc3.Init.DMAContinuousRequests = DISABLE;
  hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc3) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the analog watchdog
  */
  AnalogWDGConfig.WatchdogMode = ADC_ANALOGWATCHDOG_SINGLE_REG;
  AnalogWDGConfig.HighThreshold = 0;
  AnalogWDGConfig.LowThreshold = 0;
  AnalogWDGConfig.Channel = ADC_CHANNEL_8;
  AnalogWDGConfig.ITMode = DISABLE;
  if (HAL_ADC_AnalogWDGConfig(&hadc3, &AnalogWDGConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

void analog_watchdogs(void)
{

}