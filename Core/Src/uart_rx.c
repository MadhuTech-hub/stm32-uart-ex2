/*
 * uart.rx.c
 *
 *  Created on: Sep 6, 2025
 *      Author: Madhusudhan Y V
 */

#include "uart_rx.h"
#include "stdio.h"
#include "string.h"

extern UART_HandleTypeDef huart2;

void uart_wait_for_new_line(void)
{
  char inputChar[1];
  char myString[20];
  static uint8_t ctr = 0;

   //Reset the string if counter is zero
  if (ctr==0) {
    memset(myString,0,20);
  }
  //Initialize Input char with NULL
  inputChar[0] = '\0';

  //Wait for one character from UART
  //If data received, store in mSstring at the end
	if (HAL_UART_Receive(&huart2, (uint8_t*) inputChar, 1, 5) == HAL_OK) {
		myString[ctr++] = inputChar[0];
		//If reset counter when it reaches max
		if (ctr >= 20) {
			ctr = 0;
		}
	}

	//Check if input string is equal to "stm"
	//If "stm", then send OK to uart
	//else send "Error" to uart
	if (inputChar[0] == '\r') {
		if (strncmp(myString, "stm32uc\r", 4) == 0) {
			sprintf(myString, "OK\r\n");
		} else {
			sprintf(myString, "Error\r\n");
		}
		HAL_UART_Transmit(&huart2, (uint8_t*) myString, strlen(myString), 10);
		//Reset the counter, wait for new command
		ctr = 0;
	}
}
