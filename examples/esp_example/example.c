#include "example.h"

#include "esp_log.h"

#define TAG "EXAMPLE"

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
    .states = {

        /*** MAIN STATE 1 *****************************************************/
        {
            .state = FSM_STATE_MAIN_1,
            .entryAction = {myLog, (fsm_arg_t) "MAIN: STATE1: ENTRY"},
            .doAction = {myLog, (fsm_arg_t) "MAIN: STATE1: DO"},
            .exitAction = {myLog, (fsm_arg_t) "MAIN: STATE1: EXIT"},
            .transitions = (const fsm_transition_cfg_t[]){

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
            .transitions = (const fsm_transition_cfg_t[]){

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
            .transitions = (const fsm_transition_cfg_t[]){
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
    .states = {

        /*** SUB STATE 1 ******************************************************/
        {
            .state = FSM_STATE_SUB_1,
            .entryAction = {myLog, (fsm_arg_t) "SUB: STATE1: ENTRY"},
            .doAction = {myLog, (fsm_arg_t) "SUB: STATE1: DO"},
            .exitAction = {myLog, (fsm_arg_t) "SUB: STATE1: EXIT"},
            .transitions = (const fsm_transition_cfg_t[]){

                /*** EVENT 1 ***/
                {
                    .event = FSM_EVENT_1,
                    .action = {myLog, (fsm_arg_t) "SUB: STATE1: EVENT1"},
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
            .transitions = (const fsm_transition_cfg_t[]){

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
void run_example(void)
{
    ESP_LOGI(TAG, "example started");
    fsm_RC_t res = FSM_RC_ERROR;

    res = fsm_init(&fsmMain, &fsmMainCfg);
    if (res != FSM_RC_OK)
    {
        ESP_LOGE(TAG, "example: error initializing main fsm");
        return;
    }

    res = fsm_init(&fsmSub, &fsmSubCfg);
    if (res != FSM_RC_OK)
    {
        ESP_LOGE(TAG, "example: error initializing sub fsm");
        return;
    }
}

static void myLog(fsm_arg_t i_message)
{
    const char *messageStr = (const char *)i_message;
    ESP_LOGI(TAG, "%s", messageStr);
}

static void resetFsm(fsm_arg_t i_fsm)
{
    fsm_t *const fsm = (fsm_t *const)i_fsm;
    if (fsm == NULL)
    {
        ESP_LOGE(TAG, "resetFsm: nullptr");
        return;
        fsm_reset(fsm);
        ESP_LOGI(TAG, "SUB: FSM reset");
    }
}
