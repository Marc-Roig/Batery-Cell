# Firebase Structure

Each sequence will be stored in firebase in the sequences keys:

```json5
{
    // Stores all the defined sequences
    "sequences": {
        // Sequence definition
        "name_of_sequence": {
            // List of operations of this sequence
            "operations": [
                // Operation 1
                {
                    // (string) name of the instrument. Example: "R"
                    "instrument": null,
                    // (string) definition of the operation to execute. Example: "move_to"
                    "operation": null,
                    // (int) Optional value, if null it's not used. Example: 10
                    "value": null
                },
                // ... 
            ],
            // Sequence required input parameters
            "parameters": {
                // parameters that have to be tuned manually
                "manual": {
                    // (string / int) parameter
                    "param_name": null
                }, 
                // needed parameters by the microcontroller
                "microcontroller": {
                    "param_name": null
                }, 
                // needed parameters by the pc  
                "pc": {
                    "param_name": null
                }
            },
            // List of which eppendorfs where filled during the experiment
            "eppendorfs_filled": [1, 2],
            // (int) How many times the cell had been used without cleaning before the experiment
            // Retrieve from the hardware document
            "n_cell_usages": null,
            // (datetime) How much time the cell had been used ...
            // Retrieve from the hardware document
            "time_cell_usages": null
        }   
    }
}
```

Also, in each experiment field in firebase, inside each action a parameter key will be introduced for each operation that needs one.