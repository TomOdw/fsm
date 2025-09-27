# fsm
C 👀 - A Finite Statemachine!<br>
My Statemachine Implementation in C.

## ✨ Features
📝 Statemachine definition near the graphical UML representation (as near as it gets).<br>
⚙️ Entry, do, and exit actions for each state.<br>
🔀 Transition actions and optional guard conditions.<br>
🌳 Nested Statemachines support.<br>
🧪 Includes a working example in the test folder.<br>


## 📝 FSM Definition
A Statemachine is implemented in the following manner:
```c
/*** MAIN STATEMACHINE ***/
static const fsm_cfg_t fsmMainCfg = {
    .initialState = FSM_STATE_MAIN_1,
    .statesCount = 3,
    .states = (const fsm_state_cfg_t[3]){

        /*** MAIN STATE 1 **********************************************/
        {
            .state = FSM_STATE_MAIN_1,
            .entryAction = {myLog, (fsm_arg_t) "MAIN: STATE1: ENTRY"},
            .doAction = {myLog, (fsm_arg_t) "MAIN: STATE1: DO"},
            .exitAction = {myLog, (fsm_arg_t) "MAIN: STATE1: EXIT"},
            .transitionsCount = 1,
            .transitions = (const fsm_transition_cfg_t[1]){

                /*** EVENT 2 ***/
                {
                    .event = FSM_EVENT_2,
                    .action = {myLog, (fsm_arg_t) "MAIN: STATE1: EVENT2"},
                    .toState = FSM_STATE_MAIN_2,
                },

                /* more events */
                ...
            },
        },
        
        /* more states */
        ...
```


## 🧪 Test / Example
The provided example within [test](/test) implements the pictured statemachine:

![image](/doc/example_state_diagram.png)


