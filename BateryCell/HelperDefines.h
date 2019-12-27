
#ifndef HELPERDEFINES_H
#define HELPERDEFINES_H


#define OPEN_VALVE ValveOperations::open
#define CLOSE_VALVE ValveOperations::close

#define REVOLVER_ENABLE RevolverOperations::enable
#define REVOLVER_DISABLE RevolverOperations::disable
#define REVOLVER_ROTATE_ABSOLUTE RevolverOperations::rotateAbsolute
#define REVOLVER_ROTATE_TO_NEXT RevolverOperations::rotateToNext
#define REVOLVER_ROTATE_TO_PREVIOUS RevolverOperations::rotateToPrevious

#define PUMP_SET_SPEED PumpOperations::setSpeed

#define MOTORSTIRRER_SET_SPEED MotorStirrerOperations::setSpeed

#define WS_START_EXPERIMENT WebsocketOperation::startExperiment 
#define WS_START_MEASUREMENTS WebsocketOperation::startMeasurements
#define WS_STOP_EXPERIMENT WebsocketOperation::stopExperiment
#define WS_STOP_MEASUREMENTS WebsocketOperation::stopMeasurements
#define WS_END_OF_SEQUENCE WebsocketOperation::endOfSequence

#define WS_DEBUG_END_OF_OPERATION WebsocketOperationTest::endOfOperation

#endif
