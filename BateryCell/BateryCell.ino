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


void initialize_operations() {

    // Valve
    Sequence::setNewOperation("OPEN_VALVE", ValveOperations::open);
    Sequence::setNewOperation("CLOSE_VALVE", ValveOperations::close);

    // Revolver
    Sequence::setNewOperation("REVOLVER_ENABLE", RevolverOperations::enable);
    Sequence::setNewOperation("REVOLVER_DISABLE", RevolverOperations::disable);
    Sequence::setNewOperation("REVOLVER_ROTATE_TO_NEXT", RevolverOperations::rotateToNext);
    Sequence::setNewOperation("REVOLVER_ROTATE_ABSOLUTE", RevolverOperations::rotateAbsolute);
    Sequence::setNewOperation("REVOLVER_ROTATE_TO_PREVIOUS", RevolverOperations::rotateToPrevious);

    // Pump
    Sequence::setNewOperation("PUMP_SET_SPEED", PumpOperations::setSpeed);

    // Motor Stirrer
    Sequence::setNewOperation("MOTOR_STIRRER_SET_SPEED", MotorStirrerOperations::setSpeed);

    // WebSocket
    Sequence::setNewOperation("WS_START_EXPERIMENT", WebsocketOperation::startExperiment);
    Sequence::setNewOperation("WS_START_MEASUREMENTS", WebsocketOperation::startMeasurements);
    Sequence::setNewOperation("WS_STOP_EXPERIMENT", WebsocketOperation::stopExperiment);
    Sequence::setNewOperation("WS_STOP_MEASUREMENTS", WebsocketOperation::stopMeasurements);
    Sequence::setNewOperation("WS_END_OF_SEQUENCE", WebsocketOperation::endOfSequence);
    Sequence::setNewOperation("WS_DEBUG_END_OF_OPERATION", WebsocketOperationTest::endOfOperation);

    // Delays
    Sequence::setNewOperation("TIME_DELAY_MS", SleepOperation::sleep);
    Sequence::setNewOperation("TIME_DELAY_MINUTES", SleepOperation::sleepMinutes);
}


void setup() {

    // Serial initialization for console debugging
    Serial.begin(115200);
    Serial.println("START...");

    // Name all instruments used
    initialize_instruments();
    initialize_operations();

    // Initialize core 0 main thread (Current is in core 1) and any queue / signal for
    // internal communication between cores
    initCore0();

    // Initialize firebase communication
    FirebaseOperation::initFirebase();

    // Wait time to let everything run
    delay(500);

    #ifdef TEST_FLAG
    test();
    #endif
}


void loop() {

    #ifndef TEST_FLAG
    // Receive the commands from server
    if (wsQueueStartSequence != 0) {
        WSMessage wsMessage;
        if (xQueueReceive(wsQueueStartSequence, &(wsMessage), portMAX_DELAY) == pdPASS) {

            Serial.print("[INFO] Executing sequence: ");
            Serial.println(wsMessage.content);

            // Get sequence from it's name
            Sequence sequence = sequences[wsMessage.content];

            // Set sequence fireBase id to update operations status
            String exp_id = String(wsMessage.type);
            sequence.setFireBaseId(exp_id);

            // Execute sequence
            sequence.executeAll();

        } else {
            Serial.println("[ERROR] Failed to Receive QueueStartSequence message");
        }
    }
    #endif
}
