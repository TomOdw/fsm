/**
 * @file       fsm.h
 * @brief      A Finite State Machine (FSM) in C
 *
 *             Statemachine definition near the graphical UML representation
 *             (as near as it gets). Entry, do, and exit actions for each state.
 *             Transition actions and optional guard conditions.
 *             Nested Statemachines support.
 *             Includes a working example in the test folder.
 *
 * @author     Tom Christ
 * @copyright  Copyright (c) 2025 Tom Christ; MIT License
 * @date       2025-09-27
 *
 * @version    0.1  Initial Version
 */
#ifndef FSM_H_
#define FSM_H_

/******************************************************************************/
/*** Include files                                                            */
/******************************************************************************/
#include "fsm_events_states.h" /* "extern" defined events and states */

#include <stdbool.h> /* for bool */
#include <stdint.h>  /* for int types */

/******************************************************************************/
/*** Types                                                                    */
/******************************************************************************/
typedef void *fsm_arg_t;                     /**< Argument for FSM */
typedef void (*fsm_func_t)(fsm_arg_t);       /**< FuncPtr for FSM funcs */
typedef bool (*fsm_guard_func_t)(fsm_arg_t); /**< FuncPtr for FSM guards */

struct fsm;               /**< Forward declaration of statemachine struct*/
typedef struct fsm fsm_t; /**< Statemachine struct */

/**
 * @brief Action Function
 */
typedef struct
{
  const fsm_func_t func; /**< Function Pointer */
  const fsm_arg_t arg;   /**< Argument for the function */
} fsm_action_t;

/**
 * @brief Guard Condition Function
 */
typedef struct
{
  const fsm_guard_func_t func; /**< Function Pointer */
  const fsm_arg_t arg;         /**< Argument for the function */
} fsm_guard_t;

/**
 * @brief Transition Configuration
 *
 * Defines a transition from one state to another on a specific event,
 * with an optional guard condition and an action to be performed during the
 * transition.
 */
typedef struct
{
  const fsm_event_t event;   /**< Event triggering the transition */
  const fsm_state_t toState; /**< The next state entered by the transition */
  const fsm_guard_t guard;   /**< Optional guard condition */
  const fsm_action_t action; /**< Optional transition action */
} fsm_transition_cfg_t;

/**
 * @brief State Configuration
 *
 * Defines a state in the FSM, including its entry, do, and exit actions,
 * as well as the transitions that can occur from this state.
 */
typedef struct
{
  const fsm_state_t state;                       /**< The Enum entry for this state */
  const fsm_t *const subFsm;                     /**< [optional] Sub-FSM which is run during this state */
  const fsm_action_t entryAction;                /**< [optional] Action performed on state entry */
  const fsm_action_t doAction;                   /**< [optional] Action performed during every event */
  const fsm_action_t exitAction;                 /**< [optional] Action perfored on state exit */
  const fsm_transition_cfg_t *const transitions; /**< Array of transitions from this state */
  const uint32_t transitionsCount;               /**< Number of transitions in the transitions array */
} fsm_state_cfg_t;

/**
 * @brief Statemachine Configuration
 *
 * Defines the configuration of the FSM, including its initial state and
 * the states that make up the FSM.
 */
typedef struct
{
  const fsm_state_t initialState;      /**< The initial state of the FSM */
  const fsm_state_cfg_t *const states; /**< Array of states in the FSM */
  const uint32_t statesCount;          /**< Number of states in the states array */
} fsm_cfg_t;

/**
 * @brief Statemachine Struct
 *
 * Represents an instance of the FSM, including its current state and
 * configuration.
 */
struct fsm
{
  const fsm_cfg_t *config;  /**< Pointer to the FSM configuration */
  fsm_state_t currentState; /**< The current state of the FSM */
  bool isFirstRun;          /**< Flag indicating if the FSM has been run yet */
};

/**
 * @brief Return Codes for FSM functions
 */
typedef enum
{
  FSM_RC_OK,                   /**< Operation successful */
  FSM_RC_ERROR,                /**< Generic error */
  FSM_RC_ERROR_NULLPTR,        /**< Null pointer error */
  FSM_RC_ERROR_INVALID_CONFIG, /**< Invalid configuration error */
} fsm_RC_t;

/******************************************************************************/
/*** API Functions                                                            */
/******************************************************************************/

/**
 * @brief Initialize the FSM with the given configuration
 *
 * @param io_this Pointer to the FSM instance to initialize
 * @param i_config Pointer to the FSM configuration
 *
 * @return FSM_RC_OK on success, error code otherwise
 */
fsm_RC_t fsm_init(fsm_t *const io_this, const fsm_cfg_t *const i_config);

/**
 * @brief Reset the FSM to its initial state
 *
 * @param io_this Pointer to the FSM instance to reset
 *
 * @return FSM_RC_OK on success, error code otherwise
 */
fsm_RC_t fsm_reset(fsm_t *const io_this);

/**
 * @brief Process an event in the FSM, potentially causing a state transition
 *
 * @param io_this Pointer to the FSM instance
 * @param i_event The event to process
 *
 * @return FSM_RC_OK on success, error code otherwise
 */
fsm_RC_t fsm_process(fsm_t *const io_this, fsm_event_t i_event);

#endif /* FSM_H_ */