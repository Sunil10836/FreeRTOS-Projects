/*
 * shell.h
 *
 *  Created on: Feb 13, 2026
 *      Author: Sunil Sutar
 */

#ifndef SHELL_H_
#define SHELL_H_

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "uart.h"
#include "led.h"
#include "adc.h"

typedef enum
{
    LED_CMD_ON,
    LED_CMD_OFF,
    LED_CMD_TOGGLE,
	LED_CMD_BLINK
}LedCommand_t;

typedef enum
{
    ADC_CMD_SINGLE,
    ADC_CMD_AUTO,
    ADC_CMD_STOP
}AdcCommand_t;

void shell_task(void *pvParameters);
void led_task(void *pvParameters);
void adc_task(void *pvParameters);

#endif /* SHELL_H_ */
