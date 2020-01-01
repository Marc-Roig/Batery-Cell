//
// Created by m1 on 1/01/20.
//

#ifndef BATERY_CELL_FIREBASEOPERATIONS_H
#define BATERY_CELL_FIREBASEOPERATIONS_H

void initFirebase();

class FirebaseOperation {
public:

    static FirebaseData firebaseData;

    static void initFirebase();

    static void setOperationAsPending(String firebase_id, int operation_idx);
    static void setOperationAsDone(String firebase_id, int operation_idx);
    static void setOperationAsFailed(String firebase_id, int operation_idx);
};
#endif //BATERY_CELL_FIREBASEOPERATIONS_H
