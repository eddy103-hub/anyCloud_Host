/*
 * app.c
 *
 *  Created on: 6 Nov 2023
 *      Author: M67732
 */

#include "app.h"

// Global variables
volatile uint16_t u16timerFlag = 0;        // Flag indicating timer event
uint16_t u16myTime = 0;                      // Timeout value in seconds

bool bMyTimeoutFlag = false;                 // Timeout flag
volatile bool bTxReady = false;              // Transmission ready flag
volatile bool bRxReady = false;              // Reception ready flag

// Function to generate a random temperature value
double generateRandomTemperature() {

    return (double)rand() / RAND_MAX * 100;
}

// Set timeout value in seconds
void setTimeout(int16_t sec) {

	u16myTime = sec;
}

// Check if a timeout has occurred
bool isTimeout(void) {

	if (bMyTimeoutFlag) {
		bMyTimeoutFlag = false;              // Reset the timeout flag
		return true;
	}
	return false;
}

// Timer0 interrupt service routine
void Timer0_ISR(void) {

	u16timerFlag = 1;

	if (u16myTime) {
		u16myTime--;
		if (u16myTime == 0)
			bMyTimeoutFlag = true; // Set timeout flag when the timer reaches zero
	}
}

// Callback function for the end of transmission
void endOfTransmissionCB_ISR(void) {

	bTxReady = true;                          // Set transmission ready flag
}

// Callback function for the end of reception
void endOfReceptionCB_ISR(void) {

	bRxReady = true;                          // Set reception ready flag
}
