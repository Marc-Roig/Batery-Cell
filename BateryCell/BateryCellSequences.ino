#include "main.h"


Sequence buildSequenceEppendorfFill() {

    Sequence seqFillNextEppendorf;

    seqFillNextEppendorf.add("R", "REVOLVER_ROTATE_TO_NEXT", 1)
            .add("V5", "OPEN_VALVE")
            .add("V6", "OPEN_VALVE")
            .addDelay(EPENDORF_FILL_TIME)
            .add("V5", "CLOSE_VALVE")
            .add("V6", "CLOSE_VALVE")
            .add("R", "FB_ADD_EPPENDORF_FILLED")
            .addDelay(1000);

    return seqFillNextEppendorf;

}

Sequence buildSequenceDrainWaste() {

    Sequence seqDrainWaste;
    seqDrainWaste.add("R", "REVOLVER_ROTATE_ABSOLUTE", 0)
            .add("V5", "OPEN_VALVE")
            .add("V6", "OPEN_VALVE")
            .addDelay(TIME_UNTIL_EMPTY_MS)
            .add("V5", "CLOSE_VALVE")
            .add("V6", "CLOSE_VALVE")
            .addDelay(1000);

    return seqDrainWaste;

}

Sequence buildPhase1() {

    Sequence phase1;
    phase1.add("R", "REVOLVER_ROTATE_ABSOLUTE", 0)
            .add("V1", "OPEN_VALVE")
            .add("V4", "OPEN_VALVE")
            .add("P", "PUMP_SET_SPEED", PUMP_MAX_SPEED)
            .addDelay(PUMP_TIME_UNTIL_FULL_MS)
            .add("P", "PUMP_SET_SPEED", PUMP_STOP)
            .addDelay(100)
            .add("V1", "CLOSE_VALVE")
            .add("V4", "CLOSE_VALVE")
            .add("MSc", "MOTOR_STIRRER_SET_SPEED", MAGNETIC_STIRRER_HIGH_SPEED)
            .add("MSa", "MOTOR_STIRRER_SET_SPEED", MAGNETIC_STIRRER_HIGH_SPEED)
            .addDelay(TIME_MAGNETIC_STIRRERS_CLEANING_MS)
            .add("V5", "OPEN_VALVE")
            .add("V6", "OPEN_VALVE")
            .addDelay(TIME_UNTIL_EMPTY_MS)
            .add("V5", "CLOSE_VALVE")
            .add("V6", "CLOSE_VALVE")
            .addDelay(TIME_BETWEEN_PHASES_MS);

    return phase1;

}

Sequence buildPhase2() {

    Sequence seqFillNextEppendorf = buildSequenceEppendorfFill();
    Sequence seqDrainWaste = buildSequenceDrainWaste();

    Sequence phase2;

    phase2.add("V1", "OPEN_VALVE")
            .add("V4", "OPEN_VALVE")
            .add("P", "PUMP_SET_SPEED", PUMP_MAX_SPEED)
            .addDelay(PUMP_TIME_UNTIL_FULL_MS)
            .add("V1", "CLOSE_VALVE")
            .add("V4", "CLOSE_VALVE")
            .add("P", "PUMP_SET_SPEED", PUMP_STOP)
            .add("MSc", "MOTOR_STIRRER_SET_SPEED", MAGNETIC_STIRRER_HIGH_SPEED)
            .add("MSa", "MOTOR_STIRRER_SET_SPEED", MAGNETIC_STIRRER_HIGH_SPEED)
            .addDelay(TIME_TO_HOMOGENIZE_SOLUTION_MS)
            .add("Websocket", "WS_START_EXPERIMENT")
            .add("Websocket", "WS_START_MEASUREMENTS")
            .addDelayMinutes(REACTION_TIME_MINUTES)
            .add("Websocket", "WS_STOP_MEASUREMENTS")
            .add("Websocket", "WS_STOP_EXPERIMENT")
            .add("MSc", "MOTOR_STIRRER_SET_SPEED", MAGNETIC_STIRRER_STOP)
            .add("MSa", "MOTOR_STIRRER_SET_SPEED", MAGNETIC_STIRRER_STOP)
            .addDelay(5000)
            .add(seqFillNextEppendorf)
            .add(seqDrainWaste);

    return phase2;

}

Sequence buildPhase3() {

    Sequence phase3;

    phase3.add("R", "REVOLVER_ROTATE_ABSOLUTE", 0)
            .add("V2", "OPEN_VALVE")
            .add("V3", "OPEN_VALVE")
            .add("P", "PUMP_SET_SPEED", PUMP_MAX_SPEED)
            .addDelay(PUMP_TIME_UNTIL_FULL_MS)
            .add("P", "PUMP_SET_SPEED", PUMP_STOP)
            .add("V2", "CLOSE_VALVE")
            .add("V3", "CLOSE_VALVE")
            .add("MSc", "MOTOR_STIRRER_SET_SPEED", MAGNETIC_STIRRER_HIGH_SPEED)
            .add("MSa", "MOTOR_STIRRER_SET_SPEED", MAGNETIC_STIRRER_HIGH_SPEED)
            .addDelay(TIME_MAGNETIC_STIRRERS_CLEANING_MS)
            .add("V5", "OPEN_VALVE")
            .add("V6", "OPEN_VALVE")
            .addDelay(TIME_UNTIL_EMPTY_MS)
            .add("V5", "CLOSE_VALVE")
            .add("V6", "CLOSE_VALVE");

    return phase3;

}