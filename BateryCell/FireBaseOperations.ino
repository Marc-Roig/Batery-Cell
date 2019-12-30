#include "main.h"

#include <WiFi.h>
#include <ArduinoJson.h>
#include "FirebaseESP32.h"


void initFirebase() {

    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);

    //Set database read timeout to 1 minute (max 15 minutes)
    Firebase.setReadTimeout(firebaseData, 1000 * 60);
    //tiny, small, medium, large and unlimited.
    //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
    Firebase.setwriteSizeLimit(firebaseData, "small");


}

void test_firebase() {



    String path = "/sequence";
    String out;

    if (Firebase.pathExist(firebaseData, path)) {
        Serial.println("ok");
    }

    if (Firebase.getJSON(firebaseData, path)) {
        Serial.println(firebaseData.jsonData());
    } else {
        Serial.println("REASON: " + firebaseData.errorReason());
    }

    if (Firebase.setInt(firebaseData, path + "content/actions1", 1)) {
        Serial.println("PASSED");
    }
    // String jsonData = "{\"clean\":\"ok\"}";

    // if (Firebase.pushJSON(firebaseData, path, jsonData)) {

    //   Serial.println(firebaseData.dataPath());

    //   Serial.println(firebaseData.pushName());

    //   Serial.println(firebaseData.dataPath() + "/"+ firebaseData.pushName());

    // } else {
    //   Serial.println(firebaseData.errorReason());
    // }

}