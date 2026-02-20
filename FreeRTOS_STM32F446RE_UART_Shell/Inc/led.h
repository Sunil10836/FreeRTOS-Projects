/*
 * led.h
 *
 *  Created on: Feb 14, 2026
 *      Author: Sunil Sutar
 */

#ifndef LED_H_
#define LED_H_

#define GPIOA_CLK_EN	0
#define LED_PIN			5

void LED_Init(void);
void LED_On(void);
void LED_Off(void);
void LED_Toggle(void);
void delay(void);

#endif /* LED_H_ */
