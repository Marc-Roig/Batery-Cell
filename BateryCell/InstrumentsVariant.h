#ifndef INSTRUMENTSVARIANT_H
#define INSTRUMENTSVARIANT_H

enum instrumentVariant_t {t_int, t_valve, t_revolver, t_pump, t_motor_stirrer, t_websocket};

// Heterogeneous container to hold different types
class InstrumentVariant {

public:
    instrumentVariant_t type_;

    union {

        int as_integer = 0;
        Valve as_valve;
        Revolver as_revolver;
        Pump as_pump;
        MotorStirrer as_motor_stirrer;
        WebsocketInstrument as_websocket;

    };

    InstrumentVariant() {};
    InstrumentVariant(int val);
    InstrumentVariant(Valve valve);
    InstrumentVariant(Revolver revolver);
    InstrumentVariant(Pump pump);
    InstrumentVariant(MotorStirrer motorStirrer);
    InstrumentVariant(WebsocketInstrument webSocket);

    template<class T> 
    bool is() {

        if (std::is_same<T, int>() && type_ == t_int) return true;
        else if (std::is_same<T, Valve>() && type_ == t_valve) return true;
        else if (std::is_same<T, Pump>() && type_ == t_pump) return true;
        else if (std::is_same<T, Revolver>() && type_ == t_revolver) return true;
        else if (std::is_same<T, MotorStirrer>() && type_ == t_motor_stirrer) return true;
        else if (std::is_same<T, WebsocketInstrument>() && type_ == t_websocket) return true;

        return false;

    }

    InstrumentVariant& operator = (int val);
    InstrumentVariant& operator = (Valve val);
    InstrumentVariant& operator = (Pump val);
    InstrumentVariant& operator = (Revolver val);
    InstrumentVariant& operator = (MotorStirrer val);
    InstrumentVariant& operator = (WebsocketInstrument val);
    
};

#endif