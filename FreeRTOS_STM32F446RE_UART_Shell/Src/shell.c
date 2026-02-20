/*
 * shell.c
 *
 *  Created on: Feb 13, 2026
 *      Author: Sunil Sutar
 */


#include "shell.h"

#define CMD_SIZE 50

char cmdBuffer[CMD_SIZE];
uint8_t indx = 0;

extern SemaphoreHandle_t rxSemaphore;
extern QueueHandle_t ledQueue;
extern volatile char rxChar;

extern QueueHandle_t adcCmdQueue;
extern QueueHandle_t adcDataQueue;


void shell_task(void *pvParameters)
{
	UART2_SendString("\r\n******** UART Shell **********\r\n> ");

	UART2_SendString("Shell Task\r\n");

	while(1)
	{
		 xSemaphoreTake(rxSemaphore, portMAX_DELAY);

		 char ch = rxChar;

		 if(ch == '\r' || ch == '\n')
		 {
			cmdBuffer[indx] = '\0';
			UART2_SendString("\r\n");

			if(strcmp(cmdBuffer, "led on") == 0)
			{
				//LED_On();
				UART2_SendString("LED ON\r\n");
			    LedCommand_t cmd = LED_CMD_ON;
			    xQueueSend(ledQueue, &cmd, portMAX_DELAY);
			}
			else if(strcmp(cmdBuffer, "led off") == 0)
			{
//				LED_Off();
				UART2_SendString("LED OFF\r\n");
			    LedCommand_t cmd = LED_CMD_OFF;
			    xQueueSend(ledQueue, &cmd, portMAX_DELAY);

			}
			else if(strcmp(cmdBuffer, "led toggle") == 0)
			{
				UART2_SendString("LED Toggle\r\n");
			    LedCommand_t cmd = LED_CMD_TOGGLE;
			    xQueueSend(ledQueue, &cmd, portMAX_DELAY);
			}
			else if(strcmp(cmdBuffer, "led blink") == 0)
			{
				UART2_SendString("LED BLINK\r\n");
			    LedCommand_t cmd = LED_CMD_BLINK;
			    xQueueSend(ledQueue, &cmd, portMAX_DELAY);
			}
			else if(strcmp(cmdBuffer, "adc read") == 0)
			{
			    AdcCommand_t cmd = ADC_CMD_SINGLE;
			    xQueueSend(adcCmdQueue, &cmd, portMAX_DELAY);
			}
			else if(strcmp(cmdBuffer, "adc auto") == 0)
			{
			    AdcCommand_t cmd = ADC_CMD_AUTO;
			    xQueueSend(adcCmdQueue, &cmd, portMAX_DELAY);
			}
			else if(strcmp(cmdBuffer, "adc stop") == 0)
			{
			    AdcCommand_t cmd = ADC_CMD_STOP;
			    xQueueSend(adcCmdQueue, &cmd, portMAX_DELAY);
			}
			else if(strcmp(cmdBuffer, "help") == 0)
			{
				UART2_SendString("Supported Commands:\r\n");
				UART2_SendString("led on\r\n");
				UART2_SendString("led off\r\n");
				UART2_SendString("led toggle\r\n");
				UART2_SendString("led blink\r\n");
				UART2_SendString("adc read\r\n");
				UART2_SendString("adc auto\r\n");
				UART2_SendString("adc stop\r\n");
			}
			else
			{
				UART2_SendString("Unknown Command\r\n");
			}

			indx = 0;
			UART2_SendString("> ");
		}
		else
		{
			if(indx < CMD_SIZE-1)
			{
				cmdBuffer[indx++] = ch;
				UART2_SendChar(ch);   // echo
			}
		}
	}
}


void led_task(void *pvParameters)
{
    LedCommand_t receivedCmd;
    uint8_t blinkMode = 0;

    UART2_SendString("LED Task\r\n");

    while(1)
    {
        if(xQueueReceive(ledQueue, &receivedCmd, 0))
        {
            switch(receivedCmd)
            {
                case LED_CMD_ON:
                	blinkMode = 0;
                    LED_On();
                    break;

                case LED_CMD_OFF:
                	blinkMode = 0;
                    LED_Off();
                    break;

                case LED_CMD_TOGGLE:
                	 blinkMode = 0;
                    LED_Toggle();
                    vTaskDelay(pdMS_TO_TICKS(100));
                    break;
                case LED_CMD_BLINK:
                     blinkMode = 1;
                     break;
            }
        }

        if(blinkMode)
        {
            LED_Toggle();
            vTaskDelay(pdMS_TO_TICKS(500));
        }
        else
        {
            vTaskDelay(pdMS_TO_TICKS(50));
        }
    }
}

void adc_task(void *params)
{
	char buffer[30];
    AdcCommand_t receivedCmd;
    uint8_t autoMode = 0;
    uint16_t adcValue = 1000;

    UART2_SendString("ADC Task\r\n");

    while(1)
    {
        if(xQueueReceive(adcCmdQueue, &receivedCmd, pdMS_TO_TICKS(10)))
        {
        	UART2_SendString("Command Received\r\n");

            switch(receivedCmd)
            {
                case ADC_CMD_SINGLE:
                	UART2_SendString("ADC_CMD_SINGLE\r\n");
                	adcValue = ADC1_Read();
                    xQueueSend(adcDataQueue, &adcValue, portMAX_DELAY);
                    sprintf(buffer, "ADC Value: %d\r\n", adcValue);
                    UART2_SendString(buffer);
                    UART2_SendString("> ");
                    break;

                case ADC_CMD_AUTO:
                	autoMode = 1;
                    break;

                case ADC_CMD_STOP:
                	autoMode = 0;
                	UART2_SendString("ADC_CMD_STOP\r\n");
                    break;
            }
        }

        if(autoMode)
        {
        	UART2_SendString("ADC_CMD_AUTO\r\n");
        	adcValue = ADC1_Read();
            xQueueSend(adcDataQueue, &adcValue, portMAX_DELAY);
            sprintf(buffer, "ADC Value: %d\r\n", adcValue);
            UART2_SendString(buffer);
            UART2_SendString("> ");
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
        else
        {
            vTaskDelay(pdMS_TO_TICKS(50));
        }

    }
}
