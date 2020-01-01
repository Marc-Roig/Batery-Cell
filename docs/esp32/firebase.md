# FIREBASE ESP32

- Dependencies:

    - https://github.com/mobizt/Firebase-ESP32
    - https://github.com/mobizt/HTTPClientESP32Ex

## Get elements

```C++
#include <WiFi.h>
#include <FirebaseESP32.h>

FirebaseData firebaseData;
Firebase.getInt(firebaseData, path + "/int/Data")
if (Firebase.getInt(firebaseData, path + "/Double/Data" + (i + 1))) {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    Serial.println("ETag: " + firebaseData.ETag());
    if (firebaseData.dataType() == "int")
        Serial.println(firebaseData.intData());
}
```

From an array you can:
```C++
FirebaseJsonData jsonData;

arr.get(jsonData, "[1]/food");
```

To get other types:

```C++
    Serial.println(data.intData());
    Serial.println(data.floatData(), 5);
    printf("%.9lf\n", data.doubleData());
    Serial.println(data.boolData() == 1 ? "true" : "false");
    Serial.println(data.stringData());
```

### Print Json

```C++
String jsonStr;
json.toString(jsonStr, true);
Serial.println(jsonStr);
```

### Iterate Json

```C++

void iterateJson(FirebaseData &data) {

    // Allowed dataTypes => int | float | double | boolean | string | json | array
    if (data.dataType == "json") {
        
        // Extract json from data
        FirebaseJson &json = data.jsonObject();

        // To print json
        String jsonStr;
        json.toString(jsonStr, true);
        Serial.println(jsonStr);

        // Iterate elements of json
        size_t len = json.iteratorBegin();
        String key, value = "";
        int type = 0;

        for (size_t i = 0; i < len; i++) {

            json.iteratorGet(i, type, key, value);

            Serial.print(i);
            Serial.print(", ");
            Serial.print("Type: ");
            Serial.print(type == JSON_OBJECT ? "object" : "array");

            if (type == JSON_OBJECT) {
                Serial.print(", Key: ");
                Serial.print(key);
            }
            Serial.print(", Value: ");
            Serial.println(value);

        }

        json.iteratorEnd();

    }
}

```


### Iterate Array

```C++

void iterateArray(FirebaseData &data) {

    // Allowed dataTypes => int | float | double | boolean | string | json | array
    if (data.dataType == "array") {
        
        // Extract array from data
        FirebaseJsonArray &arr = data.jsonArray();
        
        // To print array
        Serial.println("Pretty printed Array:");
        String arrStr;
        arr.toString(arrStr, true);
        Serial.println(arrStr);

        // Iterate elements of array
        for (size_t i = 0; i < arr.size(); i++) {

            Serial.print(i);
            Serial.print(", Value: ");

            // Get the result data from FirebaseJsonArray object
            FirebaseJsonData &jsonData = data.jsonData();
            arr.get(jsonData, i);

            // Check type
            if (jsonData.typeNum == JSON_BOOL)
                Serial.println(jsonData.boolValue ? "true" : "false");
            else if (jsonData.typeNum == JSON_INT)
                Serial.println(jsonData.intValue);
            else if (jsonData.typeNum == JSON_DOUBLE)
                printf("%.9lf\n", jsonData.doubleValue);
            else if (jsonData.typeNum == JSON_STRING ||
                jsonData.typeNum == JSON_NULL ||
                jsonData.typeNum == JSON_OBJECT ||
                jsonData.typeNum == JSON_ARRAY)

            Serial.println(jsonData.stringValue);
        }

    }
}

```

## Set elements

```C++
#include <WiFi.h>
#include <FirebaseESP32.h>

FirebaseData firebaseData;
FirebaseJson json;

// Populate json
json.clear().add("len", 2);
// You can edit the json keys
json.set("len", 3);
```

### Push Json

```C++
if (Firebase.pushJSON(firebaseData, "experiment/sequence", json)) {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println(firebaseData.pushName());
    Serial.println("ETag: " + firebaseData.ETag());
} else {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
}
```

### Update Node

```C++

if (Firebase.updateNode(firebaseData, "experiment/sequence", json)) {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println(firebaseData.pushName());
    Serial.println("ETag: " + firebaseData.ETag());
} else {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
}
```

