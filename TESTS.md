# Tests

Here we provide the list of tests that HAVE to be passed in order to use this in production.

## Hardware tests

- [ ] Valve tests (For each valve in the system)
    - [ ] Valve opens / closes.
    - [ ] Valve remains closed when power down.
    - [ ] Valve can remain opened for 5 minutes straight.

- [ ] Revolver tests
    - [ ] Can move one slot forward
    - [ ] Can move one slot backwards
    - [ ] Can move 10 slots forward
    - [ ] Can move 10 slots backward    
    - [ ] Can move 40 slots forward
    - [ ] Can move 40 slots backward

- [ ] Power outage failure
    - [ ] Assert experiment is not resumed in a power outage
    - [ ] Assert notification of power outage is sent to the server (NOT IMPLEMENTED YET)

## Communication tests

- [ ] Assert sequence starts when requested from the web
- [ ] Assert sequence is aborted when requested from the web
- [ ] Assert python software clicks where it should
    - [ ] Assert experiment starts when requested from the ESP32
    - [ ] Assert experiment stops when requested from the ESP32
    - [ ] Assert measurements start when requested from the ESP32
    - [ ] Assert measurements stop when requested from the ESP32
- [ ] Assert 