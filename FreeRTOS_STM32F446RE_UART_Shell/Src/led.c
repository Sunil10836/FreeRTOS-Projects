/*
 * led.c
 *
 *  Created on: Feb 14, 2026
 *      Author: Sunil Sutar
 */

#include <stm32f446xx.h>
#include "led.h"

void LED_Init(void)
{
	//LED => PA5
	//1. Enable Clock for GPIOA
	RCC->AHB1ENR |= (1 << GPIOA_CLK_EN);

	/* 2. Set PA5 as output */
	GPIOA->MODER &= ~(3 << (5 * 2));	//Clear Bits
	GPIOA->MODER |= (1 << (5 * 2));		//Set as Output

	 /* 3. Set output type as Push-Pull */
	GPIOA->OTYPER &= ~(1 << 5);

    /* 4. Set output speed */
    GPIOA->OSPEEDR |= (1 << (5 * 2));

}

void LED_On(void)
{
	GPIOA->ODR |= (1 << LED_PIN);
}

void LED_Off(void)
{
	GPIOA->ODR &= ~(1 << LED_PIN);
}

void LED_Toggle(void)
{
	GPIOA->ODR ^= (1 << LED_PIN);
}

void delay(void)
{
    for (volatile int i = 0; i < 500000; i++);
}
