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

    Serial.println("Confirmation Received");
    // Experiment was finalized correctly
    // @TODO: What if experiment has failed??
    if (uxBits & END_OF_OPERATION_BIT_OK) {
        Serial.print("Confirmation OK");
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


void WebsocketInstrument::startMeasurements(int content) {



}

void WebsocketInstrument::storeDataFromExperiments(int content) {



}

void WebsocketInstrument::storeDataFromMeasurements(int content) {



}

void WebsocketInstrument::endOfSequence(int content) {



}
