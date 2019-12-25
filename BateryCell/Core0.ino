#include "main.h"

#ifdef TEST_FLAG
WebSocketsClientMock webSocket;
WiFiMultiMock WiFi_Multi;
#else
WebSocketsClient webSocket;
WiFiMulti WiFi_Multi;
#endif

TaskHandle_t hTh_Core0_main;

bool WS_online = false;

//const char ssid[] = "TP-LINK_E7F758";
//const char pass[] = "55302760";

// const char ssid[] = "TP-Link_5C96";
// const char pass[] = "87716443";

const char ssid[] = "MIWIFI_2G_Jr3p";
const char pass[] = "svXjAHe6";

void initCore0() {

    WiFi_Multi.addAP(ssid, pass);
    while (WiFi_Multi.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
        delay(250);
        Serial.print('.');
    }

    Serial.println('\n');
    Serial.print("Connected to ");
    Serial.println(WiFi.SSID());              // Tell us what network we're connected to
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer
    
    initQueues();
    initSignals();

    xTaskCreatePinnedToCore(
                Core0Main,     /* Function to implement the task */
                "Core0_main",   /* Name of the task               */
                50000,          /* Stack size in words            */
                NULL,           /* Task input parameter           */
                5,              /* Priority of the task           */
                &hTh_Core0_main,/* Task handle.                   */
                0);             /* Core where the task should run */

    configASSERT( hTh_Core0_main );
    if (hTh_Core0_main == NULL) vTaskDelete(hTh_Core0_main);
    
}

void Core0Main(void * pvParameters) {

    webSocketInit();

    Serial.println("Starting websocket");
    

    while ( true ) {

         webSocket.loop();

        // Only if Websocket is online
        if (WS_online) {
            // Send Ws message to server to start an experiment
            // when core 1 asks for it through this queue
            if (wsQueueExperiment != 0) {

                WSMessage wsMessage;

                if ( xQueueReceive( wsQueueExperiment, &(wsMessage), ( TickType_t ) 5) == pdPASS) {
                    Serial.println("Received");
                    buildWsExperiment(wsMessage.content);
                }

            }

            // Check if a sequence has ended
            auto uxBits = xEventGroupWaitBits( wsSignal, END_OF_SEQUENCE_BIT, pdTRUE, pdFALSE, (TickType_t)1 );
            if (uxBits & END_OF_SEQUENCE_BIT) buildWsEndOfSequence();


        }

        // Watchdog
        TIMERG0.wdt_wprotect = TIMG_WDT_WKEY_VALUE;
        TIMERG0.wdt_feed     = 1;
        TIMERG0.wdt_wprotect = 0;
       
    }

}

void webSocketInit() {

    // server address, port and URL
    webSocket.begin(WS_Server_ID, WS_Server_Port, WS_Server_Route);

    // event handler
    webSocket.onEvent(webSocketEvent);

    // use HTTP Basic Authorization this is optional remove if not needed
    //webSocket.setAuthorization("user", "Password");

    // try to reconnect every 2s if connection fails
    webSocket.setReconnectInterval(2000);

}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {

    switch (type) {
        case WStype_DISCONNECTED:
            WS_online = false;
            Serial.println("Disconnected");
            break;

        case WStype_CONNECTED:
            WS_online = true;
            Serial.println("Connected");
            break;

        case WStype_TEXT:
            // Parse message received
            handleWsMessage(payload, length);
            Serial.printf("%s\n", payload);
            break;

        case WStype_BIN:
            break;

        case WStype_ERROR:
            break;

        default:
            break;

    }

}

void initQueues() {

    wsQueueStartExperiment = xQueueCreate( 20, sizeof ( const WSMessage ) );

    if (wsQueueStartExperiment == NULL) {
        Serial.println("Error creating wsQueueStartExperiment");
        ESP.restart();
    }

    wsQueueStartSequence = xQueueCreate( 20, sizeof ( const WSMessage ) );

    if (wsQueueStartSequence == NULL) {
        Serial.println("Error creating wsQueueStartSequence");
        ESP.restart();
    }

    wsQueueAbortSequence = xQueueCreate( 20, sizeof ( const WSMessage ) );

    if (wsQueueAbortSequence == NULL) {
        Serial.println("Error creating wsQueueAbortSequence");
        ESP.restart();
    }

    wsQueueExperiment = xQueueCreate( 20, sizeof ( const WSMessage ) );

    if (wsQueueExperiment == NULL) {
        Serial.println("Error creating wsQueueExperiment");
        ESP.restart();
    }

}

void initSignals() {

    wsSignal = xEventGroupCreate();

    if (wsSignal == NULL) {
        Serial.println("Error creating wsSignal");
        ESP.restart();
    }

}

void buildWsExperiment(const char* operation) {
    char jsonBuffer[300];
    const size_t capacity = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4);
    DynamicJsonDocument doc(capacity);

    doc["type"] = "transmit_message_to";
    doc["from"] = "microcontroller";
    doc["to"] = "pc";

    JsonObject content = doc.createNestedObject("content");
    content["retry"] = true;
    content["operation_requested"] = operation;

    serializeJson(doc, jsonBuffer, 300);
    webSocket.sendTXT(jsonBuffer);
}

void buildWsEndOfSequence() {

    char jsonBuffer[300];
    const size_t capacity = JSON_OBJECT_SIZE(0) + JSON_OBJECT_SIZE(4);
    DynamicJsonDocument doc(capacity);

    doc["type"] = "end_of_sequence";
    doc["from"] = "microcontroller";
    doc["to"] = "server";
    JsonObject content = doc.createNestedObject("content");

    serializeJson(doc, jsonBuffer, 300);
    webSocket.sendTXT(jsonBuffer);

}

void handleWsMessage(uint8_t * payload, size_t length, uint8_t verbose /*=0*/) {

    // Parse json from payload
    const size_t capacity = JSON_OBJECT_SIZE(10)+500;
    DynamicJsonDocument doc(capacity);

    deserializeJson(doc, payload, length);

    // Check which command has been received
//    const char* type = doc["type"];
    String type = doc["type"];

    // Call callback function associated with this command
    // @TODO: FIND OR CREATE LIST OF WEBSOCKET CALLBACKS
    if (type == "sequence_start") handleWsStartSequenceMessage(doc, verbose);
    // Sequence start
    // failed_operation (not yet)
}