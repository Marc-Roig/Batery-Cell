#include "main.h"

// WEBSOCKET OPERATIONS
Operation_t WebsocketOperation::startExperiment   { WebsocketOperation::_startExperiment  , "WS Start Experiment"  };
Operation_t WebsocketOperation::stopExperiment    { WebsocketOperation::_stopExperiment   , "WS Stop Experiment"   };
Operation_t WebsocketOperation::startMeasurements { WebsocketOperation::_startMeasurements, "WS Start Measurements"};
Operation_t WebsocketOperation::stopMeasurements  { WebsocketOperation::_stopMeasurements , "WS Stop Measurements" };
Operation_t WebsocketOperation::endOfSequence     { WebsocketOperation::_endOfSequence    , "WS End of Sequence"   };


// Send order to start experiment (and record necessary data)
void WebsocketOperation::_startExperiment(InstrumentVariant& variant, int content) {

    // Check if variant is actually holding a Websocket type
    if (!variant.is<WebsocketInstrument>()) throw("INCORRECT VARIANT TYPE");

    // Transform variant into a WebsocketInstrument variable
    WebsocketInstrument ws = variant.as_websocket;

    // Start experiment
    ws.startExperiment(content);
}

// Send order to stop the experiment (and store data during the experiment)
void WebsocketOperation::_stopExperiment(InstrumentVariant& variant, int content) {

    // Check if variant is actually holding a Websocket type
    if (!variant.is<WebsocketInstrument>()) throw("INCORRECT VARIANT TYPE");

    // Transform variant into a WebsocketInstrument variable
    WebsocketInstrument ws = variant.as_websocket;

    // Stop experiment
    ws.stopExperiment(content);

}


void WebsocketOperation::_startMeasurements(InstrumentVariant& variant, int content) {

    // Check if variant is actually holding a Websocket type
    if (!variant.is<WebsocketInstrument>()) throw("INCORRECT VARIANT TYPE");

    // Transform variant into a WebsocketInstrument variable
    WebsocketInstrument ws = variant.as_websocket;

    // Start measurements
    ws.startMeasurements(content);

}

void WebsocketOperation::_stopMeasurements(InstrumentVariant& variant, int content) {
    
    // Check if variant is actually holding a Websocket type
    if (!variant.is<WebsocketInstrument>()) throw("INCORRECT VARIANT TYPE");

    // Transform variant into a WebsocketInstrument variable
    WebsocketInstrument ws = variant.as_websocket;

    // Stop measurements
    ws.stopMeasurements(content);

}

void WebsocketOperation::_endOfSequence(InstrumentVariant& variant, int content) {

    // Check if variant is actually holding a Websocket type
    if (!variant.is<WebsocketInstrument>()) throw("INCORRECT VARIANT TYPE");

    // Transform variant into a Valve variable
    WebsocketInstrument ws = variant.as_websocket;

    // Finish sequence
    ws.endOfSequence(content);

}

