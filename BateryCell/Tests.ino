#include "main.h"

// -- WEBSOCKET MOCK -- //
void WebSocketsClientMock::begin(String _server_id, int _server_port, String _server_route) {
    server_id = _server_id;
    server_port = _server_port;
    server_route = _server_route;
}


void WebSocketsClientMock::onEvent(void (*_websocketEvent)(WStype_t, uint8_t *, size_t)) {
    websocketEvent = _websocketEvent;

    // Leave connection open
    websocketEvent(WStype_CONNECTED, nullptr, 0);
}


void WebSocketsClientMock::setReconnectInterval(int) {}


void WebSocketsClientMock::sendTXT(char *msg) {
    Serial.print("[WS Mock] Sending websocket message: ");
    Serial.println(msg);
}


void WebSocketsClientMock::sendTXT(const char *msg) {
    Serial.print("[WS Mock] Sending websocket message: ");
    Serial.println(msg);
}


void WebSocketsClientMock::sendTXT(String msg) {
    Serial.print("[WS Mock] Sending websocket message: ");
    Serial.println(msg);
}


void WebSocketsClientMock::loop() {

}

// -- TEST FUNCTION -- //


uint8_t* charToUint8(const char* msg, size_t length) {

    const char* beg = msg;
    const char* end = msg + length;

    uint8_t* msg2 = new uint8_t[length];

    size_t i = 0;
    for (; beg != end; ++beg, ++i)
    {
        msg2[i] = (uint8_t)(*beg);
    }

    return msg2;
}

void testWS() {
    // Incorrect sequence
    Serial.println("[TEST] Sequence start with invalid name");
    const char* start_sequence_msg = "{\"type\":\"sequence_start\",\"from\":\"server\",\"to\":\"microcontroller\",\"content\":{\"sequence\":\"clean_operation\",\"experiment_id\":\"aSEFasdWEs\"}}";
    size_t length = strlen(start_sequence_msg) + 1;
    uint8_t* msg_incorrect_sequence = charToUint8(start_sequence_msg, length);
    handleWsMessage(msg_incorrect_sequence, length, 1 /* verbose */);
    delete[] msg_incorrect_sequence;

}

void testCorrectSequenceName() {
    // correct sequence
    Serial.println("[TEST] Sequence start with valid name");
    const char *start_sequence_msg = "{\"type\":\"sequence_start\",\"from\":\"server\",\"to\":\"microcontroller\",\"content\":{\"sequence\":\"clean_operation\",\"experiment_id\":\"aSEFasdWEs\"}}";
    size_t length = strlen(start_sequence_msg) + 1;
    create_clean_sequence();
    uint8_t* msg_correct_sequence = charToUint8(start_sequence_msg, length);
    handleWsMessage(msg_correct_sequence, length, 1 /* verbose */);
    delete[] msg_correct_sequence;
}

void testReceiveStartSequence() {

    testCorrectSequenceName();

    // ---- Receive message
    Serial.println("[TEST] Get message from freertos queue");
    if (wsQueueStartSequence != 0) {
        WSMessage wsMessage;
        if ( xQueueReceive( wsQueueStartSequence, &(wsMessage), ( TickType_t ) 1000) == pdPASS) {
            Serial.print("--> Message content: ");
            Serial.println(wsMessage.content);
            Serial.print("--> Message type: ");
            Serial.println(wsMessage.type);
            Serial.println("[TEST] Execute sequence");
            Sequence sequence = sequences[wsMessage.content];
            sequence.executeAll();

        } else {
            Serial.println("[ERROR] Failed to Receive QueueStartSequence message");
        }

    }
}

void test() {

    testReceiveStartSequence();

    // -- Test sending WS Message
    Serial.println("[TEST] Sequence WS start experiment");
    create_normal_sequence();
    Sequence sequence = sequences["normal_operation"];

    sequence.executeAll();


}
