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

    // --------------- //
    // -- Variables -- //
    // --------------- //
    typedef void (*callback_t)(InstrumentVariant&, int);

    // Map instrument name with its variant
    static std::map<std::string, InstrumentVariant> instruments;
    // Map operation name with the callback
    static std::map<std::string, Operation_t> callbacks;

    // Map parameter name with idx of operation in the sequence
    std::map<int, String> parameters;

    std::vector<const char*> names_list;    // instrument names
    std::vector<const char*> operations_list; // operation callbacks
    std::vector<int> msgs_list; // content to send to each operation callback

    String fireBaseId = ""; // Needed to update sequence status in fireBase

    int sequence_idx = 0;

    // Default constructor 
    Sequence() = default;
    // Copy from another sequence constructor
    Sequence(const Sequence& seq);

    // -- Operation addition -- //
    // Add instrument operation to the sequence
    Sequence& add(const char* instrument, const char* operation_name, int operation_value = 0);

    // Add instrument operation to sequence and parameter name to retrieve from fireBase
    Sequence& addFbParam(const char* instrument, const char* operation_name, const char* firebase_param = "");

    // Append a new sequence
    Sequence& add(const Sequence& seq);

    // Update element of the sequence
    Sequence& update(const int& sequence_idx, Operation_t callback, int msg);

    // Update element of the sequence
    Sequence &update(const int &sequence_idx, const char *operation_name, int operation_value = 0);

    // Add ms delay in the sequence
    Sequence& addDelay(int ms);

    // Add ms delay in the sequence and retrieve value from fireBase
    Sequence& addDelayFbParam(const char* firebase_param);

    // Add min delay in the sequence and retrieve value from fireBase
    Sequence& addDelayMinutesFbParam(int min);

    // Add min delay in the sequence
    Sequence& addDelayMinutes(int min);

    // Set fireBase ID to update operations status
    void setFireBaseId(const String _fireBaseId);

    // Execute operation from sequence located at position idx
    int execute(int idx);

    // Execute range of operations from sequence from
    // position idx_a to idx_b
    int executeRange(int idx_a, int idx_b);

    // Execute all operations in the sequence
    int executeAll();

    // ----------- //
    // -- Utils -- //
    // ----------- //

    int size() const { return msgs_list.size(); } // Get number of elements in the sequence
    String list(); // List all the elements of the sequence into a string

    callback_t getCallback(int idx); // Get callback from sequence idx
    int getParameter(int idx); // Get operation parameter

    // Insert operation instrument name, callback name and param
    int pushOperationParams(const char* instrument, const char* operation_name, int operation_value);

    // ---------------------------------------- //
    // -- Instrument - Operation definitions -- //
    // ---------------------------------------- //

    template <class T>
    static void setNewInstrument(const char*, T instrument); // Introduce new instrument into the std map
    static void setNewOperation(std::string operation_name, Operation_t operation); // Introduce new operation into the std map

    // --------------- //
    // -- Operators -- //
    // --------------- //
    Sequence& operator=(const Sequence& seq);

};


#endif