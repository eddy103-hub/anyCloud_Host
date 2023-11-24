/*
 * app.h
 *
 *  Created on: 6 Nov 2023
 *      Author: M67732
 */

#ifndef APP_H_
#define APP_H_

#include "DAVE.h"
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

// Function declarations
void setTimeout(int16_t sec);
bool isTimeout(void);
double generateRandomTemperature(void);

/* External variables to be used */
extern volatile uint16_t u16timerFlag;   // Timer flag indicating an event
extern volatile bool bTxReady;           // Transmission ready flag
extern volatile bool bRxReady;           // Reception ready flag

#endif /* APP_H_ */
