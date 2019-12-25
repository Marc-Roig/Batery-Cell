#ifndef WEBSOCKET_OPERATIONS_H
#define WEBSOCKET_OPERATIONS_H

// @TODO: Define WSMessage max_content_lenght max_type_lenght as global
//        And use these variables wherever a strncpy is used
struct WSMessage {

    char content[30] = {0};
    char type[30] = {0};

};


// This class holds all the operations that have
// to be trasmited(received) to(from) the server 
class WebsocketOperation {

public:

    static int messages_map[8];

    // ESP32 to Server Operations //

    static Operation_t startExperiment;
    static Operation_t startMeasurements;
    static Operation_t storeDataFromExperiments;
    static Operation_t storeDataFromMeasurements;

    static void _startExperiment(InstrumentVariant& ws, int content);
    static void _stopExperiment(InstrumentVariant& ws, int content);
    static void _startMeasurements(InstrumentVariant& ws, int content);
    static void _storeDataFromExperiments(InstrumentVariant& ws, int content);
    static void _storeDataFromMeasurements(InstrumentVariant& ws, int content);

    // Build messages to send
    static void sequenceRequestResponse(const char response[30]); // Response from ESP32 to the requested sequence
    static void transmitMessageTo(const char operation[30]); // Transmit message to PC (startExperiment, Measurements, ...)
    static void endOfSequence(); // Message Server the requested sequence has ended
    static void sequenceAbortResponse(const char response[30]); // Response from ESP32 to the requested sequence abort operation

    // Server to ESP32 Operations //
    static void startSequence(const String &sequence_name); // Start a given sequence
    static void failedOperation(const String &content); // Failed to transmit message to PC
    static void sequenceAbort(const String &content);

    // FireBase
    static void updateFireBaseDocument(const char id[30], int idx, int status); // Update status of firebase document

};



#endif

    
