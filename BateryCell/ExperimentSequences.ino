#include "main.h"

void storeNewSequence(Sequence& newSequence, const char* sequenceName) {

    // Only add if this sequence name was not previously used
    if (sequences.count(sequenceName) == 0) {
        sequences[sequenceName] = newSequence;
    } else {
        Serial.print("[WARNING] ");
        Serial.print(sequenceName);
        Serial.println(" sequence name was already defined");
    }

}

void executeSequence(int, char* sequenceName) {

    if (sequences.count(sequenceName) > 0) {
        Sequence sequenceToExecute = sequences[sequenceName];
        sequenceToExecute.executeAll();
    }

}

