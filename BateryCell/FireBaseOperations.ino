#include "main.h"


FirebaseData FirebaseOperation::firebaseData;
String FirebaseOperation::firebaseId = "";

void FirebaseOperation::initFirebase() {

    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);

    //Set database read timeout to 1 minute (max 15 minutes)
    Firebase.setReadTimeout(FirebaseOperation::firebaseData, 1000 * 60);
    //tiny, small, medium, large and unlimited.
    //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
    Firebase.setwriteSizeLimit(FirebaseOperation::firebaseData, "small");

}


void FirebaseOperation::setOperationAsPending(int operation_idx) {

    // When starting a sequence, set the workingSequence name
    // Each time an operation ends this function is called and specified the operation idx in the sequence
    // to update.

    String operation_path = "/" + FirebaseOperation::firebaseId;

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
        Serial.println(FirebaseOperation::firebaseId);
    }

}


void FirebaseOperation::setOperationAsDone(int operation_idx) {

    // When starting a sequence, set the workingSequence name
    // Each time an operation ends this function is called and specified the operation idx in the sequence
    // to update.

    String operation_path = "/" + FirebaseOperation::firebaseId;

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
        Serial.println(FirebaseOperation::firebaseId);
    }

}


void FirebaseOperation::setOperationAsFailed(int operation_idx) {

    // When starting a sequence, set the workingSequence name
    // Each time an operation ends this function is called and specified the operation idx in the sequence
    // to update.

    String operation_path = "/" + FirebaseOperation::firebaseId;

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
        Serial.println(FirebaseOperation::firebaseId);
    }

}

void FirebaseOperation::setTimestampStart(int operation_idx) {

    String operation_path = "/" + FirebaseOperation::firebaseId;

    if (Firebase.pathExist(FirebaseOperation::firebaseData, operation_path)) {

        operation_path += "/content/actions/";
        operation_path += String(operation_idx); // Transform idx to char and append value to the path

        // Update start timestamp
        Firebase.setTimestamp(firebaseData, operation_path + "/startTime");

    } else {
        Serial.print("[ERROR] FireBase path does not exist: ");
        Serial.println(FirebaseOperation::firebaseId);
    }

}


int FirebaseOperation::getParamByName(String param_name) {

    String parameter_path = "/" + FirebaseOperation::firebaseId;
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


void FirebaseOperation::uploadSequence(const char *seq_name) {

    FirebaseJson json;
    FirebaseJson esp32_param_json;

    FirebaseJsonArray arr;

    if (sequences.count(seq_name)) {

        Sequence seq = sequences[seq_name];

        String firebasePath = String("sequences/") + seq_name;

        Serial.println("[INFO] Uploading sequence to firebase: ");

        // Iterate over sequence to get the operation names
        for (int i = 0; i < seq.size(); i++) {

            FirebaseJson operation_json;

            // Instrument
            operation_json.add("instrument", seq.names_list[i]);

            // Operation
            operation_json.add("name", seq.operations_list[i]);

            // FireBase param
            if (seq.fb_param_list[i] != nullptr) {
                operation_json.add("fb_param", seq.fb_param_list[i]);
                esp32_param_json.add(seq.fb_param_list[i] /*parameter name*/, 0 /*default value*/);
            }

            arr.add(operation_json);

        }

        json.add("operations", arr);

        String jsonStr;
        json.toString(jsonStr, true);
        Serial.println(jsonStr);

        Firebase.updateNode(firebaseData, firebasePath, json);
        if (!Firebase.updateNode(firebaseData, firebasePath + "/parameters/microcontroller", esp32_param_json)) {
            Serial.println("[ERROR] Can not create sequence parameters");
        }

    } else {
        Serial.print("[ERROR] Could not upload sequence to firebase. Seq name does not exist: ");
        Serial.println(seq_name);
    }

}


// Instruments
void FirebaseOperation::updateRevolverSlot(int currentSlot) {

    String firebasePath = "hardware/content/eppendorf_idx";
    if (!Firebase.setInt(firebaseData, firebasePath, currentSlot))
        Serial.println("[ERROR] Could not sed eppendorf idx in firebase hardware document");

}


void FirebaseOperation::pushEppendorfFilled(int currentSlot) {

    String parameter_path = "/" + FirebaseOperation::firebaseId;
    parameter_path += "/content";

    // If eppendorfs array was previously declared, push value into it
    if (Firebase.get(firebaseData, parameter_path + "/eppendorfs_filled")) {
        if (firebaseData.dataType() == "array") {

            FirebaseJsonArray &arr = firebaseData.jsonArray();
            arr.add(currentSlot);

            if (!Firebase.setArray(firebaseData, parameter_path + "/eppendorfs_filled", arr)) {
                Serial.println("[ERROR] Could not set filled eppendorf");
                Serial.println("REASON: " + firebaseData.errorReason());
            }

            return;
        }
    }

    // If it was not, create a new array
    Serial.println("b");
    FirebaseJsonArray arr;
    arr.add(currentSlot);
    if (!Firebase.setArray(firebaseData, parameter_path + "/eppendorfs_filled", arr)) {
        Serial.println("[ERROR] Could not set filled eppendorf");
        Serial.println("REASON: " + firebaseData.errorReason());
    }


}