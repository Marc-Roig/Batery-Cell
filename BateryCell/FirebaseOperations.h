//
// Created by m1 on 1/01/20.
//

#ifndef BATERY_CELL_FIREBASEOPERATIONS_H
#define BATERY_CELL_FIREBASEOPERATIONS_H

class FirebaseOperation {
public:

    static FirebaseData firebaseData;

    static void initFirebase();

    static void setOperationAsPending(String firebase_id, int operation_idx);
    static void setOperationAsDone(String firebase_id, int operation_idx);
    static void setOperationAsFailed(String firebase_id, int operation_idx);
    static void setTimestampStart(String firebase_id, int operation_idx);

    static int getParamByName(String firebase_id, String param_name);

    static void uploadSequence(const Sequence& seq, const char *seq_name);
};
#endif //BATERY_CELL_FIREBASEOPERATIONS_H
