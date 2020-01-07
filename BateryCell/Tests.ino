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

void create_clean_sequence() {


    Sequence cleanSeq;

    cleanSeq.add("R", "REVOLVER_ENABLE")
            .add("R", "REVOLVER_ROTATE_TO_NEXT")
            .addFbParam("R", "REVOLVER_ROTATE_ABSOLUTE", "revolver_zero" /*FireBase param name*/)
            .add("R", "FB_ADD_EPPENDORF_FILLED")
            .add("R", "REVOLVER_ROTATE_TO_NEXT")
            .add("R", "FB_ADD_EPPENDORF_FILLED")
            .addDelayFbParam("pump_time_until_full")
            .add("R", "REVOLVER_ROTATE_TO_PREVIOUS")
            .add("R", "REVOLVER_DISABLE")
            .add("V5", "OPEN_VALVE")
            .add("V5", "CLOSE_VALVE")
            .addDelay(100)
            .add("P", "PUMP_SET_SPEED", PUMP_MAX_SPEED)
            .add("P", "PUMP_SET_SPEED", PUMP_STOP);

    storeNewSequence(cleanSeq, "clean_operation");
}

void create_normal_sequence() {


//    Sequence normSeq;
//    normSeq.add("WebSocket", WS_DEBUG_END_OF_OPERATION); // Simulate server ok response for the next element of seq.
//    normSeq.add("WebSocket", WS_START_EXPERIMENT);
//    normSeq.add("WebSocket", WS_DEBUG_END_OF_OPERATION); // Simulate server ok response for the next element of seq.
//    normSeq.add("WebSocket", WS_STOP_EXPERIMENT);
//    normSeq.add("WebSocket", WS_DEBUG_END_OF_OPERATION); // Simulate server ok response for the next element of seq.
//    normSeq.add("WebSocket", WS_START_MEASUREMENTS);
//    normSeq.add("WebSocket", WS_DEBUG_END_OF_OPERATION); // Simulate server ok response for the next element of seq.
//    normSeq.add("WebSocket", WS_STOP_MEASUREMENTS);
//    normSeq.addDelay(1000); // Give time to process the last element of seq. before ending sequence
//    normSeq.add("WebSocket", WS_END_OF_SEQUENCE);
//    storeNewSequence(normSeq, "normal_operation");

}

void testCorrectSequenceName() {
    // correct sequence
    Serial.println("[TEST] Sequence start with valid name");
    const char *start_sequence_msg = "{\"type\":\"sequence_start\",\"from\":\"server\",\"to\":\"microcontroller\",\"content\":{\"sequence\":\"clean_operation\",\"experiment_id\":\"-LxVpoNVlj3UhVthtbVf\"}}";
    size_t length = strlen(start_sequence_msg) + 1;
    create_clean_sequence();
    uint8_t* msg_correct_sequence = charToUint8(start_sequence_msg, length);
    handleWsMessage(msg_correct_sequence, length, 1 /* verbose */);
    delete[] msg_correct_sequence;
}

void testReceiveStartSequence() {

    // Simulate receiving start of sequence request from server
    testCorrectSequenceName();

    // Get message stored on wsQueueStartSequence queue
    Serial.println("[TEST] Get message from freertos queue");
    if (wsQueueStartSequence != 0) {
        WSMessage wsMessage;

        if ( xQueueReceive( wsQueueStartSequence, &(wsMessage), ( TickType_t ) 1000) == pdPASS) {
            Serial.print("--> Message content: ");
            Serial.println(wsMessage.content);
            Serial.print("--> Message type: ");
            Serial.println(wsMessage.type);
            Serial.println("[TEST] Execute sequence");


            // Get sequence from it's name
            Sequence sequence = sequences[wsMessage.content];

            // Set sequence fireBase id to update operations status
            String exp_id = String(wsMessage.type);
            FirebaseOperation::firebaseId = exp_id;

            // Execute sequence
            sequence.executeAll();

        } else {
            Serial.println("[ERROR] Failed to Receive QueueStartSequence message");
        }

    }
}

void test() {

    // -- Test instruments
//    testReceiveStartSequence();


    // -- Upload sequence to firebase
    create_clean_sequence();
    FirebaseOperation::uploadSequence("clean_operation");
//
//    // -- Test sending WS Message
//    Serial.println("[TEST] Sequence WS start experiment");
//    create_normal_sequence();
//    Sequence sequence = sequences["normal_operation"];
//    sequence.executeAll();


}
