#include "track_energy.h"
#include <string.h>
#include "main.h"

uint8_t track_on[3];
static uint8_t track_energy_pin[3];
bool update_track_energy_io;

//calcultate bin from current,  x - Ampers
#define CALC_CURRENT_(x)  (uint16_t)((0.1F * x) * (4096.F/3.3))

static void reset_all_pin(void)
{
   memset(track_on,0,3);
   GPIO_ResetBits(GPIOF, GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4);
}

void init_track_energy_gpio(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
    track_energy_pin[0] = GPIO_Pin_2;
    track_energy_pin[1] = GPIO_Pin_3;
    track_energy_pin[2] = GPIO_Pin_4;

    GPIO_InitTypeDef GPIO_InitStructure = {0};
    reset_all_pin();

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
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
    //ADC3 GPIO Configuration
    //PF8     ------> ADC3_IN6
    //PF9     ------> ADC3_IN7
    //PF10     ------> ADC3_IN8
    
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOF, &GPIO_InitStruct);

    /* Initialization and Configuration functions *********************************/
    ADC_TypeDef* ADCx = ADC3;
    ADC_InitTypeDef ADC_InitStruct = {
      .ADC_ContinuousConvMode = ENABLE,
      .ADC_DataAlign = ADC_DataAlign_Right,
      .ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1,
      .ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None,
      .ADC_NbrOfConversion = 3,
      .ADC_Resolution = ADC_Resolution_12b,
      .ADC_ScanConvMode = ENABLE,  
      };
    ADC_CommonInitTypeDef ADC_CommonInitStruct = {
      .ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled,
      .ADC_Mode = ADC_Mode_Independent ,
      .ADC_Prescaler = ADC_Prescaler_Div8,
      .ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles
    };

    ADC_Init(ADCx, &ADC_InitStruct);
    ADC_CommonInit(&ADC_CommonInitStruct);

    /* Analog Watchdog configuration functions ************************************/
    ADC_AnalogWatchdogCmd(ADCx, ADC_AnalogWatchdog_AllRegEnable);
    ADC_AnalogWatchdogThresholdsConfig(ADCx, CALC_CURRENT_(3), 0);
    
    /* Regular Channels Configuration functions ***********************************/
    ADC_RegularChannelConfig(ADCx, ADC_Channel_6, 1, ADC_SampleTime_480Cycles);
    ADC_RegularChannelConfig(ADCx, ADC_Channel_7, 2, ADC_SampleTime_480Cycles);
    ADC_RegularChannelConfig(ADCx, ADC_Channel_8, 3, ADC_SampleTime_480Cycles);

    ADC_EOCOnEachRegularChannelCmd(ADCx, DISABLE);

    ADC_ITConfig(ADCx, ADC_IT_AWD, ENABLE);
    /* Enable the ADCx Interrupt */
    NVIC_InitTypeDef NVIC_InitStructure;
	  NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);

    ADC_Cmd(ADCx, ENABLE);
    Delay_ms(2);// adc stabilize
    ADC_SoftwareStartConv(ADCx);
};

void analog_watchdogs(void)
{
  ADC_TypeDef* ADCx = ADC3;

  if(ADC_GetITStatus(ADCx, ADC_IT_AWD) != RESET)
  {
    if(ADC_GetFlagStatus(ADCx, ADC_FLAG_AWD))
    {
      /* Clear the ADC analog watchdog flag */      
      reset_all_pin();// TO DO  switch off all track
      ADC_ClearFlag(ADCx, ADC_FLAG_AWD);
    }
  }
};



