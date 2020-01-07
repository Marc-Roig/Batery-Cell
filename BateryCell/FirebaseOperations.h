//
// Created by m1 on 1/01/20.
//

#ifndef BATERY_CELL_FIREBASEOPERATIONS_H
#define BATERY_CELL_FIREBASEOPERATIONS_H

class FirebaseOperation {
public:

    static FirebaseData firebaseData;
    // Document key for the current sequence. This value will be updated each time
    // a sequence starts
    static String firebaseId;

    static void initFirebase();

    static void setOperationAsPending(int operation_idx);
    static void setOperationAsDone(int operation_idx);
    static void setOperationAsFailed(int operation_idx);
    static void setTimestampStart(int operation_idx);

    static int getParamByName(String param_name);

    // Sequences
    static void uploadSequence(const Sequence& seq, const char *seq_name);

    // Instruments
    static void updateRevolverSlot(int currentSlot);
    static void pushEppendorfFilled(int currentSlot);

};
#endif //BATERY_CELL_FIREBASEOPERATIONS_H
