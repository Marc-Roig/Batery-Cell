#include "main.h"

// WEBSOCKET INSTRUMENT

void WebsocketInstrument::startExperiment(int content) {

    WSMessage wsMessage;

    // Copy strings, strncpy requires to pass the string length
    strncpy(wsMessage.content, "start_experiment", 16);
    strncpy(wsMessage.type, "0", 1);

    Serial.println("Start Experiment");
    // Request Core 0 to send WS message of start of experiment
    while ( xQueueSend( wsQueueExperiment, ( void * ) &wsMessage, ( TickType_t ) portMAX_DELAY ) != pdPASS ) {}

    // Wait until the confirmation
    auto uxBits = xEventGroupWaitBits( wsSignal, /* xEventGroup -> The event group being tested.*/
                                  END_OF_OPERATION_BIT_OK | END_OF_OPERATION_BIT_ERROR,
                                  pdTRUE, /* xClearOnExit -> BITS should be cleared before returning. */
                                  pdFALSE, /* xWaitForAllBits -> Don't wait for multiple bits, either bit will do. */
                                  (TickType_t)portMAX_DELAY ); /* xTicksToWait -> Wait a maximum of 100ms for either bit to be set. */

    // Experiment was finalized correctly
    // @TODO: What if experiment has failed??
    if (uxBits & END_OF_OPERATION_BIT_OK) {
        Serial.println("Confirmation Received OK");
    } else {
        Serial.println("Confirmation Received ERROR");
    }

}

void WebsocketInstrument::stopExperiment(int content) {

    WSMessage wsMessage;

    // Copy strings, strncpy requires to pass the string length
    strncpy(wsMessage.content, "stop_experiment", 16);
    strncpy(wsMessage.type, "0", 1);

    // Request Core 0 to send WS message of start of experiment
    while ( xQueueSend( wsQueueExperiment, ( void * ) &wsMessage, ( TickType_t ) portMAX_DELAY ) != pdPASS ) {}

    // Wait until the confirmation
    auto uxBits = xEventGroupWaitBits( wsSignal, /* xEventGroup -> The event group being tested.*/
                                  END_OF_OPERATION_BIT_OK | END_OF_OPERATION_BIT_ERROR,
                                  pdTRUE, /* xClearOnExit -> BITS should be cleared before returning. */
                                  pdFALSE, /* xWaitForAllBits -> Don't wait for multiple bits, either bit will do. */
                                  (TickType_t)portMAX_DELAY ); /* xTicksToWait -> Wait a maximum of 100ms for either bit to be set. */

    // Experiment was finalized correctly
    // @TODO: What if experiment has failed??
    if (uxBits & END_OF_OPERATION_BIT_OK) {

    }

}

// Function in charge to store on the queue the message it has to be sent to
// the server to request start the measurements in the pc
void WebsocketInstrument::startMeasurements(int content) {

    WSMessage wsMessage;

    // Copy strings, strncpy requires to pass the string length
    strncpy(wsMessage.content, "start_measurements", 18);
    strncpy(wsMessage.type, "0", 1);

    // Request Core 0 to send WS message of start measurements
    while ( xQueueSend( wsQueueExperiment, ( void * ) &wsMessage, ( TickType_t ) portMAX_DELAY ) != pdPASS ) {}

    // Wait until the confirmation
    auto uxBits = xEventGroupWaitBits( wsSignal, /* xEventGroup -> The event group being tested.*/
                                  END_OF_OPERATION_BIT_OK | END_OF_OPERATION_BIT_ERROR,
                                  pdTRUE, /* xClearOnExit -> BITS should be cleared before returning. */
                                  pdFALSE, /* xWaitForAllBits -> Don't wait for multiple bits, either bit will do. */
                                  (TickType_t)portMAX_DELAY ); /* xTicksToWait -> Wait a maximum of 100ms for either bit to be set. */

    // Measurements have started
    // @TODO: What if experiment has failed??
    if (uxBits & END_OF_OPERATION_BIT_OK) {

    }

}

void  WebsocketInstrument::stopMeasurements(int content) {

    WSMessage wsMessage;

    // Copy strings, strncpy requires to pass the string length
    strncpy(wsMessage.content, "stop_measurements", 17);
    strncpy(wsMessage.type, "0", 1);

    // Request Core 0 to send WS message of stop measurements
    while ( xQueueSend( wsQueueExperiment, ( void * ) &wsMessage, ( TickType_t ) portMAX_DELAY ) != pdPASS ) {}

    // Wait until the confirmation
    auto uxBits = xEventGroupWaitBits( wsSignal, /* xEventGroup -> The event group being tested.*/
                                  END_OF_OPERATION_BIT_OK | END_OF_OPERATION_BIT_ERROR,
                                  pdTRUE, /* xClearOnExit -> BITS should be cleared before returning. */
                                  pdFALSE, /* xWaitForAllBits -> Don't wait for multiple bits, either bit will do. */
                                  (TickType_t)portMAX_DELAY ); /* xTicksToWait -> Wait a maximum of 100ms for either bit to be set. */

    // Measurements have stopped
    // @TODO: What if experiment has failed??
    if (uxBits & END_OF_OPERATION_BIT_OK) {

    }

}

void WebsocketInstrument::endOfSequence(int content) {

    // Set signal to notify sequence has ended
    // Core 0 will send a ws message to notify the server
    xEventGroupSetBits( wsSignal, END_OF_SEQUENCE_BIT);



}
