/*
 * constants.h
 *
 *  Created on: 6 Nov 2023
 *      Author: M67732
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

/**
 * DigiCertGlobalRootG2 from
 * https://github.com/Azure/azure-iot-sdk-c/blob/main/certs/certs.c
 */

#define CERT  "-----BEGIN CERTIFICATE-----\n\
MIIDjjCCAnagAwIBAgIQAzrx5qcRqaC7KGSxHQn65TANBgkqhkiG9w0BAQsFADBh\n\
MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n\
d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBH\n\
MjAeFw0xMzA4MDExMjAwMDBaFw0zODAxMTUxMjAwMDBaMGExCzAJBgNVBAYTAlVT\n\
MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n\
b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IEcyMIIBIjANBgkqhkiG\n\
9w0BAQEFAAOCAQ8AMIIBCgKCAQEAuzfNNNx7a8myaJCtSnX/RrohCgiN9RlUyfuI\n\
2/Ou8jqJkTx65qsGGmvPrC3oXgkkRLpimn7Wo6h+4FR1IAWsULecYxpsMNzaHxmx\n\
1x7e/dfgy5SDN67sH0NO3Xss0r0upS/kqbitOtSZpLYl6ZtrAGCSYP9PIUkY92eQ\n\
q2EGnI/yuum06ZIya7XzV+hdG82MHauVBJVJ8zUtluNJbd134/tJS7SsVQepj5Wz\n\
tCO7TG1F8PapspUwtP1MVYwnSlcUfIKdzXOS0xZKBgyMUNGPHgm+F6HmIcr9g+UQ\n\
vIOlCsRnKPZzFBQ9RnbDhxSJITRNrw9FDKZJobq7nMWxM4MphQIDAQABo0IwQDAP\n\
BgNVHRMBAf8EBTADAQH/MA4GA1UdDwEB/wQEAwIBhjAdBgNVHQ4EFgQUTiJUIBiV\n\
5uNu5g/6+rkS7QYXjzkwDQYJKoZIhvcNAQELBQADggEBAGBnKJRvDkhj6zHd6mcY\n\
1Yl9PMWLSn/pvtsrF9+wX3N3KjITOYFnQoQj8kVnNeyIv/iPsGEMNKSuIEyExtv4\n\
NeF22d+mQrvHRAiGfzZ0JFrabA0UWTW98kndth/Jsw1HKj2ZL7tcu7XUIOGZX1NG\n\
Fdtom/DzMNU+MeKNhJ7jitralj41E6Vf8PlwUHBHQRFXGU7Aj64GxJUTFy8bJZ91\n\
8rGOmaFvE7FBcf6IKshPECBV1/MUReXgRPTqh5Uykw7+U0b6LJ3/iyK5S9kJRaTe\n\
pLiaWN0bfVKfjllDiIGknibVb63dDcY3fe0Dkhvld1927jyNxF1WW6LZZm6zNTfl\n\
MrY=\n\
-----END CERTIFICATE-----\n"


/**
 Section: AT Command Macro Declarations
 */
#define ATCMD_PRINT_BUFFER_SIZE       2048
#define ATCMD_RECEIVE_BUFFER_SIZE     1024

#define ID_SSID                 1
#define ID_SEC_TYPE             2
#define ID_CREDENTIALS          3
#define ID_CHANNEL              4
#define ID_NTP_SVR              12
#define ID_NTP_STATIC           13
#define TURN_OFF_ECHO           0
#define TURN_ON_ECHO            1
#define PAR_SEC_TYPE_WPA2       3
#define PAR_ANY_CHANNEL         255
#define PAR_NTP_STATIC          1
#define PAR_USE_CONFIGURATION   1

#define ID_MQTT_BROKER_ADDR     1
#define ID_MQTT_BROKER_PORT     2
#define ID_MQTT_CLIENT_ID       3
#define ID_MQTT_USERNAME        4
#define ID_MQTT_TLS_CONF_IDX    7
#define TLS_ENABLE				1
#define TLS_DISABLE				0

#define MQTT_BROKER_PORT        8883
#define MQTT_START_CONNECTION   1
#define MQTT_RCLEAN             1
#define MQTT_DUP               0
#define MQTT_QOS               0
#define MQTT_RETAIN            1
#define MQTT_NOT_RETAIN        0
#define MQTT_PAYLOAD_SIZE      256
#define MQTT_SUB_QOS			1


/**
 Section: Azure Macro Declarations
 */
#define MY_BROKER_ADRESS  				"global.azure-devices-provisioning.net"
#define BROKER_USER_NAME  				"0ne00B4880F/registrations/sn01237C3166A6D94401/api-version=2019-03-31"
#define CLIENT_ID       				"sn01237C3166A6D94401"
#define PUB_TOPIC_DPS_GET 				"$dps/registrations/GET/iotdps-get-operationstatus/?rid=2&operationId=%s"
#define PUB_TELEMETRY_TEMPERATURE 		"{\\\"temperature\\\":%2.2f}"
#define PUB_TOPIC_TELEMETRY 			"devices/%s/messages/events/"
#define MYTHING_ID						"0ne00B4880F"
#define MODEL_ID        				"dtmi:custom1g4130wwqxw:XMC4700RelaxKit_4an;1"
#define SUB_DPS_REGISTRATION_TOPIC   	"$dps/registrations/res/#"
#define PUB_TOPIC_PROPERTIES			"$iothub/twin/PATCH/properties/reported/?$rid=1"
#define PUB_TOPIC_DPS_PUT				"$dps/registrations/PUT/iotdps-register/?rid="
#define TOPIC_IOTC_PROPERTY_REQUEST		"$iothub/twin/GET/?$rid="
#define TOPIC_IOTC_CMD_RESPONSE			"$iothub/methods/res/200/?$rid="
#define TOPIC_IOTC_METHOD_REQUEST  		"$iothub/methods/POST/#"
#define TOPIC_IOTC_PROPERTY_DESIRED		"$iothub/twin/PATCH/properties/desired/#"
#define TOPIC_IOTC_PROPERTY_RESPONSE	"$iothub/twin/res/#"
#define PUB_PAYLOAD_IP 					"{\\\"ipAddress\\\":\\\"%s.%s.%s.%s\\\"}"

#define MY_AP       					"iPhone"
#define MY_PASSWORD						"eddymeshack"
#define NTP_SERVER  					"pool.ntp.org"


#endif /* CONSTANTS_H_ */
