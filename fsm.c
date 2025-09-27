#include "fsm.h"

#include <stddef.h> /* for NULL */

static void perform_action(const fsm_action_t *const action)
{
  if (action != NULL)
  {
    if (action->func != NULL)
    {
      action->func(action->arg);
    }
  }
}

static fsm_state_cfg_t const *get_state_cfg(const fsm_t *const i_this, fsm_state_t state)
{
  if (i_this == NULL)
  {
    return NULL;
  }
  if (i_this->config == NULL)
  {
    return NULL;
  }

  for (uint32_t i = 0; i < i_this->config->statesCount; i++)
  {
    if (i_this->config->states[i].state == state)
    {
      return &i_this->config->states[i];
    }
  }
  return NULL;
}

fsm_RC_t fsm_init(fsm_t *const io_this, const fsm_cfg_t *const i_config)
{
  if (io_this == NULL || i_config == NULL)
  {
    return FSM_RC_ERROR_NULLPTR;
  }

  /*** Perform cheks on the configuration ***/
  /* Check if the states array is non null */
  if (i_config->states == NULL)
  {
    return FSM_RC_ERROR_INVALID_CONFIG;
  }

  /* Check if at least one state in states array */
  if (i_config->statesCount == 0)
  {
    return FSM_RC_ERROR_INVALID_CONFIG;
  }

  /* Iterate the states */
  bool initStateFound = false;
  for (uint32_t i = 0; i < i_config->statesCount; i++)
  {
    /* Check if the initial state is in the states array */
    if (i_config->states[i].state == i_config->initialState)
    {
      initStateFound = true;
    }
    /* Check if the state has at least one transition */
    if (i_config->states[i].transitionsCount == 0)
    {
      return FSM_RC_ERROR_INVALID_CONFIG;
    }
    if (i_config->states[i].transitions == NULL)
    {
      return FSM_RC_ERROR_INVALID_CONFIG;
    }
  }
  if (initStateFound == false)
  {
    return FSM_RC_ERROR_INVALID_CONFIG;
  }

  /* Check transistion array validity */
  for (uint32_t i = 0; i < i_config->statesCount; i++)
  {
    if (i_config->states[i].transitionsCount > 0 &&
        i_config->states[i].transitions == NULL)
    {
      return FSM_RC_ERROR_INVALID_CONFIG;
    }
  }

  /*** Configuration checks done ***/
  io_this->config = i_config;
  return fsm_reset(io_this);
}

fsm_RC_t fsm_reset(fsm_t *const io_this)
{
  if (io_this == NULL)
  {
    return FSM_RC_ERROR_NULLPTR;
  }

  io_this->currentState = io_this->config->initialState;
  io_this->isFirstRun = true;
  return FSM_RC_OK;
}

fsm_RC_t fsm_process(fsm_t *const io_this, fsm_event_t i_event)
{
  if (io_this == NULL)
  {
    return FSM_RC_ERROR_NULLPTR;
  }
  if (io_this->config == NULL)
  {
    return FSM_RC_ERROR_NULLPTR;
  }

  const fsm_cfg_t *cfg = io_this->config;

  /* Get the current state cfg */
  fsm_state_t currState = io_this->currentState;
  const fsm_state_cfg_t *currStateCfg = get_state_cfg(io_this, currState);
  if (currStateCfg == NULL)
  {
    return FSM_RC_ERROR_INVALID_CONFIG;
  }

  /* Get the Matching transistion cfg */
  const fsm_transition_cfg_t *transitionCfg = NULL;
  for (uint32_t i = 0; i < currStateCfg->transitionsCount; i++)
  {
    if (currStateCfg->transitions[i].event == i_event)
    {
      transitionCfg = &currStateCfg->transitions[i];
      break;
    }
  }

  /* Get next state cfg and transition action */
  const fsm_state_cfg_t *nextStateCfg = NULL;
  fsm_action_t const *actionTransition = NULL;
  if (transitionCfg != NULL)
  {
    /* Transition defined for event */
    bool guardCond = true;
    /* Check Gurad condition */
    if (transitionCfg->guard.func != NULL)
    {
      guardCond = transitionCfg->guard.func(transitionCfg->guard.arg);
    }

    if (guardCond == true)
    {
      /* Save next state cfg and transition action */
      actionTransition = &transitionCfg->action;
      nextStateCfg = get_state_cfg(io_this, transitionCfg->toState);
      if (nextStateCfg == NULL)
      {
        return FSM_RC_ERROR_INVALID_CONFIG;
      }
    }
    else
    {
      /* Guard condition false, stay in current state */
      nextStateCfg = currStateCfg;
    }
  }
  else
  {
    /* No transition defined for the current event, stay in the current state */
    nextStateCfg = currStateCfg;
  }

  /* Check if it is the first run */
  if (io_this->isFirstRun == true)
  {
    /* First run, perform entry action of initial state */
    perform_action(&(currStateCfg->entryAction));
    io_this->isFirstRun = false;
  }

  /* Perfrom the actions */
  if (currStateCfg == nextStateCfg)
  {
    /* No state change, perform do action */
    perform_action(&(currStateCfg->doAction));

    /* Perform transition action if any */
    perform_action(actionTransition);

    /* Process event in the sub fsm */
    if (currStateCfg->subFsm != NULL)
    {
      fsm_RC_t res = fsm_process((fsm_t *)currStateCfg->subFsm, i_event);
      if (res != FSM_RC_OK)
      {
        return res;
      }
    }
  }
  else
  {
    /* State change, perform do and exit of current state, */
    /* transition action, entry and do action of next state */
    perform_action(&(currStateCfg->doAction));
    perform_action(&(currStateCfg->exitAction));
    perform_action(actionTransition);
    perform_action(&(nextStateCfg->entryAction));
    perform_action(&(nextStateCfg->doAction));

    /* Check if a sub fsm is linked to the next state*/
    if (nextStateCfg->subFsm != NULL)
    {
      /* Process the entering event in the sub fsm */
      fsm_RC_t res = fsm_process((fsm_t *)nextStateCfg->subFsm, i_event);
      if (res != FSM_RC_OK)
      {
        return res;
      }
    }
  }

  /* Save the state change (if happened) */
  io_this->currentState = nextStateCfg->state;
  return FSM_RC_OK;
}