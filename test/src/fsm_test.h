/**
 * @file       fsm_test.h
 * @brief      Test and Example FSM
 *
 * @author     Tom Christ
 * @copyright  Copyright (c) 2025 Tom Christ; MIT License
 * @date       2025-09-27
 *
 * @version    0.1  Initial Version
 */
#ifndef FSM_TEST_H_
#define FSM_TEST_H_

#include "fsm.h"

/**
 * @brief Program entry
 */
int main(void);

/**
 * @brief Fire an event to the statemachine
 * 
 * @param i_event The event for the FSM to process
 */
void fireEvent(fsm_event_t i_event);

#endif