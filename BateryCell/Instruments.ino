#include "main.h"

// -- VALVE -- //
Valve::Valve(int _pinValve, int init_state=0) {

    pinMode(_pinValve, OUTPUT);
    digitalWrite(_pinValve, init_state);
    pinValve=_pinValve;
}

void Valve::open() {

    #ifdef DEBUG_VALVE_FLAG
    Serial.print("[DEBUG] {Valve} Opening Valve. PIN: ");
    Serial.println(pinValve);
    #endif

    digitalWrite(pinValve, LOW);

}

void Valve::close() {

    #ifdef DEBUG_VALVE_FLAG
    Serial.print("[DEBUG] {Valve} Closing Valve. PIN: ");
    Serial.println(pinValve);
    #endif

    digitalWrite(pinValve, HIGH);

}

// -- MOTOR STIRRER -- //

MotorStirrer::MotorStirrer(int _pin, int _channel) : pin(_pin), channel(_channel) {

    ledcSetup(channel, 5000, 8);
    ledcAttachPin(pin, channel);

}

void MotorStirrer::rampDown(int bottomSpeed) const {

    for (int i = MIN_START_SPEED; i > bottomSpeed; i--) {
        ledcWrite(channel, i);
        delay(30);
    }

}

void MotorStirrer::setSpeed(int new_speed) {

    if (speed < MIN_START_SPEED && new_speed < MIN_START_SPEED && new_speed != 0 )
        rampDown(new_speed);

    speed = new_speed;
    ledcWrite(channel, speed);


}

// -- REVOLVER -- //

Revolver::Revolver(int _stepPin, int _dirPin, int _enablePin, int _nSlots, int _microsteps) :
    stepPin(_stepPin), dirPin(_dirPin), enablePin(_enablePin), nSlots(_nSlots)
{

    pinMode(enablePin, OUTPUT);
    digitalWrite(enablePin, HIGH);

    pinMode(dirPin, OUTPUT);
    digitalWrite(dirPin, HIGH);

    pinMode(stepPin, OUTPUT);
    digitalWrite(stepPin, LOW);

    float degreesPerSlot = 360 / nSlots;
    float degreePerStep = 1.8 / _microsteps;
    stepsPerSlot = degreesPerSlot / degreePerStep;

}

void Revolver::enable() {

    digitalWrite(enablePin, LOW);

}

void Revolver::disable() {

    digitalWrite(enablePin, HIGH);

}

void Revolver::rotate(int n_slots) {

    int stepsToMove;

    if (n_slots < 0) {

        stepsToMove = -n_slots * stepsPerSlot;
        digitalWrite(dirPin, LOW);

    } else {

        stepsToMove = n_slots * stepsPerSlot;
        digitalWrite(dirPin, HIGH);

    }

    for (int i = 0; i < stepsToMove; i++) {

        digitalWrite(stepPin, HIGH);
        delayMicroseconds(50);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(500);

    }


}

void Revolver::rotateToNext(uint n_slots) {

    // DEBUG
    #ifdef DEBUG_REVOLVER_FLAG
    Serial.print("[DEBUG] {Revolver} Current Slot: ");
    Serial.print(currentSlot);
    Serial.print(", Slots To Move: ");
    Serial.print(1);
    #endif

    rotate(1);
    setCurrentSlot(currentSlot + 1);
    // Update current slot to firebase
    FirebaseOperation::updateRevolverSlot(currentSlot);

    // DEBUG
    #ifdef DEBUG_REVOLVER_FLAG
    Serial.print(", Updated Slot Position: ");
    Serial.println(currentSlot);
    #endif

}

void Revolver::rotateToPrevious(uint n_slots) {

    // DEBUG
    #ifdef DEBUG_REVOLVER_FLAG
    Serial.print("[DEBUG] {Revolver} Current Slot: ");
    Serial.print(currentSlot);
    Serial.print(", Slots To Move: ");
    Serial.print(-1);
    #endif

    rotate(-1);
    setCurrentSlot(currentSlot - 1);
    // Update current slot to firebase
    FirebaseOperation::updateRevolverSlot(currentSlot);

    // DEBUG
    #ifdef DEBUG_REVOLVER_FLAG
    Serial.print(", Updated Slot Position: ");
    Serial.println(currentSlot);
    #endif


}

void Revolver::rotateAbsolute(uint slot) {

    slot = slot % nSlots;

    int slotsToMove = ((int)slot) - currentSlot;

    // DEBUG
    #ifdef DEBUG_REVOLVER_FLAG
    Serial.print("[DEBUG] {Revolver} Slots To Move: ");
    Serial.print(slotsToMove);
    Serial.print(", Current Slot: ");
    Serial.print(currentSlot);
    Serial.print(", Absolute Slot to Move: ");
    Serial.println(slot);
    #endif

    // Rotate revolver
    rotate(slotsToMove);
    // Update slot value
    setCurrentSlot(slot);
    // Update current slot to firebase
    FirebaseOperation::updateRevolverSlot(currentSlot);

    // DEBUG
    #ifdef DEBUG_REVOLVER_FLAG
    Serial.print(", Updated Slot: ");
    Serial.print(currentSlot);
    Serial.print(", NSlots: ");
    Serial.println(nSlots);
    #endif

}

void Revolver::setCurrentSlot(int newSlot) {

    if (newSlot >= nSlots) {
        currentSlot = newSlot % nSlots;
    }
    else if (newSlot < 0) currentSlot = nSlots - ((-newSlot) % nSlots);
    else currentSlot = newSlot;

}


Pump::Pump(int _pinA, int _pinB) {

    // Start-stop pin
    pinA = _pinA;
    pinMode(pinA, OUTPUT);

    // CW-CCW pin
    pinB = _pinB;
//    pinMode(pinB, OUTPUT);

    // Init pins
    digitalWrite(pinA, LOW);
//    digitalWrite(pinB, HIGH);

}

void Pump::setSpeed(int speed) {

    if (speed == 0) {
        digitalWrite(pinA, LOW);

        #ifdef DEBUG_PUMP_FLAG
        Serial.print("[DEBUG] {Pump} Turning off Pump. PIN: ");
        Serial.println(pinA);
        #endif
    }
    else {
        digitalWrite(pinA, HIGH);

        #ifdef DEBUG_PUMP_FLAG
        Serial.print("[DEBUG] {Pump} Turning on Pump. PIN: ");
        Serial.println(pinA);
        #endif

    }

}
