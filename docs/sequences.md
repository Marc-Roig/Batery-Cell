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
                    // (int) Optional parameter, if null it's not used. Example: 10
                    "parameter": null
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
        }   
    }
}
```

Also, in each experiment field in firebase, inside each action a parameter key will be introduced for each operation that needs one.