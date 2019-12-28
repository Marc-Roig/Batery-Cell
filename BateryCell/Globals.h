#ifndef GLOBALS_H
#define GLOBALS_H

//Define Firebase Data object
FirebaseData firebaseData;

// SEQUENCES
std::map<std::string, Sequence> sequences;

// FREERTOS QUEUES
QueueHandle_t wsSendCommandQueue;
QueueHandle_t wsReceiveCommandQueue;

// WEBSOCKET
std::map<std::string, WSMessage> WS_CommandsToReceiveMap;
std::map<std::string, WSMessage> WS_CommandsToSendMap;

// OPERATIONS QUEUES
QueueHandle_t wsQueueStartExperiment;
QueueHandle_t wsQueueStartSequence;
QueueHandle_t wsQueueAbortSequence;
QueueHandle_t wsQueueExperiment;

// @DONE: ADD SIGNAL FOR NOTIFYING END OF OPERATION (C1 -> C0)
//                                 END OF SEQUENCE  (C1 -> C0) 
// TO SET EVENT
// https://www.freertos.org/xEventGroupSetBits.html
// xEventGroupSetBits(
//                  wsSignalEndOfOperation, // The eventGroup being updated
//                  END_OF_OPERATION_BIT,   // The bits to set
//                  )

// TO WAIT FOR EVENT
//  uxBits = xEventGroupWaitBits(
//            wsSignalEndOfOperation,                   /* xEventGroup     -> The event group being tested.                     */
//            END_OF_OPERATION_BIT,                     /* uxBitsToWaitFor -> The bits within the event group to wait for.      */
//            pdTRUE,                                   /* xClearOnExit    -> BITS should be cleared before returning.          */
//            pdFALSE,                                  /* xWaitForAllBits -> Don't wait for multiple bits, either bit will do. */
//            (TickType_t)(100 / portTICK_PERIOD_MS );  /* xTicksToWait    -> Wait a maximum of 100ms for either bit to be set. */

#define  END_OF_OPERATION_BIT_OK ( 1 << 0 ) // When Operation on the PC has ended correctly, server will send ws message. Then, send signal to Core 1
#define  END_OF_OPERATION_BIT_ERROR ( 1 << 1 ) // When Operation on the PC has ended incorrectly, server will send ws message. Then, send signal to Core 1
#define  END_OF_SEQUENCE_BIT ( 1 << 2 )  // When sequence on core 1 has ended, send signal to core 0 to notify server
EventGroupHandle_t wsSignal; // Event group handler


#endif