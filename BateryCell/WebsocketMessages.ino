// Where all the raw messages from the websocket are parsed

#include "main.h"

// START SEQUENCE TYPE
void handleWsStartSequenceMessage(const DynamicJsonDocument &doc, int verbose) {

    WSMessage wsMessage;

    char jsonBuffer[300];
    const size_t capacity = JSON_OBJECT_SIZE(8);
    DynamicJsonDocument response_doc(capacity);


    // Get values we need from the message
    const char* sequence_name = doc["content"]["sequence"];
    size_t seq_name_length = strlen(sequence_name) + 1;

    const char* experiment_id = doc["content"]["experiment_id"];
    size_t exp_id_length = strlen(sequence_name) + 1;

    // Copy sequence name and experiment id to the wsMessage variable
    strncpy(wsMessage.content, sequence_name, seq_name_length);
    strncpy(wsMessage.type, experiment_id, exp_id_length);

    // Start building response message to send to server
    response_doc["type"] = "sequence_request_response";
    response_doc["from"] = "microcontroller";
    response_doc["to"] = "server";
    JsonObject content = response_doc.createNestedObject("content");


    // @TODO: CHECK IF FIREBASE EXPERIMENT_ID IS VALID
    // Check if sequence name is valid
    if (sequences.count(sequence_name)) {

        if (wsQueueStartSequence) {
            xQueueSend(wsQueueStartSequence, &wsMessage, portMAX_DELAY);
        }
        else {
            Serial.println("[ERROR] wsQueueStartSequence queue is not initialized");
            throw;
        }

        // @TODO: ACCOUNT IF QUEUE IS FULL
        content["response"] = "ok";

    } else {
        // If given name is not in the list
        // Send error message to the server
        content["response"] = "incorrect_name";
    }

    serializeJson(response_doc, jsonBuffer, 300);
    if (verbose) Serial.println(jsonBuffer);
    webSocket.sendTXT(jsonBuffer);

}

void handleWsTransmitedMessage(const DynamicJsonDocument &doc) {

    String operation_requested = doc["content"]["operation_requested"];
    String status = doc["content"]["status"];

    // If a message to start experiment was send, we are
    // waiting for teh confirmation
    if (operation_requested == "start_experiment" || operation_requested == "start_measurements" ||
        operation_requested == "stop_experiment" || operation_requested == "stop_measurements") {
        // Send signal to Core1
        if      (status == "ok")    xEventGroupSetBits( wsSignal, END_OF_OPERATION_BIT_OK);
        else if (status == "error") xEventGroupSetBits( wsSignal, END_OF_OPERATION_BIT_ERROR);
    }
}