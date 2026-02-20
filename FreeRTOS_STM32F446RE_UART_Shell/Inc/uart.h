/*
 * uart.h
 *
 *  Created on: Nov 3, 2025
 *      Author: Sunil
 */

#ifndef UART_H_
#define UART_H_

#include <stm32f446xx.h>
#include <stdint.h>

#define USART2EN				(1U << 17)
#define GPIOAEN					(1U << 0)

#define SYS_CLK_FREQ			16000000 			//Default clock HSI = 16MHz
#define APB1_CLK_FREQ			SYS_CLK_FREQ
#define UART_BAUDRATE			115200

#define USART2_CR1_RE			2
#define USART2_CR1_TE			3
#define USART2_CR1_RXNEIE		5
#define USART2_CR1_TCIE			6
#define USART2_CR1_TXEIE		7
#define USART2_CR1_UE			13

#define USART2_SR_TXE			(1U<<7)

void UART2_Init();
void UART2_SendChar(int ch);
void UART2_SendString(const char *str);


#endif /* UART_H_ */
