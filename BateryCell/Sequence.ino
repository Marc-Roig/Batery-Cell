#include "main.h"

// Called to tell the compiler to reserve storage for this
// static member
std::map<std::string, InstrumentVariant> Sequence::instruments;

// Copy from another sequence constructor
Sequence::Sequence(const Sequence& seq) {

    // Copy all elements of one sequence to this one
    for (int i = 0; i < seq.size(); i++ ) {
        names_list.push_back(seq.names_list[i]);
        callbacks_list.push_back(seq.callbacks_list[i]);
        msgs_list.push_back(seq.msgs_list[i]);

    }

    this->sequence_idx = seq.sequence_idx;

}


Sequence& Sequence::add(const char* instrument, Operation_t callback, int msg /* =0 */ ) {

    if (Sequence::instruments.count(instrument) > 0) {

        names_list.push_back(instrument);
        callbacks_list.push_back(callback);
        msgs_list.push_back(msg);

    } else {
        // std::cout << "[WARNING] Specified instrument ("<< instrument <<") in add Sequence method do not exist\n";
        Serial.println("Specified instrument in add Sequence method do not exist");
    }

    return *this;

}


Sequence& Sequence::add(const Sequence& seq) {

    for (int i = 0; i < seq.size(); i++ ) {

        names_list.push_back(seq.names_list[i]);
        callbacks_list.push_back(seq.callbacks_list[i]);
        msgs_list.push_back(seq.msgs_list[i]);

    }

    return *this;

}


Sequence& Sequence::update(const int& sequence_idx, Operation_t callback, int msg=0) {

    callbacks_list[sequence_idx] = callback;
    msgs_list[sequence_idx] = msg;

    return *this;

}


Sequence& Sequence::addDelay(int ms) {

    if (Sequence::instruments.count("Sleep") > 0) {

        names_list.push_back("Sleep");
        callbacks_list.push_back(SleepOperation::sleep);
        msgs_list.push_back(ms);

    }


    return *this;

}


Sequence& Sequence::addDelayMinutes(int min) {

    if (Sequence::instruments.count("SleepMinutes") > 0) {

        names_list.push_back("SleepMinutes");
        callbacks_list.push_back(Operation_t(SleepOperation::_sleepMinutes, " "));
        msgs_list.push_back(min);

    }

    return *this;

}


void Sequence::setFireBaseId(const String _fireBaseId) {

    fireBaseId = _fireBaseId;

}


int Sequence::execute(int idx) {


    // @TODO: Read Core0 message (?)

    if (idx >= size()) {
        Serial.println("[ERROR] Sequence index out of range");
        return idx;
    }

    if (instruments.count(names_list[idx]) > 0) {

        // Get variant
        InstrumentVariant& instrumentVariant = instruments.at(names_list[idx]);

        // Get callback
        Operation_t operation = callbacks_list[idx];
        void (*callback)(InstrumentVariant&, int) = operation.first;

        // Get message
        int msg = msgs_list[idx];

        // Call callback function
        callback(instrumentVariant, msg);

        FirebaseOperation::setOperationAsDone(fireBaseId, idx);

        return -1;

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
        seq += callbacks_list[i].second; // Get string definition of the callback function stored in the tuple
        seq += names_list[i]; // Get the name of the instrument/operation
        seq += '\n';

    }

    return seq;

}

template <class T>
void Sequence::setNewInstrument(const char* name, T instrum) {

	Sequence::instruments[name] = InstrumentVariant(instrum);

}


Sequence& Sequence::operator=(const Sequence& seq) {

    // Copy all elements of one sequence to this one
    for (int i = 0; i < seq.size(); i++ ) {

        names_list.push_back(seq.names_list[i]);
        callbacks_list.push_back(seq.callbacks_list[i]);
        msgs_list.push_back(seq.msgs_list[i]);

    }

    this->sequence_idx = seq.sequence_idx;

    return *this;
}