#ifndef INSTRUMENTSOPERATIONS_H
#define INSTRUMENTSOPERATIONS_H

// Here all the functions to update the instruments
// status are created. Where they transform the variant
// variable into the appropiate class.

// ----------------- //
// -- PUMP SPEEDS -- //
// ----------------- //
#define PUMP_MAX_SPEED 255
#define PUMP_HIGH_SPEED 128
#define PUMP_MID_SPEED 85
#define PUMP_LOW_SPEED 55
#define PUMP_STOP 0

// ----------------------------- //
// -- MAGNETIC STIRRER SPEEDS -- //
// ----------------------------- //
#define MAGNETIC_STIRRER_MAX_SPEED 255
#define MAGNETIC_STIRRER_HIGH_SPEED 128
#define MAGNETIC_STIRRER_MID_SPEED 85
#define MAGNETIC_STIRRER_LOW_SPEED 60
#define MAGNETIC_STIRRER_STOP 0


// -------------------- //
// -- OPERATION TYPE -- //
// -------------------- //

// It holds the callback function to execute the operation
// and the message associated to it. Used to list the elements
// of a Sequence variable.

// The callback functions recieves a Variant argument that
// presumably holds the correct Instrument Type, and an
// integer with the message of the operation.

//typedef std::pair<void (*)(Variant,int), const char*> Operation_t;
using Operation_t = std::pair<void (*)(InstrumentVariant&,int), const char*>;


class ValveOperations {

public:

    static Operation_t open;
    static Operation_t close;

    // Transform variant into Valve type and open valve
    static void _open(InstrumentVariant& _valve, int msg);

    // Transform variant into Valve type and close valve
    static void _close(InstrumentVariant& _valve, int msg);

};


class RevolverOperations {

public:

    static Operation_t enable;
    static Operation_t disable;
    static Operation_t rotateAbsolute;
    static Operation_t rotateToNext;
    static Operation_t rotateToPrevious;
    static Operation_t fbAddFilledEppendorf;

    static void _enable(InstrumentVariant& _revolver, int msg);
    static void _disable(InstrumentVariant& _revolver, int msg);

    static void _rotateAbsolute(InstrumentVariant& _revolver, int n_slot);
    static void _rotateToNext(InstrumentVariant& _revolver, int n_slot);
    static void _rotateToPrevious(InstrumentVariant& _revolver, int n_slot);

    static void _fbAddFilledEppendorf(InstrumentVariant& _revolver, int msg);

};


class PumpOperations {

public:

    static Operation_t setSpeed;

    static void _setSpeed(InstrumentVariant& _pump, int speed);

};


class MotorStirrerOperations {

public:

    static Operation_t setSpeed;

    static void _setSpeed(InstrumentVariant& _motorStirrer, int speed);

};

class SleepOperation {
public:

    static Operation_t sleep;
    static Operation_t sleepMinutes;

    static void _sleep(InstrumentVariant& _time, int ms);
    static void _sleepMinutes(InstrumentVariant& _time, int32_t minutes);

};

#endif
