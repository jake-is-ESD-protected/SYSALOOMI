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

/* Types of tasks
* always prefixed with "e_task"
*/
typedef enum task_t{
    e_task_sampler,
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
}task_struct_t;

/* Task struct setter 
* PARAMS
* `task_struct_t* pt`: task to set params to
* `task_t t`: type of task as specified in enum
* `TaskHandle_t h`: handle for task
* `void (*f)(void* p))`: task itself
*/
void set_task_config(task_struct_t* pt, 
                     task_t t, 
                     TaskHandle_t h,
                     uint32_t m,
                     uint8_t p, 
                     void (*f)(void* p));

void create_task(task_struct_t* pt);
void core_init();
void core_selfcheck();

void task_sampler(void* p);
void task_metronome(void* p);
void task_parser(void* p);

void (*function_list[NUM_TASKS])(void* p) = {
    task_sampler,
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