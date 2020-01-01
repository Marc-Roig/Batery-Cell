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

void setup() {

    // Serial initialization for console debugging
    Serial.begin(115200);
    Serial.println("START...");

    // Name all instruments used
    initialize_instruments();

    // Initialize core 0 main thread (Current is in core 1) and any queue / signal for
    // internal communication between cores
    initCore0();

    // Initialize firebase communication
    FirebaseOperation::initFirebase();

    // Wait time to let everything run
    delay(500);

    test();

}


void loop() {

//    // Receive the commands from server
//    if (wsQueueStartSequence != 0) {
//        WSMessage wsMessage;
//        if (xQueueReceive(wsQueueStartSequence, &(wsMessage), portMAX_DELAY) == pdPASS) {
//
//            Serial.print("[INFO] Executing sequence: ");
//            Serial.println(wsMessage.content);
//
//            // Get sequence from it's name
//            Sequence sequence = sequences[wsMessage.content];
//
//            // Set sequence fireBase id to update operations status
//            String exp_id = String(wsMessage.type);
//            sequence.setFireBaseId(exp_id);
//
//            // Execute sequence
//            sequence.executeAll();
//
//        } else {
//            Serial.println("[ERROR] Failed to Receive QueueStartSequence message");
//        }
//    }

}
