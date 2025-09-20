#ifndef FSM_EVENTS_STATES_H_
#define FSM_EVENTS_STATES_H_

/**
 * @brief Statemachine events
 */
typedef enum fsm_event{
  FSM_EVENT_1,
  FSM_EVENT_2,
  FSM_EVENT_3,
}fsm_event_t;

/**
 * @brief Statemachine states
 */
typedef enum fsm_state{
  FSM_STATE_MAIN_1,
  FSM_STATE_MAIN_2,
  FSM_STATE_MAIN_SUB,
  FSM_STATE_SUB_1,
  FSM_STATE_SUB_2,
}fsm_state_t;

#endif
