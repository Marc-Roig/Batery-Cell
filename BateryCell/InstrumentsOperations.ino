#include "main.h"

// -- VALVE -- //

Operation_t ValveOperations::open  { ValveOperations::_open , "Open Valve"};
Operation_t ValveOperations::close { ValveOperations::_close, "Close Valve"};

// Transform variant into Valve type and open valve
void ValveOperations::_open(InstrumentVariant& _valve, int msg) {

    // Check if variant is actually holding a Valve type
    if (!_valve.is<Valve>()) throw("INCORRECT VARIANT TYPE");

    // Transform variant into a Valve variable
    Valve valve = _valve.as_valve;

    // Open valve
    valve.open();
        
}

// Transform variant into Valve type and close valve
void ValveOperations::_close(InstrumentVariant& _valve, int msg) {

    // Check if variant is actually holding a Valve type
    if (!_valve.is<Valve>()) throw("INCORRECT VARIANT TYPE");

    // Transform variant into a Valve variable
    Valve valve = _valve.as_valve;

    // Open valve
    valve.close();
    
}


// -- REVOLVER -- //

Operation_t RevolverOperations::enable           { RevolverOperations::_enable           , "Enable Revolver"        };
Operation_t RevolverOperations::disable          { RevolverOperations::_disable          , "Disable Revolver"       };
Operation_t RevolverOperations::rotateAbsolute   { RevolverOperations::_rotateAbsolute   , "Rotate Revolver: X"     };
Operation_t RevolverOperations::rotateToNext     { RevolverOperations::_rotateToNext     , "Rotate Revolver to next"};
Operation_t RevolverOperations::rotateToPrevious { RevolverOperations::_rotateToPrevious , "Rotate Revolver to next"};


void RevolverOperations::_enable(InstrumentVariant& _revolver, int msg) {

        // Check if variant is actually holding a Revolver type
    if (!_revolver.is<Revolver>()) throw ("INCORRECT VARIANT TYPE, EXPECTED REVOLVER TYPE");

    // Transform variant into a Revolver variable
    Revolver& revolver = _revolver.as_revolver;

    // Rotate revolver
    revolver.enable();

}

void RevolverOperations::_disable(InstrumentVariant& _revolver, int msg) {

    // Check if variant is actually holding a Revolver type
    if (!_revolver.is<Revolver>()) throw ("INCORRECT VARIANT TYPE, EXPECTED REVOLVER TYPE");

    // Transform variant into a Revolver variable
    Revolver& revolver = _revolver.as_revolver;

    // Rotate revolver
    revolver.disable();

}

void RevolverOperations::_rotateAbsolute(InstrumentVariant& _revolver, int n_slot) {

    // Check if variant is actually holding a Revolver type
    if (!_revolver.is<Revolver>()) throw ("INCORRECT VARIANT TYPE, EXPECTED REVOLVER TYPE");

    // Transform variant into a Revolver variable
    Revolver& revolver = _revolver.as_revolver;

    // Rotate revolver
    revolver.rotateAbsolute(n_slot);

}

void RevolverOperations::_rotateToNext(InstrumentVariant& _revolver, int n_slot) {

    // Check if variant is actually holding a Revolver type
    if (!_revolver.is<Revolver>()) throw ("INCORRECT VARIANT TYPE, EXPECTED REVOLVER TYPE");

    // Transform variant into a Revolver variable
    Revolver& revolver = _revolver.as_revolver;

    // Rotate revolver
    revolver.rotateToNext(n_slot);

}

void RevolverOperations::_rotateToPrevious(InstrumentVariant& _revolver, int n_slot) {

    // Check if variant is actually holding a Revolver type
    if (!_revolver.is<Revolver>()) throw ("INCORRECT VARIANT TYPE, EXPECTED REVOLVER TYPE");

    // Transform variant into a Revolver variable
    Revolver& revolver = _revolver.as_revolver;

    // Rotate revolver
    revolver.rotateToPrevious(n_slot);

}

// -- PUMP -- //
Operation_t PumpOperations::setSpeed { PumpOperations::_setSpeed, "Set Pump speed: X"};

void PumpOperations::_setSpeed(InstrumentVariant& _pump, int speed) {

    // Check if variant is actually holding a Revolver type
    if (!_pump.is<Pump>()) {
        throw ("INCORRECT VARIANT TYPE, EXPECTED PUMP TYPE");
    }

    // Transform variant into a Revolver variable
    Pump pump = _pump.as_pump;

    // Rotate revolver
    pump.setSpeed(speed);

}

// -- MOTOR STIRRER -- //
Operation_t MotorStirrerOperations::setSpeed { MotorStirrerOperations::_setSpeed, "Setting Motor Stirrer Speed: X"};

void MotorStirrerOperations::_setSpeed(InstrumentVariant& _motorStirrer, int speed) {

    // Check if variant is actually holding a Revolver type
    if (!_motorStirrer.is<MotorStirrer>()) {
        throw ("INCORRECT VARIANT TYPE, EXPECTED MOTORSTIRRER TYPE");
    }

    // Transform variant into a Revolver variable
    MotorStirrer motorStirrer = _motorStirrer.as_motor_stirrer;

    // Rotate revolver
    motorStirrer.setSpeed(speed);

}

// -- SLEEP -- //

Operation_t SleepOperation::sleep        {SleepOperation::_sleep, "Delay: X"};
Operation_t SleepOperation::sleepMinutes {SleepOperation::_sleepMinutes, "Delay minutes: X"};

void SleepOperation::_sleep(InstrumentVariant& _time, int ms) {

    #ifdef DEBUG_DELAY_FLAG
    Serial.print("[DEBUG] {delay} Wait for ");
    Serial.print(ms);
    Serial.println(" ms");
    #endif

    delay(ms);

}

void SleepOperation::_sleepMinutes(InstrumentVariant& _time, int32_t minutes) {

    int64_t total_time_ticks = minutes * 60 * 1000 / portTICK_PERIOD_MS ;

    #ifdef DEBUG_DELAY_FLAG
    Serial.print("[DEBUG] {delayMinutes} Wait for ");
    Serial.print(minutes);
    Serial.println(" minutes");
    #endif

    while (total_time_ticks > portMAX_DELAY) {

        TickType_t xDelay = total_time_ticks % portMAX_DELAY;
        total_time_ticks -= xDelay;
        vTaskDelay(xDelay);

    }

    vTaskDelay((TickType_t) total_time_ticks);

}
