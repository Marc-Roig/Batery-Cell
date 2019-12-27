#ifndef WEBSOCKET_OPERATIONS_H
#define WEBSOCKET_OPERATIONS_H

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
    static Operation_t stopExperiment;
    static Operation_t startMeasurements;
    static Operation_t stopMeasurements;
    static Operation_t endOfSequence;

    static void _startExperiment(InstrumentVariant& variant, int content);
    static void _stopExperiment(InstrumentVariant& variant, int content);
    static void _startMeasurements(InstrumentVariant& variant, int content);
    static void _stopMeasurements(InstrumentVariant& variant, int content);
    static void _endOfSequence(InstrumentVariant& variant, int content);

    // Build messages to send
    static void sequenceRequestResponse(const char response[30]); // Response from ESP32 to the requested sequence
    static void transmitMessageTo(const char operation[30]); // Transmit message to PC (startExperiment, Measurements, ...)
//    static void endOfSequence(); // Message Server the requested sequence has ended
    static void sequenceAbortResponse(const char response[30]); // Response from ESP32 to the requested sequence abort operation

    // Server to ESP32 Operations //
    static void startSequence(const String &sequence_name); // Start a given sequence
    static void failedOperation(const String &content); // Failed to transmit message to PC
    static void sequenceAbort(const String &content);

    // FireBase
    static void updateFireBaseDocument(const char id[30], int idx, int status); // Update status of firebase document

};



#endif

    
