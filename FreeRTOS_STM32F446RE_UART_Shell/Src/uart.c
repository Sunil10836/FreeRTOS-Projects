/*
 * uart.c
 *
 *  Created on: Nov 3, 2025
 *      Author: Sunil
 */

#include "uart.h"
#include "stm32f446xx.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "led.h"

extern SemaphoreHandle_t uartMutex;
extern SemaphoreHandle_t rxSemaphore;

volatile char rxChar;

void UART2_Init()
{
	/********** Configure USART GPIO Pins ***********/
	//1. Enable Clock for GPIOA
	RCC->AHB1ENR |= GPIOAEN;

	//2. Set Mode to Alt Fun for GPIO Pins : PA2 and PA3
	GPIOA->MODER &= ~(0x3 << 2 * 2);	//Clear bits of PA2 Pins : 2 bits for each pin Hence 11 i.e. 0x3
	GPIOA->MODER |= (0x2 << 2 * 2);		//Set 10 at bit 4 and bit 5 : 10 i.e. 2 => Alt Fun.

	GPIOA->MODER &= ~(0x3 << 2 * 3);	//Clear bits of PA3 Pins
	GPIOA->MODER |= (0x2 << 2 * 3);		//Set 10 at bit 6 and bit 7	: 10 i.e. 2 => Alt Fun.

	//3. Set PA2  and PA3 Alt Fun Type to USART2_TX and USART2_RX : AF7
	GPIOA->AFR[0] &= ~(0xF << 4 * 2); 	//Clear bits of PA2 Pins: Bit 8 to 11 => 4 bit for each pin 1111 => 0xF
	GPIOA->AFR[0] |= (0x7 << 4 * 2);	//Set : AF7 => 0111 at bit 8 to bit 11
	GPIOA->AFR[0] &= ~(0xF << 4 * 3); 	//Clear bits of PA3 Pins: Bit 12 to 15 => 4 bit for each pin 1111 => 0xF
	GPIOA->AFR[0] |= (0x7 << 4 * 3);	//Set : AF7 => 0111 at bit 12 to bit 15

	/********** Configure USART Module *************/
	//3. Enable Clock for USART2
	RCC->APB1ENR |= USART2EN;

	//4. Configure UART Buadrate
	USART2->BRR = 0x88;				//115200 @ 16MHz

	//5. Configure the transfer direction : both Tx and Rx
	USART2->CR1 |= ((1 << USART2_CR1_TE) | (1 << USART2_CR1_RE));

	//Enable Receiver Interrupt(RXNEIE)
	USART2->CR1 |= (1 << USART2_CR1_RXNEIE);

	//6. Enable UART Module
	USART2->CR1 |= (1 << USART2_CR1_UE);

	//Enable UART2_IRQ
	NVIC_SetPriority(USART2_IRQn, 6);	// Must be > 5
	NVIC_EnableIRQ(USART2_IRQn);

}

void UART2_SendChar(int ch)
{
	//1. Make sure UART Transmit Data Register Empty
	while( !(USART2->SR & (USART2_SR_TXE))){}

	//2. Write to UART Transmit Data Register
	USART2->DR = (ch & 0xFF);

}

void UART2_SendString(const char *str)
{
	//xSemaphoreTake(uartMutex, portMAX_DELAY);

	while(*str)
	{
		UART2_SendChar(*str++);
	}

	//xSemaphoreGive(uartMutex);
}

void USART2_IRQHandler(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if(USART2->SR & USART_SR_RXNE)
    {
        rxChar = USART2->DR;
        xSemaphoreGiveFromISR(rxSemaphore, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}
