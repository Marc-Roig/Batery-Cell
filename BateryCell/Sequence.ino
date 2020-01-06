#include "main.h"

// Called to tell the compiler to reserve storage for this
std::map<std::string, InstrumentVariant> Sequence::instruments;
std::map<std::string, Operation_t> Sequence::callbacks;
std::map<int, String> Sequence::parameters;

// Copy from another sequence constructor
Sequence::Sequence(const Sequence& seq) {

    // Copy all elements of one sequence to this one
    for (int i = 0; i < seq.size(); i++ )
        this->pushOperationParams(seq.names_list[i], seq.operations_list[i], seq.msgs_list[i]);

    this->sequence_idx = seq.sequence_idx;

}

// TODO: Remove
Sequence& Sequence::add(const char* instrument, Operation_t callback, int msg /* =0 */ ) {

    if (Sequence::instruments.count(instrument) > 0) {
        names_list.push_back(instrument);
        msgs_list.push_back(msg);
    }
    return *this;

}


Sequence& Sequence::add(const char* instrument, const char* operation_name, int operation_param /* =0 */) {

    if (!this->pushOperationParams(instrument, operation_name, operation_param))
        throw ("Could not set operations sequence");

    return *this;

}

Sequence& Sequence::addFbParam(const char *instrument, const char* operation_name, const char *firebase_param){

    if (!this->pushOperationParams(instrument, operation_name, -1))
        throw ("Could not set operations sequence");

    // Assign parameter name in the current sequence idx
    parameters[this->size() - 1] = firebase_param;

    return *this;

}


// TODO: Remove
Sequence& Sequence::add(const Sequence& seq) {

    for (int i = 0; i < seq.size(); i++ ) {

        names_list.push_back(seq.names_list[i]);
//        callbacks_list.push_back(seq.callbacks_list[i]);
        msgs_list.push_back(seq.msgs_list[i]);

    }

    return *this;

}

// TODO: Remove
Sequence& Sequence::update(const int& sequence_idx, Operation_t callback, int msg=0) {

//    callbacks_list[sequence_idx] = callback;
    msgs_list[sequence_idx] = msg;

    return *this;

}


Sequence &Sequence::update(const int &sequence_idx, const char *operation_name, int operation_value /* =0 */) {

    operations_list[sequence_idx] = operation_name;
    msgs_list[sequence_idx] = operation_value;

    return *this;

}


Sequence& Sequence::addDelay(int ms) {

    if (Sequence::instruments.count("Sleep") > 0) {

        names_list.push_back("Sleep");
        operations_list.push_back("TIME_DELAY_MS");
        msgs_list.push_back(ms);


    }


    return *this;

}


Sequence& Sequence::addDelayMinutes(int min) {

    if (Sequence::instruments.count("SleepMinutes") > 0) {

        names_list.push_back("SleepMinutes");
        operations_list.push_back("TIME_DELAY_MINUTES");
        msgs_list.push_back(min);

    }

    return *this;

}


void Sequence::setFireBaseId(const String _fireBaseId) {

    fireBaseId = _fireBaseId;

}


int Sequence::execute(int idx) {


    // TODO: Read Core0 message to check if sequence was aborted (soft)

    if (idx >= size()) {
        Serial.println("[ERROR] Sequence index out of range");
        return idx;
    }

    if (instruments.count(names_list[idx]) > 0) {

        // Get instrument variant
        InstrumentVariant& instrumentVariant = instruments.at(names_list[idx]);

        int param = this->getParameter(idx);
        if (param != -1) {

            // Set start of operation timestamp
            FirebaseOperation::setTimestampStart(fireBaseId, idx);

            // Call callback function
            auto callback = this->getCallback(idx);
            callback(instrumentVariant, param);

            // Update fireBase experiment document
            FirebaseOperation::setOperationAsDone(fireBaseId, idx);

            return -1;
        }

    }

    FirebaseOperation::setOperationAsFailed(fireBaseId, idx);
    return idx;

}


int Sequence::executeRange(int idx_a, int idx_b) {

    for (int i = idx_a; i <= idx_b; i++) {
        if (execute(i) != -1) return i;
    }

    return -1;

}


int Sequence::executeAll() {


    for (int i = 0; i < size(); i++) {
        if (execute(i) != -1) {
            return i;
        }
    }

    return -1;
}


String Sequence::list() {

    String seq;
    seq.reserve(600);

    for (int i = 0; i < size(); i++) {

        seq += String(i); // Stringify number
        seq += operations_list[i]; // Get string definition of the callback function stored in the tuple
        seq += names_list[i]; // Get the name of the instrument/operation
        seq += '\n';

    }

    return seq;

}

Sequence::callback_t Sequence::getCallback(int idx) {


    auto operation_name = this->operations_list[idx];
    return callbacks.at(operation_name).first;

}

int Sequence::getParameter(int idx) {

    // Get parameter
    int param = msgs_list[idx];

    // If we need to extract parameter from fireBase
    if (param == -1) {
        if (parameters.count(idx)) {
            String param_name = parameters.at(idx);
            param = FirebaseOperation::getParamByName(fireBaseId, param_name);
        } else {
            Serial.println("[ERROR] operation idx does not have any firebase parameter associated");
            return -1;
        }
    }

    return param;

}

int Sequence::pushOperationParams(const char *instrument, const char *operation_name, int operation_value) {

    // Check instrument and operation are defined
    if (Sequence::instruments.count(instrument)) {

        if (Sequence::callbacks.count(operation_name)) {
            names_list.push_back(instrument);
            operations_list.push_back(operation_name);
            msgs_list.push_back(operation_value);
            return 0;
        } else {
            Serial.print("[ERROR] Operation name was not defined: ");
            Serial.println(operation_name);
        }

    } else {
        Serial.print("[ERROR] Instrument name was not defined: ");
        Serial.println(instrument);
    }

    return -1;
}


template <class T>
void Sequence::setNewInstrument(const char* name, T instrument) {

	Sequence::instruments[name] = InstrumentVariant(instrument);

}


void Sequence::setNewOperation(std::string operation_name, Operation_t operation) {

    Sequence::callbacks[operation_name] = operation;

}


Sequence& Sequence::operator=(const Sequence& seq) {

    // Copy all elements of one sequence to this one
    for (int i = 0; i < seq.size(); i++ ) {

        names_list.push_back(seq.names_list[i]);
        operations_list.push_back(seq.operations_list[i]);
        msgs_list.push_back(seq.msgs_list[i]);

    }

    this->sequence_idx = seq.sequence_idx;

    return *this;
}