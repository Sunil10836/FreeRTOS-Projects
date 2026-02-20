/*
 * main.c
 *
 *  Created on: Feb 13, 2026
 *      Author: Sunil Sutar
 */


#include <stm32f446xx.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "shell.h"
#include "uart.h"
#include "led.h"
#include "adc.h"

uint32_t SystemCoreClock = 16000000;

//Global Variables
SemaphoreHandle_t uartMutex;
SemaphoreHandle_t rxSemaphore;
QueueHandle_t ledQueue;

QueueHandle_t adcCmdQueue;
QueueHandle_t adcDataQueue;

int main()
{
	//Initializations
	UART2_Init();
	LED_Init();
	ADC1_Init();

	UART2_SendString("FreeRTOS Project\r\n");

    uartMutex = xSemaphoreCreateMutex();
    rxSemaphore = xSemaphoreCreateBinary();
    ledQueue = xQueueCreate(5, sizeof(LedCommand_t));

    //ADC Queues
    adcCmdQueue  = xQueueCreate(5, sizeof(AdcCommand_t));
    adcDataQueue = xQueueCreate(5, sizeof(uint16_t));

    //Creating Tasks
    xTaskCreate(shell_task, "Shell", 256, NULL, 3, NULL);
    xTaskCreate(led_task, "LED", 256, NULL, 1, NULL);
    xTaskCreate(adc_task, "ADC", 512, NULL, 2, NULL);

    vTaskStartScheduler();

    while(1)
    {

    }

}











