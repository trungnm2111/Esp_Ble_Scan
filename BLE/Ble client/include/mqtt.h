#ifndef _MQTT_H_
#define _MQTT_H_

#include <PubSubClient.h>
#include <WiFi.h>
#include "ArduinoJson.h"
#include <string.h>

void Mqtt_Init(void);
String Send_Message(String Address, int Rssi);
// void Mqtt_Listen(void);
bool Mqtt_Check(void);
void Mqtt_Reconnect(void);
// void Mqtt_Send(String message);

#endif