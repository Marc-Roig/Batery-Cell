#include "main.h"

// Called to tell the compiler to reserve storage for this
std::map<std::string, InstrumentVariant> Sequence::instruments;
std::map<std::string, Operation_t> Sequence::callbacks;

// Copy from another sequence constructor
Sequence::Sequence(const Sequence& seq) {

    // Copy all elements of one sequence to this one
    names_list = seq.names_list;
    operations_list = seq.operations_list;
    msgs_list = seq.msgs_list;
    fb_param_list = seq.fb_param_list;

}

Sequence& Sequence::add(const char* instrument, const char* operation_name, int operation_param /* =0 */) {

    if (!this->pushOperationParams(instrument, operation_name, operation_param))
        throw ("Could not set operations sequence");

    return *this;

}

Sequence& Sequence::addFbParam(const char *instrument, const char* operation_name, const char *firebase_param){

    if (!this->pushOperationParams(instrument, operation_name, -1, firebase_param))
        throw ("Could not set operations sequence");

    return *this;

}


Sequence& Sequence::add(const Sequence& seq) {


    for (int i = 0; i < seq.size(); i++ )
        this->pushOperationParams(seq.names_list[i], seq.operations_list[i], seq.msgs_list[i], fb_param_list[i]);

    return *this;

}


Sequence &Sequence::update(const int &sequence_idx, const char *operation_name, int operation_value /* =0 */) {

    operations_list[sequence_idx] = operation_name;
    msgs_list[sequence_idx] = operation_value;

    return *this;

}


Sequence& Sequence::addDelay(int ms) {

    if (Sequence::instruments.count("Sleep") > 0)
        this->pushOperationParams("Sleep", "TIME_DELAY_MS", ms);

    return *this;

}

Sequence& Sequence::addDelayFbParam(const char* firebase_param) {

    if (!this->pushOperationParams("Sleep", "TIME_DELAY_MS", -1, firebase_param))
        throw ("Could not set operations sequence");

    return *this;

}


Sequence& Sequence::addDelayMinutes(int min) {

    if (!this->pushOperationParams("SleepMinutes", "TIME_DELAY_MINUTES", min))
        throw ("Could not set operations sequence");

    return *this;

}


Sequence& Sequence::addDelayMinutesFbParam(const char* firebase_param) {

    if (!this->pushOperationParams("SleepMinutes", "TIME_DELAY_MINUTES", -1, firebase_param))
        throw ("Could not set operations sequence");

    return *this;

}


int Sequence::execute(int idx) {


    // TODO: Read Core0 message to check if sequence was aborted (soft)

    if (idx >= size()) {
        Serial.println("[ERROR] Sequence index out of range");
        return idx;
    }

    if (instruments.count(names_list[idx]) > 0) {

        // Get instrument variant
        InstrumentVariant &instrumentVariant = instruments.at(names_list[idx]);

        // Get operation parameter
        int param = this->getParameter(idx);
        if (param != -1) {

            // Set start of operation timestamp
            FirebaseOperation::setTimestampStart(idx);

            // Call callback function
            auto callback = this->getCallback(idx);
            callback(instrumentVariant, param);

            // Update fireBase experiment document
            FirebaseOperation::setOperationAsDone(idx);

            return -1;
        }

    }
    Serial.print("[ERROR] Operation failed: ");
    Serial.println(idx);
    FirebaseOperation::setOperationAsFailed(idx);
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
    int param ;
    const char *parameter_name = fb_param_list[idx];

    // If there is a firebase param
    if (parameter_name != nullptr) {
        param = FirebaseOperation::getParamByName(parameter_name);
    } else {
        param = msgs_list[idx];
    }

    return param;

}


int Sequence::pushOperationParams(const char *instrument, const char *operation_name, int operation_value /* =0 */,
                                  const char *fb_param /* =nullptr */) {

    // Check instrument and operation are defined
    if (Sequence::instruments.count(instrument)) {
        if (Sequence::callbacks.count(operation_name)) {
            names_list.push_back(instrument);
            operations_list.push_back(operation_name);
            msgs_list.push_back(operation_value);
            fb_param_list.push_back(fb_param);
            return 1;
        } else {
            Serial.print("[ERROR] Operation name was not defined: ");
            Serial.println(operation_name);
        }

    } else {
        Serial.print("[ERROR] Instrument name was not defined: ");
        Serial.println(instrument);
    }

    return 0;
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
    names_list = seq.names_list;
    operations_list = seq.operations_list;
    msgs_list = seq.msgs_list;
    fb_param_list = seq.fb_param_list;

    return *this;
}