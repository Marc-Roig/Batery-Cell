#include "main.h"


// WEBSOCKET OPERATIONS

Operation_t WebsocketOperation::startExperiment  { WebsocketOperation::_startExperiment , " Start Experiment"};
Operation_t WebsocketOperation::stopExperiment   { WebsocketOperation::_stopExperiment  , " Stop Experiment"};
Operation_t WebsocketOperation::startMeasurements { WebsocketOperation::_startMeasurements, " Start Measurements "};
Operation_t WebsocketOperation::stopMeasurements { WebsocketOperation::_stopMeasurements, " Stop Measurements"};
Operation_t WebsocketOperation::endOfSequence { WebsocketOperation::_endOfSequence, "End of Sequence"};


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

