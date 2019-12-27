#include "main.h"


// WEBSOCKET OPERATIONS

Operation_t WebsocketOperationTest::endOfOperation  { WebsocketOperationTest::_endOfOperation , "Operation ended"};


// Send order to start experiment (and record necessary data)
void WebsocketOperationTest::_endOfOperation(InstrumentVariant &variant, int content){

    // Check if variant is actually holding a Websocket type
    if (!variant.is<WebsocketInstrument>()) throw("INCORRECT VARIANT TYPE");

    Serial.println("[WS Operation Test] Pre sending request confirmation of operation");
    // Set bits on wsSignal to simulate we received comfirmation of successful operation request.
    xEventGroupSetBits( wsSignal, END_OF_OPERATION_BIT_OK);

}