#ifndef SEQUENCE_H
#define SEQUENCE_H


// A sequence is formed from multiple actions such as
// - 1: Open valve A.1
// - 2: Wait 3 seconds
// - 3: Close valve A.1
// - 4: Rotate motor LOW_SPEED
// - 5: Wait 6 hours
// - 6: Open valve B.1
// - 7: Wait 3 seconds
// - 8: Close valve B.1

// That means the Sequence needs to store for each step of the sequence:
// - A: The variable to actuate
// - B: The callback function to call and execute
// - C: The message (such as, 3seconds)
class Sequence {

public:

    static std::map<std::string, InstrumentVariant> instruments;

    std::vector<const char*> names_list;
    std::vector<Operation_t> callbacks_list;
    std::vector<int> msgs_list;

    int sequence_idx = 0;

    // Default constructor 
    Sequence() = default;
    // Copy from another sequence constructor
    Sequence(const Sequence& seq);

    // Add instrument operation to the sequence
    Sequence& add(const char* instrument, Operation_t callback, int msg);
    
    // Add instrument operation to the sequence
    Sequence& add(const char* instrument, Operation_t callback);

    // Append a new sequence
    Sequence& add(const Sequence& seq);

    // Update element of the sequence
    Sequence& update(const int& sequence_idx, Operation_t callback, int msg);

    // Add ms delay in the sequence
    Sequence& addDelay(int ms);

    // Add min delay in the sequence
    Sequence& addDelayMinutes(int min);

    // Execute operation from sequence located at position idx
    int execute(int idx);

    // Execute range of operations from sequence from
    // position idx_a to idx_b
    int executeRange(int idx_a, int idx_b);

    // Execute all operations in the sequence
    int executeAll();

    // Get number of elements in the sequence
    int size() const { return msgs_list.size(); }

    // List all the elements of the sequence into a string
    String list();

    // Introduce new instrument into the std map
    template <class T>
    static void setNewInstrument(const char*, T instrument);

    // -- Operators -- //
    Sequence& operator=(const Sequence& seq);

};


#endif