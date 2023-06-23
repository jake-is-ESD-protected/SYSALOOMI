#ifndef _CORE_H_
#define _CORE_H_

/* Main core acting as finite state machine 
* includes witchcraft from here: 
* https://stackoverflow.com/questions/252748/how-can-i-use-an-array-of-function-pointers
* More extremely smart withcery:
* https://stackoverflow.com/a/9972417
*/

#include "Arduino.h"
#include "STM32FreeRTOS.h"
#include "err.h"

/* Types of tasks
* always prefixed with "e_task"
*/
typedef enum task_t{
    e_task_synthesizer,
    e_task_sampler,
    e_task_looper,
    e_task_midicontroller,
    e_task_metronome,
    e_task_parser,
    NUM_TASKS
}task_t;

/* Types of states 
* always prefixed with "e_state"
*/
typedef enum state_t{
    e_state_init,
    e_state_idle,   // should this even exist?
    e_state_fault,
    e_state_synthesizer,
    e_state_sampler,
    e_state_looper,
    e_state_midi_controller,
    NUM_STATES
} state_t;

/* Abstraction struct for tasks
*
* PARAMS
* `task_t type`: task type specified as enum
* `TaskHandle_t handle`: handle to task (FreeRTOS)
* `void* function`: task itself, function pointer
*/
typedef struct task_struct_t{
    task_t type;
    TaskHandle_t handle = NULL;
    uint32_t mem_size = 0;
    uint8_t priority = 0;
    void (*function) (void* p) = NULL;
    bool running = false;
}task_struct_t;

/* Task struct setter
* 
* PARAMS
* `task_struct_t* pt`: task to set params to
* `task_t t`: type of task as specified in enum
* `TaskHandle_t h`: handle for task
* `void (*f)(void* p))`: task itself
*
* RETURNS
* `err_t` status, `e_no_err` if OK
*
* NOTES
* Checks for plausible task parameters,
* e.g. forbids a mem size of 0 or empty
* task function pointers
*/
err_t set_task_config(task_struct_t* pt, 
                     task_t t, 
                     TaskHandle_t h,
                     uint32_t m,
                     uint8_t p, 
                     void (*f)(void* p));

/* Task creation wrapper 
*
* PARAMS
* `task_struct_t* pt`: pointer to task handle
*
* RETURNS
* `err_t` status, `e_no_err` if OK
*
* NOTES
* Checks if task memory could be allocated. Task handle
* is passed along to task itself for read-only purposes
*/
err_t create_task(task_struct_t* pt);

/* Core initializer 
*
* RETURNS
* `err_t` status, `e_no_err` if OK
*
* NOTES
* creates all tasks in default mode (stopped)
*/
err_t core_init();
err_t core_selfcheck();
task_struct_t* core_get_cur_task_handle();

void task_synthesizer(void* p);
void task_sampler(void* p);
void task_looper(void* p);
void task_midicontroller(void* p);
void task_metronome(void* p);
void task_parser(void* p);
void (*function_list[NUM_TASKS])(void* p) = {
    task_synthesizer,
    task_sampler,
    task_looper,
    task_midicontroller,
    task_metronome,
    task_parser
};

/* main core object
* has one instance
* PARAMS
* `state_t state`: current state of core (fsm)
* `void *tasklist`: pointers to task functions in array format
*/
typedef struct core_t{
    state_t state;
    task_struct_t task_list[NUM_TASKS];
}core_t;


#endif