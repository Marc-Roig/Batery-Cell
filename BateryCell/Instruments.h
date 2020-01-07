
#ifndef INSTRUMENTS_H
#define INSTRUMENTS_H

class Valve {

private:

	int pinValve;
   bool is_opened = false;

public:

	Valve() = default;

	Valve(int pinValve, int init_state);

	void open();
	void close();

};

class MotorStirrer {
private:

	static const int MIN_START_SPEED = 170;
	
	void rampDown(int bottomSpeed) const;

public:

	int pin=-1, channel=-1;
	int speed=0;

	MotorStirrer() = default;
	MotorStirrer(int pin, int channel);

	void setSpeed(int speed);


};

class Revolver {
private:
	
	int stepPin=-1, dirPin=-1, enablePin=-1, currentSlot=0;
	int nSlots=0;
	float stepsPerSlot=0;

public:

	Revolver() = default;

	Revolver(int _stepPin, int _dirPin, int _enablePin, int _nSlots, int _stepsPerSlot);

	void enable();

	void disable();

	void rotate(int n_slots);

	void rotateToNext(uint n_slots);

	void rotateToPrevious(uint n_slots);

    void rotateAbsolute(uint slot);

	void setCurrentSlot(int newSlot);

	int getCurrentSlot() { return currentSlot; }

};

class Pump {

public:

	int pinA=-1, pinB=-1;
	int speed=0;

	Pump() = default;

	Pump(int _pinA, int _pinB);

	void setSpeed(int speed);

};

#endif
