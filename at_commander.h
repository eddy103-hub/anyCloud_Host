/*
 * app.h
 *
 *  Created on: 30 Oct 2023
 *      Author: M67732
 */

#ifndef AT_COMMANDER_H_
#define AT_COMMANDER_H_

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "app.h"

/*
 * Function prototypes
 */

void ATCMD_Print(const char *format, ...);
void ATCMD_Task(void);
uint8_t ATCMD_ReadLine(void);
int ATCMD_strcon(char a[], char b[]);
void createPubMQTTString(void);
void initModule(void);
void updateRootCert(void);
void configureWLAN(void);
void startWLAN(void);
void waitForAPConnect(void);
void configureCloud(void);
void subscribeDPS(void);
void publishDPSPut(void);
void publishDPSGet(void);
void getAssignedHub(void);
void disconnectMQTT(void);
void reconnectCloud(void);
void azureSubscribe_1(void);
void azureSubscribe_2(void);
void azureSubscribe_3(void);
void publishTelemetry(void);
void timeoutHandling(void);

/*
 * Enumeration defining the states of the AT Command module
 */
typedef enum {
    STATE_INIT,
	STATE_ROOT_CERT_UPDATE,
	STATE_CONFUGURE_WLAN,
    STATE_START_WLAN,
    STATE_WAIT_FOR_AP_CONNECT,
    STATE_CONFIGURE_CLOUD,
	STATE_SUB_DPS,
	STATE_PUB_DPS_REG_PUT,
	STATE_PUB_DPS_REG_GET,
	STATE_AZURE_GET_ASSIGN_HUB,
	STATE_MQTT_DISCONNECT,
	STATE_RECONNECT_CLOUD,
	STATE_AZURE_SUB_1,
	STATE_AZURE_SUB_2,
	STATE_AZURE_SUB_3,
    STATE_PUBLISH_CLOUD,
    STATE_TIMEOUT,
} ATCMD_STATES;


#endif /* AT_COMMANDER_H_ */
