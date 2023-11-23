/*

 * at_commander.c
 *
 *  Created on: 7 Nov 2023
 *      Author: M67732
 */

#include "at_commander.h"

/*
 * Root certificate for secure communication
 */
const char digiCertGlobalRootG2[] = CERT;

/*
 * Buffers for AT commands and MQTT-related data
 */
char ATCMD_TransmittBuffer[ATCMD_PRINT_BUFFER_SIZE];
char ATCMD_ReceiveBuffer[ATCMD_RECEIVE_BUFFER_SIZE];

/*
 * Configuration parameters for MQTT and Azure IoT Hub
 */
char json[MQTT_PAYLOAD_SIZE];
char buffer[2048];
char *resultPtrMQTTPUB;
char *operationID;
char *azureAssignedHub;
char myBrokerAdress[75] = MY_BROKER_ADRESS;
char deviceId[50] = CLIENT_ID;
char brokerUsername[150] = BROKER_USER_NAME;
char pubString[MQTT_PAYLOAD_SIZE];
char bufferPayload[MQTT_PAYLOAD_SIZE];
const char okResponse[] = "OK\r\n>";

/*
 * Enumeration to represent different states in the state machine
 */
ATCMD_STATES ATCMD_state = STATE_INIT;
uint8_t ATCMD_Error_Code = 0;

/*
 * Function: createPubMQTTString
 * ------------------------------
 * Creates a formatted MQTT payload string for telemetry and a JSON payload.
 * Uses the global variables device_id, bufferPayload, and pubString.
 */
void createPubMQTTString() {

	double temperature = generateRandomTemperature();

	snprintf(bufferPayload,
	MQTT_PAYLOAD_SIZE,
			"AT+MQTTPUB=0,0,0,\""PUB_TOPIC_TELEMETRY"\",\""PUB_TELEMETRY_TEMPERATURE"\"\r\n",
			deviceId, temperature);

	snprintf(pubString, MQTT_PAYLOAD_SIZE,
			"{\\\"payload\\\" : {\\\"modelId\\\" : \\\"%s\\\"}}", MODEL_ID);
}

/*
 * Function: initModule
 * --------------------
 * Initializes the module by turning off echo and resetting the module.
 */
void initModule(void) {

	ATCMD_Print("ATE%d\r\n", TURN_OFF_ECHO);
	ATCMD_Print("AT+RST\r\n");
	ATCMD_state = STATE_ROOT_CERT_UPDATE;

}

/*
 * Function: updateRootCert
 * ------------------------
 * Updates the root certificate for secure communication by sending the certificate to the module.
 */
void updateRootCert(void) {
	sprintf(buffer, "AT+LOADCERT=%d,\"DigiCertGlobalRootG2\"\r\n",
			strlen(digiCertGlobalRootG2));
	ATCMD_Print("ATE%d\r\n", TURN_OFF_ECHO);
	ATCMD_Print((char*) buffer, strlen(buffer));
	ATCMD_Print(digiCertGlobalRootG2);
	ATCMD_state = STATE_CONFUGURE_WLAN;

}

/*
 * Function: configureWLAN
 * -----------------------
 * Configures WLAN settings, such as SSID, security type, password, NTP server, and channel.
 */
void configureWLAN(void) {
	ATCMD_Print("ATE%d\r\n", TURN_OFF_ECHO);
	ATCMD_Print("AT+WSTAC=%d,\"%s\"\r\n", ID_SSID, MY_AP);
	ATCMD_Print("AT+WSTAC=%d,%d\r\n", ID_SEC_TYPE, PAR_SEC_TYPE_WPA2);
	ATCMD_Print("AT+WSTAC=%d,\"%s\"\r\n", ID_CREDENTIALS, MY_PASSWORD);
	ATCMD_Print("AT+WSTAC=%d,%d\r\n", ID_CHANNEL, PAR_ANY_CHANNEL);
	ATCMD_Print("AT+WSTAC=%d,\"%s\"\r\n", ID_NTP_SVR, NTP_SERVER);
	ATCMD_Print("AT+WSTAC=%d,%d\r\n", ID_NTP_STATIC, PAR_NTP_STATIC);
	ATCMD_state = STATE_START_WLAN;
}
/*
 * Function: startWLAN
 * -------------------
 * Initiates the WLAN connection using the configured parameters.
 */
void startWLAN(void) {
	ATCMD_Print("ATE%d\r\n", TURN_OFF_ECHO);
	ATCMD_Print("AT+WSTA=%d\r\n", PAR_USE_CONFIGURATION);
	ATCMD_ReadLine();
	ATCMD_ReadLine();
	ATCMD_state = STATE_WAIT_FOR_AP_CONNECT;

}

/*
 * Function: waitForAPConnect
 * --------------------------
 * Checks the status of the WLAN connection and transitions to the next state if connected.
 */
void waitForAPConnect(void) {
	ATCMD_Print("ATE%d\r\n", TURN_OFF_ECHO);
	ATCMD_Print("AT+WSTA\r\n");
	ATCMD_ReadLine();
	if (ATCMD_strcon(ATCMD_ReceiveBuffer, "+WSTALD\r\n") != 1) {
		ATCMD_state = STATE_CONFIGURE_CLOUD;
	}

}

/*
 * Function: configureCloud
 * ------------------------
 * Configures cloud-related parameters, such as MQTT broker address, port, client ID, and username.
 */
void configureCloud(void) {
	ATCMD_Print("ATE%d\r\n", TURN_OFF_ECHO);
	ATCMD_Print("AT+MQTTC=%d,\"%s\"\r\n", ID_MQTT_BROKER_ADDR,
	MY_BROKER_ADRESS);
	ATCMD_Print("AT+MQTTC=%d,%d\r\n", ID_MQTT_BROKER_PORT,
	MQTT_BROKER_PORT);
	ATCMD_Print("AT+MQTTC=%d,\"%s\"\r\n", ID_MQTT_CLIENT_ID, CLIENT_ID);
	ATCMD_Print("AT+MQTTC=%d,\"%s\"\r\n", ID_MQTT_USERNAME,
	BROKER_USER_NAME);

	ATCMD_Print("AT+MQTTC=%d,%d\r\n", ID_MQTT_TLS_CONF_IDX, TLS_ENABLE);
	ATCMD_Print("AT+MQTTCONN=%d\r\n", MQTT_RCLEAN);
	ATCMD_state = STATE_SUB_DPS;
}
/*
 * Function: subscribeDPS
 * ----------------------
 * Subscribes to the DPS registration topic.
 */
void subscribeDPS(void) {
	ATCMD_Print("ATE%d\r\n", TURN_OFF_ECHO);
	ATCMD_Print("AT+MQTTSUB=\"%s\",%d\r\n", SUB_DPS_REGISTRATION_TOPIC,
	MQTT_QOS);
	ATCMD_state = STATE_PUB_DPS_REG_PUT;

}
/*
 * Function: publishDPSPut
 * ------------------------
 * Publishes a message to the DPS PUT topic with a formatted payload.
 */
void publishDPSPut(void) {
	createPubMQTTString();
	ATCMD_Print("ATE%d\r\n", TURN_OFF_ECHO);
	ATCMD_Print("AT+MQTTPUB=%d,%d,%d,\"%s\",\"%s\"\r\n", MQTT_DUP, MQTT_QOS,
	MQTT_NOT_RETAIN,
	PUB_TOPIC_DPS_PUT, pubString);
	ATCMD_state = STATE_PUB_DPS_REG_GET;
}
/*
 * Function: publishDPSGet
 * ------------------------
 * Publishes a message to the DPS GET topic based on the response from the DPS PUT operation.
 */
void publishDPSGet(void) {

	// Assuming ATCMD_ReceiveBuffer contains the received MQTT publish response
	resultPtrMQTTPUB = strstr(ATCMD_ReceiveBuffer, "+MQTTPUB:");
	if (resultPtrMQTTPUB != NULL) {
		// Move the pointer to the beginning of the MQTT publish response
		resultPtrMQTTPUB += strlen("+MQTTPUB:");

	}

	if ((operationID = strstr(resultPtrMQTTPUB, "\"operationId\":\""))) {
		operationID += strlen("\"operationId\":\"");
		*(strstr(operationID, "\"")) = 0;
		sprintf(buffer, "AT+MQTTPUB=0,0,0,\""PUB_TOPIC_DPS_GET"\",\"""\"\r\n",
				operationID);
		ATCMD_Print("ATE%d\r\n", TURN_OFF_ECHO);
		ATCMD_Print(buffer);

		// if Azure +MQTTPUB response does not contain "retry-after=",
		// move to next state, otherwise retry in 1 SECOND
		if (!strstr(resultPtrMQTTPUB, "retry-after="))
			ATCMD_state = STATE_AZURE_GET_ASSIGN_HUB;
	}

}

/*
 * Function: getAssignedHub
 * ------------------------
 * Extracts the assigned hub information from the MQTT publish response and updates configuration.
 */
void getAssignedHub(void) {

	// Assuming ATCMD_ReceiveBuffer contains the received MQTT publish response
	resultPtrMQTTPUB = strstr(ATCMD_ReceiveBuffer, "+MQTTPUB:");
	if (resultPtrMQTTPUB != NULL) {
		// Move the pointer to the beginning of the MQTT publish response
		resultPtrMQTTPUB += strlen("+MQTTPUB:");

	}

	if ((azureAssignedHub = strstr(resultPtrMQTTPUB, "\"assignedHub\":\""))) {

		azureAssignedHub += strlen("\"assignedHub\":\"");
		*(strstr(azureAssignedHub, "\"")) = 0;

		sprintf(myBrokerAdress, azureAssignedHub);
		sprintf(brokerUsername, "%s/%s/?api-version=2021-04-12",
				azureAssignedHub, deviceId);

	}

	ATCMD_state = STATE_MQTT_DISCONNECT;

}
/*
 * Function: disconnectMQTT
 * ------------------------
 * Disconnects from the MQTT broker.
 */
void disconnectMQTT(void) {

	ATCMD_Print("ATE%d\r\n", TURN_OFF_ECHO);
	ATCMD_Print("AT+MQTTDISCONN\r\n");
	ATCMD_state = STATE_RECONNECT_CLOUD;
}
/*
 * Function: reconnectCloud
 * ------------------------
 * Reconnects to the cloud after disconnecting, updating parameters based on the assigned hub.
 */
void reconnectCloud(void) {
	ATCMD_Print("ATE%d\r\n", TURN_OFF_ECHO);
	ATCMD_Print("AT+MQTTC=%d,\"%s\"\r\n", ID_MQTT_BROKER_ADDR, myBrokerAdress);
	ATCMD_Print("AT+MQTTC=%d,%d\r\n", ID_MQTT_BROKER_PORT,
	MQTT_BROKER_PORT);
	ATCMD_Print("AT+MQTTC=%d,\"%s\"\r\n", ID_MQTT_CLIENT_ID, deviceId);
	ATCMD_Print("AT+MQTTC=%d,\"%s\"\r\n", ID_MQTT_USERNAME, brokerUsername);

	ATCMD_Print("AT+MQTTC=%d,%d\r\n", ID_MQTT_TLS_CONF_IDX, TLS_ENABLE);
	ATCMD_Print("AT+MQTTCONN=%d\r\n", MQTT_RCLEAN);
	ATCMD_state = STATE_AZURE_SUB_1;

}
/*
 * Function: azureSubscribe_1
 * --------------------------
 * Subscribes to the first Azure IoT Hub topic.
 */
void azureSubscribe_1(void) {
	ATCMD_Print("ATE%d\r\n", TURN_OFF_ECHO);
	ATCMD_Print("AT+MQTTSUB=\"%s\",%d\r\n", TOPIC_IOTC_METHOD_REQUEST,
	MQTT_SUB_QOS);
	ATCMD_state = STATE_AZURE_SUB_2;

}
/*
 * Function: azureSubscribe_2
 * --------------------------
 * Subscribes to the second Azure IoT Hub topic.
 */
void azureSubscribe_2(void) {
	ATCMD_Print("ATE%d\r\n", TURN_OFF_ECHO);
	ATCMD_Print("AT+MQTTSUB=\"%s\",%d\r\n", TOPIC_IOTC_PROPERTY_DESIRED,
	MQTT_SUB_QOS);
	ATCMD_state = STATE_AZURE_SUB_3;

}
/*
 * Function: azureSubscribe_3
 * --------------------------
 * Subscribes to the third Azure IoT Hub topic.
 */
void azureSubscribe_3(void) {
	ATCMD_Print("ATE%d\r\n", TURN_OFF_ECHO);
	ATCMD_Print("AT+MQTTSUB=\"%s\",%d\r\n", TOPIC_IOTC_PROPERTY_RESPONSE,
	MQTT_SUB_QOS);
	ATCMD_state = STATE_PUBLISH_CLOUD;

}
/*
 * Function: publishTelemetry
 * --------------------------
 * Publishes telemetry data to the cloud with a formatted payload.
 */
void publishTelemetry(void) {
	createPubMQTTString();
	ATCMD_Print("ATE%d\r\n", TURN_OFF_ECHO);
	ATCMD_Print(bufferPayload);
	//I have to change it
	//ATCMD_Print("AT+MQTTPUB=%d,%d,%d,\"%s\",\"%s\"\r\n", MQTT_DUP, MQTT_QOS,
	//MQTT_NOT_RETAIN, pub_topic_telemetry, json);
	setTimeout(10);
	ATCMD_state = STATE_TIMEOUT;

}
/*
 * Function: timeoutHandling
 * -------------------------
 * Handles the timeout scenario by transitioning to the next state if a timeout occurs.
 */
void timeoutHandling(void) {
	if (isTimeout()) {
		ATCMD_state = STATE_PUBLISH_CLOUD;
	}

}
/*
 * Function: ATCMD_Task
 * --------------------
 * Executes the appropriate task based on the current state of the state machine.
 */
void ATCMD_Task(void) {
	switch (ATCMD_state) {
	case STATE_INIT:
		initModule();
		break;
	case STATE_ROOT_CERT_UPDATE:
		updateRootCert();
		break;
	case STATE_CONFUGURE_WLAN:
		configureWLAN();
		break;
	case STATE_START_WLAN:
		startWLAN();
		break;
	case STATE_WAIT_FOR_AP_CONNECT:
		waitForAPConnect();
		break;
	case STATE_CONFIGURE_CLOUD:
		configureCloud();
		break;
	case STATE_SUB_DPS:
		subscribeDPS();
		break;
	case STATE_PUB_DPS_REG_PUT:
		publishDPSPut();
		break;
	case STATE_PUB_DPS_REG_GET:
		publishDPSGet();
		break;
	case STATE_AZURE_GET_ASSIGN_HUB:
		getAssignedHub();
		break;
	case STATE_MQTT_DISCONNECT:
		disconnectMQTT();
		break;
	case STATE_RECONNECT_CLOUD:
		reconnectCloud();
		break;
	case STATE_AZURE_SUB_1:
		azureSubscribe_1();
		break;
	case STATE_AZURE_SUB_2:
		azureSubscribe_2();
		break;
	case STATE_AZURE_SUB_3:
		azureSubscribe_3();
		break;
	case STATE_PUBLISH_CLOUD:
		publishTelemetry();
		break;
	case STATE_TIMEOUT:
		timeoutHandling();
		break;
	default:
		break;
	}
}

/*
 * Function: ATCMD_Print
 * ---------------------
 * Sends formatted AT commands to the module, handles transmission and reception,
 * and checks for potential errors.
 */
void ATCMD_Print(const char *format, ...) {

	size_t len = 0;
	va_list args = { 0 };

	int ret_length = 0;
	va_start(args, format);
	bool abort = false;

	len = vsnprintf(ATCMD_TransmittBuffer, ATCMD_PRINT_BUFFER_SIZE, format,
			args);

	va_end(args);

	ATCMD_Error_Code = 0;

	if ((len > 0) && (len < ATCMD_PRINT_BUFFER_SIZE)) {
		ATCMD_TransmittBuffer[len] = '\0';
	}

	UART_STATUS_t tx_status = UART_Transmit(&UART_0,
			(uint8_t *) &ATCMD_TransmittBuffer[0], len);
	if (tx_status != UART_STATUS_SUCCESS) {
		ATCMD_Error_Code = 1;
		return;
	}

	setTimeout(2);
	while (bTxReady == false) {
		if (isTimeout()) {
			return;
		}
	}
	bTxReady = false;

	// MVo: maximum number of bytes to receive is buffer size, not len of transmitted command
	for (int ix = 0; ix < ATCMD_RECEIVE_BUFFER_SIZE; ix++) {
		bRxReady = false;
		UART_STATUS_t rx_status = UART_Receive(&UART_0,
				(uint8_t *) &ATCMD_ReceiveBuffer[ix], 1);
		if (rx_status != UART_STATUS_SUCCESS) {
			ATCMD_Error_Code = 1;
			return;
		}

		setTimeout(3);
		while (bRxReady == false) {

			if (isTimeout()) {
				UART_AbortReceive(&UART_0);
				abort = true;
				break;
				//return;
			}
		}

		ret_length++;

		if (abort)
			break;

	}

	// MVo: check only for length of received string
	for (int ix = 0; ix < ret_length; ix++) {
		if (okResponse[ix] != ATCMD_ReceiveBuffer[ix]) {
			ATCMD_Error_Code = 1;
			return;
		}
	}
}

/*
 * Function: ATCMD_ReadLine
 * ------------------------
 * Reads a line from the receive buffer and returns the number of characters read.
 */
uint8_t ATCMD_ReadLine(void) {

	uint8_t ix;
	uint8_t byte;

	for (ix = 0; ix < ATCMD_RECEIVE_BUFFER_SIZE; ix++) {

		setTimeout(2);
		while (bRxReady == false) {
			if (isTimeout()) {
				return 0;
			}
		}

		UART_STATUS_t rx_status = UART_StartReceiveIRQ(&UART_0, &byte, 1);
		if (rx_status != UART_STATUS_SUCCESS) {
			return 0;
		}

		if (byte == '\n') {
			ATCMD_ReceiveBuffer[ix] = 0;
			break;
		} else {
			ATCMD_ReceiveBuffer[ix] = byte;
		}
	}
	return ix;
}

/*
 * Function: ATCMD_strcon
 * ----------------------
 * Compares two strings and returns 1 if they are equal, 0 otherwise.
 */
int ATCMD_strcon(char a[], char b[]) {
	int i = 0;

	while (a[i] != '\0' && b[i] != '\0') {
		if (a[i] != b[i]) {
			break;
		}
		i++;
	}
	if (b[i] == '\0') {
		return 1;
	} else {
		return 0;
	}
}
