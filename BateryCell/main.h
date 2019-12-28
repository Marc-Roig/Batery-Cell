#ifndef MAIN_H
#define MAIN_H

// Uncoment if you want to test code (WIFI / Websocket are mocked)
#define TEST_FLAG


#include "Credentials.h"

// STD libraries
#include <vector>
#include <map>
#include <unordered_map>

// Utils
#include <ArduinoJson.h>
#include <FirebaseESP32.h>

// Instruments / Operations definitions
#include "Instruments.h"
#include "WebsocketInstrument.h"

#include "InstrumentsVariant.h"
#include "InstrumentsOperations.h"

#include "WebsocketOperations.h"
#include "WebsocketOperationsTest.h"
#include "WebsocketMessages.h"

// Sequence
#include "Sequence.h"

// Other
#include "HelperDefines.h"

#include "Core0.h"

#include "ExperimentSequences.h"
#include "BateryCellSequences.h"

#include "Globals.h"


#endif