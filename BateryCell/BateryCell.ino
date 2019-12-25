#include "main.h"

void initialize_instruments() {
    Sequence::setNewInstrument("MSc", MotorStirrer(5, 0));
    Sequence::setNewInstrument("MSa", MotorStirrer(19, 0));
    Sequence::setNewInstrument("V1", Valve(32, HIGH));
    Sequence::setNewInstrument("V2", Valve(33, HIGH));
    Sequence::setNewInstrument("V3", Valve(25, HIGH));
    Sequence::setNewInstrument("V4", Valve(26, HIGH));
    Sequence::setNewInstrument("V5", Valve(27, HIGH));
    Sequence::setNewInstrument("V6", Valve(14, HIGH));
    Sequence::setNewInstrument("P", Pump(12, -1));
    Sequence::setNewInstrument("R", Revolver(/*Step pin*/ 18, /*Dir pin*/4, /*Enable pin*/2, /*N slots*/30, /*microsteps*/32));
    Sequence::setNewInstrument("WebSocket", WebsocketInstrument());
    Sequence::setNewInstrument("Sleep", -1);
    Sequence::setNewInstrument("SleepMinutes", -1);
}

void create_clean_sequence() {


    Sequence cleanSeq;
    cleanSeq.add("R", REVOLVER_ENABLE);
//    cleanSeq.add("R", REVOLVER_ROTATE_TO_NEXT);
    cleanSeq.add("R", REVOLVER_DISABLE);

    storeNewSequence(cleanSeq, "clean_operation");
}

void create_normal_sequence() {


    Sequence normSeq;
    normSeq.add("WebSocket", WS_START_EXPERIMENT);

    storeNewSequence(normSeq, "normal_operation");

}


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
    const char* start_sequence_msg = "{\"type\":\"sequence_start\",\"from\":\"server\",\"to\":\"microcontroller\",\"content\":{\"sequence\":\"clean_operation\",\"experiment_id\":\"aSEFasdWEs\"}}";
    create_clean_sequence();
    uint8_t* msg_correct_sequence = charToUint8(start_sequence_msg, length);
    handleWsMessage(msg_correct_sequence, length, 1 /* verbose */);
    delete[] msg_correct_sequence;
}

void test() {
    // Init FREERtos queues
//    initQueues();


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

    // -- Test sending WS Message
    Serial.println("[TEST] Sequence WS start experiment");
    create_normal_sequence();
    Sequence sequence = sequences["normal_operation"];

    // Simulate server sending the signal of start_experiment ok
    // TODO: handleWsMessage(msg_start_experiment_ok, length, 1);
    sequence.executeAll();
    // Start thread?
}

void setup() {

    Serial.begin(115200);
    Serial.println("START...");

    initialize_instruments();
    initCore0();

    delay(1000);

    test();


//    // Sequence initSeq;
//    // initSeq.add("R", GET_POSITION_FROM_DATABASE);


//    Sequence testSeq;
//    testSeq.add("R", REVOLVER_ENABLE);
//    testSeq.add("R", REVOLVER_ROTATE_ABSOLUTE, 29);
//    // testSeq.addDelay(1000);
//    testSeq.add("R", REVOLVER_ROTATE_ABSOLUTE, 0);
//    testSeq.add("R", REVOLVER_DISABLE);
//
//    // testSeq.add("WebSocket", WS_START_EXPERIMENT);
//    // testSeq.add("WebSocket", WS_START_MEASUREMENTS);
//    // testSeq.add("WebSocket", WS_STORE_DATA_FROM_EXPERIMENTS);
//    // testSeq.add("WebSocket", WS_STORE_DATA_FROM_MEASUREMENTS);
//
//    testSeq.executeAll();

}

//static std::map<String, void (*callback)()> commands_map;

void loop() {

    // Receive the commands from server
//    if (wsQueueStartSequence != 0) {
//        WSMessage wsMessage;
//        if (xQueueReceive(wsQueueStartSequence, &(wsMessage), portMAX_DELAY) == pdPASS) {
//            Serial.print("[INFO] Executing sequence: ");
//            Serial.println(wsMessage.content);
//            Sequence sequence = sequences[wsMessage.content];
//            sequence.executeAll();
//
//        } else {
//            Serial.println("[ERROR] Failed to Receive QueueStartSequence message");
//        }
//    }
}
