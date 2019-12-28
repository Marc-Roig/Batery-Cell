#ifndef WEBSOCKET_INSTRUMENT_H
#define WEBSOCKET_INSTRUMENT_H

class WebsocketInstrument {

public:

    WebsocketInstrument() = default;

    void startExperiment(int content);
    void stopExperiment(int content);
    void startMeasurements(int content);
    void stopMeasurements(int content);
    void endOfSequence(int content);

};

#endif