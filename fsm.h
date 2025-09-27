/*!
 * @file       fsm.h
 * @brief      A Finite State Machine (FSM) in C
 *            
 * @author     Tom Christ
 * @date       2025-09-27
 * @copyright  Copyright (c) 2025 Tom Christ; MIT License
 * @version    0.1  Initial Version
 * 
 */

#ifndef FSM_H_
#define FSM_H_

#include "fsm_events_states.h"
#include <stdbool.h> /* for bool */
#include <stdint.h> /* for int types */

typedef void * fsm_arg_t;
typedef void (*fsm_func_t)(fsm_arg_t);
typedef bool (*fsm_guard_func_t)(fsm_arg_t);

struct fsm;
typedef struct fsm fsm_t;

typedef struct{
  const fsm_func_t func;
  const fsm_arg_t arg;
}fsm_action_t;

typedef struct{
  const fsm_guard_func_t func;
  const fsm_arg_t arg;
}fsm_guard_t;

typedef struct{
  const fsm_event_t event;
  const fsm_state_t toState;
  const fsm_guard_t guard;
  const fsm_action_t action;
}fsm_transition_cfg_t;

typedef struct{
  const fsm_state_t state;
  const fsm_t * const subFsm;
  const fsm_action_t entryAction;
  const fsm_action_t doAction;
  const fsm_action_t exitAction;
  const fsm_transition_cfg_t * const transitions;
  const uint32_t transitionsCount;
}fsm_state_cfg_t;

typedef struct{
  const fsm_state_t initialState;
  const fsm_state_cfg_t * const states;
  const uint32_t statesCount;
}fsm_cfg_t;

struct fsm{
  const fsm_cfg_t * config;
  fsm_state_t currentState;
  bool isFirstRun;
};

typedef enum{
  FSM_RC_OK,
  FSM_RC_ERROR,
  FSM_RC_ERROR_NULLPTR,
  FSM_RC_ERROR_INVALID_CONFIG,
}fsm_RC_t;

fsm_RC_t fsm_init(fsm_t * const io_this, const fsm_cfg_t * const i_config);

fsm_RC_t fsm_reset(fsm_t * const io_this);

fsm_RC_t fsm_process(fsm_t * const io_this, fsm_event_t i_event);



#endif