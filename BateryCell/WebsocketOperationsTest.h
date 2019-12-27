#ifndef WEBSOCKET_OPERATIONS_TEST
#define WEBSOCKET_OPERATIONS_TEST


// This class holds all the operations that have
// to be trasmited(received) to(from) the server
class WebsocketOperationTest {

public:

    static int messages_map[8];

    // ESP32 to Server Operations //

    static Operation_t endOfOperation;

    static void _endOfOperation(InstrumentVariant& variant, int content);

};


#endif