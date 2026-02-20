/*
 * adc.c
 *
 *  Created on: Feb 17, 2026
 *      Author: Sunil Sutar
 */

#include <stm32f446xx.h>
#include "adc.h"

void ADC1_Init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->MODER |= (3<<2);   // PA1 analog mode

    ADC1->SQR3 = 1;          // Channel 1
    ADC1->SQR1 = 0;          // 1 conversion
    ADC1->CR2 |= ADC_CR2_ADON;
}

uint16_t ADC1_Read(void)
{
    ADC1->CR2 |= ADC_CR2_SWSTART;

    while(!(ADC1->SR & ADC_SR_EOC));

    return ADC1->DR;
}

