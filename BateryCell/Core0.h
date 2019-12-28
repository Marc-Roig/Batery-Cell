#ifndef CORE_0_H

#define CORE_0_H

#define WS_Server_ID "34.67.206.162"
#define WS_Server_Port 3001
#define WS_Server_Route "/?platform=microcontroller"

#include <driver/rmt.h>
#include <esp_task_wdt.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>
#include <driver/timer.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

void initCore0();
void Core0Main(void * pvParameters);

void webSocketInit();
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length);

void initQueues();
void initSignals();

void buildWsExperiment(const char* operation);
void buildWsEndOfSequence();
void handleWsMessage(uint8_t * payload, size_t length, uint8_t verbose=0);


#ifdef TEST_FLAG
#include "Tests.h"
extern WebSocketsClientMock webSocket;
extern WiFiMultiMock WiFi_Multi;
#else
extern WebSocketsClient webSocket;
extern WiFiMulti WiFi_Multi;
#endif

extern TaskHandle_t hTh_Core0_main;

#endif
