#include "fsm_test.h"

#include <stdio.h>

/******************************************************************************/
/* FUNCTION PREDECLARATION FOR FSM                                            */
/******************************************************************************/
static void myLog(fsm_arg_t i_message);
static void resetFsm(fsm_arg_t i_fsm);

/******************************************************************************/
/* PRIVATE STATIC VARIABLES                                                   */
/******************************************************************************/
static fsm_t fsmMain = {0};
static fsm_t fsmSub = {0};

/******************************************************************************/
/* STATEMACHINE CONFIGURATION                                                 */
/******************************************************************************/
/*** MAIN STATEMACHINE ***/
static const fsm_cfg_t fsmMainCfg = {
    .initialState = FSM_STATE_MAIN_1,
    .statesCount = 3,
    .states = (const fsm_state_cfg_t[3]){

        /*** MAIN STATE 1 *****************************************************/
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
            },
        },

        /*** MAIN STATE 2 *****************************************************/
        {
            .state = FSM_STATE_MAIN_2,
            .entryAction = {myLog, (fsm_arg_t) "MAIN: STATE2: ENTRY"},
            .doAction = {myLog, (fsm_arg_t) "MAIN: STATE2: DO"},
            .exitAction = {myLog, (fsm_arg_t) "MAIN: STATE2: EXIT"},
            .transitionsCount = 3,
            .transitions = (const fsm_transition_cfg_t[3]){

                /*** EVENT 1 ***/
                {
                    .event = FSM_EVENT_1,
                    .action = {myLog, (fsm_arg_t) "MAIN: STATE2: EVENT1"},
                    .toState = FSM_STATE_MAIN_1,
                },

                /*** EVENT 2 ***/
                {
                    .event = FSM_EVENT_2,
                    .action = {myLog, (fsm_arg_t) "MAIN: STATE2: EVENT2"},
                    .toState = FSM_STATE_MAIN_2,
                },

                /*** EVENT 3 ***/
                {
                    .event = FSM_EVENT_3,
                    .action = {myLog, (fsm_arg_t) "MAIN: STATE2: EVENT3"},
                    .toState = FSM_STATE_MAIN_SUB,
                },
            },
        },

        /*** MAIN STATE SUB ***************************************************/
        {
            .state = FSM_STATE_MAIN_SUB,
            .entryAction = {resetFsm, (fsm_arg_t)&fsmSub},
            .subFsm = &fsmSub,
            .transitionsCount = 1,
            .transitions = (const fsm_transition_cfg_t[1]){
                /*** EVENT 3 ***/
                {
                    .event = FSM_EVENT_3,
                    .action = {myLog, (fsm_arg_t) "MAIN: STATE SUB: EVENT3"},
                    .toState = FSM_STATE_MAIN_2,
                },
            },
        },
    },
};

/*** SUB STATEMACHINE ***/
static const fsm_cfg_t fsmSubCfg = {
    .initialState = FSM_STATE_SUB_1,
    .statesCount = 2,
    .states = (const fsm_state_cfg_t[2]){

        /*** SUB STATE 1 ******************************************************/
        {
            .state = FSM_STATE_SUB_1,
            .entryAction = {myLog, (fsm_arg_t) "SUB: STATE1: ENTRY"},
            .doAction = {myLog, (fsm_arg_t) "SUB: STATE1: DO"},
            .exitAction = {myLog, (fsm_arg_t) "SUB: STATE1: EXIT"},
            .transitionsCount = 2,
            .transitions = (const fsm_transition_cfg_t[2]){

                /*** EVENT 1 ***/
                {
                    .event = FSM_EVENT_1,
                    .action = {myLog, (fsm_arg_t) "SUB: STATE1: EVENT1"},
                    .toState = FSM_STATE_SUB_1,
                },

                /*** EVENT 2 ***/
                {
                    .event = FSM_EVENT_2,
                    .action = {myLog, (fsm_arg_t) "SUB: STATE1: EVENT2"},
                    .toState = FSM_STATE_SUB_2,
                },
            },
        },

        /*** SUB STATE 2 ******************************************************/
        {
            .state = FSM_STATE_SUB_2,
            .entryAction = {myLog, (fsm_arg_t) "SUB: STATE2: ENTRY"},
            .doAction = {myLog, (fsm_arg_t) "SUB: STATE2: DO"},
            .exitAction = {myLog, (fsm_arg_t) "SUB: STATE2: EXIT"},
            .transitionsCount = 1,
            .transitions = (const fsm_transition_cfg_t[1]){

                /*** EVENT 1 ***/
                {
                    .event = FSM_EVENT_1,
                    .action = {myLog, (fsm_arg_t) "SUB: STATE2: EVENT1"},
                    .toState = FSM_STATE_SUB_1,
                },
            },
        },
    },
};

/******************************************************************************/
/* FUNCTION IMPLEMENTATIONS                                                   */
/******************************************************************************/
int main(void)
{
    printf("main: started\n");
    fsm_RC_t res = FSM_RC_ERROR;

    /* Initialize the statemachines */
    res = fsm_init(&fsmMain, &fsmMainCfg);
    if (res != FSM_RC_OK)
    {
        printf("ERROR: main: error initializing main fsm\n");
        return -1;
    }
    printf("main: main fsm initialized\n");
    
    res = fsm_init(&fsmSub, &fsmSubCfg);
    if (res != FSM_RC_OK)
    {
        printf("ERROR: main: error initializing sub fsm\n");
        return -1;
    }
    printf("main: sub fsm initialized\n");

    printf("main: simulating events\n");
    /* Simulate events to go through the statemachine */
    fireEvent(FSM_EVENT_1); /* stay in main state 1*/
    fireEvent(FSM_EVENT_3); /* stay in main state 1*/
    fireEvent(FSM_EVENT_2); /* go to main state 2*/
    fireEvent(FSM_EVENT_2); /* stay in main state 2*/
    fireEvent(FSM_EVENT_1); /* go back to main state 1*/
    fireEvent(FSM_EVENT_3); /* stay in main state 1, test not implemented event */
    fireEvent(FSM_EVENT_2); /* go to main state 2*/
    fireEvent(FSM_EVENT_3); /* go to sub state (sub fsm) */
    fireEvent(FSM_EVENT_1); /* stay in sub state 1*/
    fireEvent(FSM_EVENT_2); /* go to sub state 2*/
    fireEvent(FSM_EVENT_2); /* stay in sub state 2*/
    fireEvent(FSM_EVENT_1); /* go back to sub state 1*/
    fireEvent(FSM_EVENT_3); /* leave the sub fsm, back to main state 2 */
    fireEvent(FSM_EVENT_3); /* go back to sub state (sub fsm) */
    fireEvent(FSM_EVENT_1); /* stay in sub state 1*/
    printf("main: simulating done\n");
    return 0;
}

void fireEvent(fsm_event_t i_event)
{
    fsm_RC_t res = FSM_RC_ERROR;

    uint32_t eventNum = (uint32_t)i_event + 1;

    printf("fireEvent: EVENT%d\n", eventNum);

    /* Fire event in main statemachine */
    res = fsm_process(&fsmMain, i_event);
    if (res != FSM_RC_OK)
    {
        printf("ERROR: fireEvent: error firing event %d in main fsm\n", i_event);
        return;
    }
}

static void myLog(fsm_arg_t i_message)
{
    const char *messageStr = (const char *)i_message;
    printf("myLog: %s\n", messageStr);
}

static void resetFsm(fsm_arg_t i_fsm)
{
    fsm_t *const fsm = (fsm_t *const)i_fsm;
    if (fsm == NULL)
    {
        printf("ERROR: resetFsm: nullptr\n");
        return;
    }
    fsm_RC_t res = fsm_reset(fsm);
    if (res != FSM_RC_OK)
    {
        printf("ERROR: resetFsm: nullptr\n");
    }
}
