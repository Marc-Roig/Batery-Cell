#include "main.h"

FirebaseData FirebaseOperation::firebaseData;
String FirebaseOperation::workingSequence = "";


void FirebaseOperation::initFirebase() {

    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);

    //Set database read timeout to 1 minute (max 15 minutes)
    Firebase.setReadTimeout(this->firebaseData, 1000 * 60);
    //tiny, small, medium, large and unlimited.
    //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
    Firebase.setwriteSizeLimit(this->firebaseData, "small");

}

void FirebaseOperation::_setOperationAsPending(InstrumentVariant& variant, int operation_idx) {

    // When starting a sequence, set the workingSequence name
    // Each time an operation ends this function is called and specified the operation idx in the sequence
    // to update.

    if (Firebase.pathExist(this->firebaseData, this->workingSequence)) {
        // Get path to update
        String operation_path = this->workingSequence + "/content/actions/";
        operation_path += (char) (operation_idx + 48); // Transform idx to char and append value to the path
        operation_path += "/status";

        // Update status
        Firebase.setInt(firebaseData, operation_path, 0);

    } else {
        Serial.print("[ERROR] FireBase path does not exist");
        Serial.println(this->workingSequence);
    }

}


void FirebaseOperation::_setOperationAsDone(InstrumentVariant& variant, int operation_idx) {

    // When starting a sequence, set the workingSequence name
    // Each time an operation ends this function is called and specified the operation idx in the sequence
    // to update.

    if (Firebase.pathExist(this->firebaseData, this->workingSequence)) {
        // Get path to update
        String operation_path = this->workingSequence + "/content/actions/";
        operation_path += (char) (operation_idx + 48); // Transform idx to char and append value to the path
        operation_path += "/status";

        // Update status
        Firebase.setInt(firebaseData, operation_path, 1);

    } else {
        Serial.print("[ERROR] FireBase path does not exist");
        Serial.println(this->workingSequence);
    }

}


void FirebaseOperation::_setOperationAsFailed(InstrumentVariant& variant, int operation_idx) {

    // When starting a sequence, set the workingSequence name
    // Each time an operation ends this function is called and specified the operation idx in the sequence
    // to update.

    if (Firebase.pathExist(this->firebaseData, this->workingSequence)) {
        // Get path to update
        String operation_path = this->workingSequence + "/content/actions/";
        operation_path += (char) (operation_idx + 48); // Transform idx to char and append value to the path
        operation_path += "/status";

        // Update status
        Firebase.setInt(firebaseData, operation_path, 2);

    } else {
        Serial.print("[ERROR] FireBase path does not exist");
        Serial.println(this->workingSequence);
    }

}
