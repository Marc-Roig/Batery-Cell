#include "main.h"


FirebaseData FirebaseOperation::firebaseData;

void FirebaseOperation::initFirebase() {

    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);

    //Set database read timeout to 1 minute (max 15 minutes)
    Firebase.setReadTimeout(FirebaseOperation::firebaseData, 1000 * 60);
    //tiny, small, medium, large and unlimited.
    //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
    Firebase.setwriteSizeLimit(FirebaseOperation::firebaseData, "small");

}


void FirebaseOperation::setOperationAsPending(String firebase_id, int operation_idx) {

    // When starting a sequence, set the workingSequence name
    // Each time an operation ends this function is called and specified the operation idx in the sequence
    // to update.

    String operation_path = "/" + firebase_id;

    if (Firebase.pathExist(FirebaseOperation::firebaseData, operation_path)) {
        // Get path to update
        operation_path += "/content/actions/";
        operation_path += String(operation_idx); // Transform idx to char and append value to the path

        // Update end timestamp
        Firebase.setTimestamp(firebaseData, operation_path + "/endTime");

        // Update status
        Firebase.setInt(firebaseData, operation_path + "/status", 0);

    } else {
        Serial.print("[ERROR] FireBase path does not exist: ");
        Serial.println(firebase_id);
    }

}


void FirebaseOperation::setOperationAsDone(String firebase_id, int operation_idx) {

    // When starting a sequence, set the workingSequence name
    // Each time an operation ends this function is called and specified the operation idx in the sequence
    // to update.

    String operation_path = "/" + firebase_id;

    if (Firebase.pathExist(FirebaseOperation::firebaseData, operation_path)) {
        // Get path to update
        operation_path += "/content/actions/";
        operation_path += String(operation_idx); // Transform idx to char and append value to the path

        // Update end timestamp
        Firebase.setTimestamp(firebaseData, operation_path + "/endTime");

        // Update status
        Firebase.setInt(firebaseData, operation_path + "/status", 1);

    } else {
        Serial.print("[ERROR] FireBase path does not exist: ");
        Serial.println(firebase_id);
    }

}


void FirebaseOperation::setOperationAsFailed(String firebase_id, int operation_idx) {

    // When starting a sequence, set the workingSequence name
    // Each time an operation ends this function is called and specified the operation idx in the sequence
    // to update.

    String operation_path = "/" + firebase_id;

    if (Firebase.pathExist(FirebaseOperation::firebaseData, operation_path)) {
        // Get path to update
        operation_path += "/content/actions/";
        operation_path += String(operation_idx); // Transform idx to char and append value to the path

        // Update end timestamp
        Firebase.setTimestamp(firebaseData, operation_path + "/endTime");

        // Update status
        Firebase.setInt(firebaseData, operation_path + "/status", 2);

    } else {
        Serial.print("[ERROR] FireBase path does not exist: ");
        Serial.println(firebase_id);
    }

}

void FirebaseOperation::setTimestampStart(String firebase_id, int operation_idx) {

    String operation_path = "/" + firebase_id;

    if (Firebase.pathExist(FirebaseOperation::firebaseData, operation_path)) {

        operation_path += "/content/actions/";
        operation_path += String(operation_idx); // Transform idx to char and append value to the path

        // Update start timestamp
        Firebase.setTimestamp(firebaseData, operation_path + "/startTime");

    } else {
        Serial.print("[ERROR] FireBase path does not exist: ");
        Serial.println(firebase_id);
    }

}


int FirebaseOperation::getParamByName(String firebase_id, String param_name) {

    String parameter_path = "/" + firebase_id;
    parameter_path += "/content/parameters/";

    // Default to 0
    int parameter = 0;

    if (Firebase.getInt(firebaseData, parameter_path + param_name)) {

        if (firebaseData.dataType() == "int") {
            parameter = firebaseData.intData();
        } else {
            Serial.println("[ERROR] {Firebase} Operation parameter parameter invalid data type");
        }
    } else {
        Serial.print("[ERROR] {FireBase} Operation parameter does not exist: ");
        Serial.println(param_name);
    }

    return parameter;

}


void FirebaseOperation::uploadSequence(const Sequence& seq, const char *seq_name) {

    FirebaseJson json;
    FirebaseJsonArray arr;

    String firebasePath = String("sequences/") + seq_name;

    Serial.println("[INFO] Uploading sequence to firebase: ");

    // Iterate over sequence to get the operation names
    for (int i = 0; i < seq.size(); i++) {

        // Name of the instrument: [R]
        String operation_name = String("[") + seq.names_list[i] + "]";
        // Operation
        operation_name += String(" ") + seq.operations_list[i];
        // If operation ends with X, replace it with the operation value (delay X -> delay 2)
        if (operation_name.endsWith("X")) {
            // Remove X
            operation_name.remove(operation_name.length() - 1, 1);
            operation_name.concat(seq.msgs_list[i]);
        }
        arr.add(operation_name);

    }

    json.add("operations", arr);

    String jsonStr;
    json.toString(jsonStr, true);
    Serial.println(jsonStr);

    Firebase.updateNode(firebaseData, firebasePath, json);

}


