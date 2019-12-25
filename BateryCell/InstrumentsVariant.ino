#include "main.h"

InstrumentVariant::InstrumentVariant(int val) {

    as_integer = val;
    type_ = t_int;

}

InstrumentVariant::InstrumentVariant(Valve valve){

    as_valve = valve;
    type_ = t_valve;

}

InstrumentVariant::InstrumentVariant(Revolver revolver){

    as_revolver = revolver;
    type_ = t_revolver;
}

InstrumentVariant::InstrumentVariant(Pump pump){

    as_pump = pump;
    type_ = t_pump;

}

InstrumentVariant::InstrumentVariant(MotorStirrer motorStirrer){

    as_motor_stirrer = motorStirrer;
    type_ = t_motor_stirrer;

}


InstrumentVariant::InstrumentVariant(WebsocketInstrument webSocket){

    as_websocket = webSocket;
    type_ = t_websocket;

}

InstrumentVariant& InstrumentVariant::operator = (int val) {

    type_ = t_int;
    as_integer = val;

}

InstrumentVariant& InstrumentVariant::operator = (Valve val) {

    type_ = t_valve;
    as_valve = val;

}

InstrumentVariant& InstrumentVariant::operator = (Pump val) {

    type_ = t_pump;
    as_pump = val;

}

InstrumentVariant& InstrumentVariant::operator = (Revolver val) {

    type_ = t_revolver;
    as_revolver = val;

}

InstrumentVariant& InstrumentVariant::operator = (MotorStirrer val) {

    type_ = t_motor_stirrer;
    as_motor_stirrer = val;

}

InstrumentVariant& InstrumentVariant::operator = (WebsocketInstrument val) {

    type_ = t_websocket;
    as_websocket = val;

}



