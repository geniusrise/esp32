#include <Arduino.h>
#include "state_machine.h"

StateMachine stateMachine;

void setup()
{
    // Initialize the state machine, which will also initialize all other managers
    stateMachine = StateMachine();
}

void loop()
{
    // Main loop should delegate to the state machine
    stateMachine.currentStateAction();
}
