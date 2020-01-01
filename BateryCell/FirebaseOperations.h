//
// Created by m1 on 1/01/20.
//

#ifndef BATERY_CELL_FIREBASEOPERATIONS_H
#define BATERY_CELL_FIREBASEOPERATIONS_H

void initFirebase();

class FirebaseOperation {
public:

    static FirebaseData firebaseData;
    static String workingSequence;

    static void initFirebase();

    static Operation_t setOperationAsPending;
    static Operation_t setOperationAsDone;
    static Operation_t setOperationAsFailed;

    static void _setOperationAsPending(InstrumentVariant& variant, int content);
    static void _setOperationAsDone(InstrumentVariant& variant, int content);
    static void _setOperationAsFailed(InstrumentVariant& variant, int content);
};
#endif //BATERY_CELL_FIREBASEOPERATIONS_H
