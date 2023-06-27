#ifndef _CORE_H_
#define _CORE_H_

/*@brief Main core acting as finite state machine 
* includes witchcraft from here: 
* https://stackoverflow.com/questions/252748/how-can-i-use-an-array-of-function-pointers
* More extremely smart withcery:
* https://stackoverflow.com/a/9972417
*/

#include "Arduino.h"
#include "STM32FreeRTOS.h"
#include "err.h"

/*@brief Types of tasks
* always prefixed with "e_task"
* @name task_t
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

/*@brief Types of states 
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

/*@brief Enum for types of places where a command can
* originate from. Used in cmd_struct_t
*/
typedef enum origin_t{
    e_origin_undefined,
    e_origin_interrupt,
    e_origin_task,
    e_origin_core
}origin_t;

/*@brief Enum for command content types aka jobs to do.
* Used in cmd_struct_t
*/
typedef enum cmd_t{
    e_cmd_undefined,
}cmd_t;

/*@brief Struct for sendable commands
* @param origin (origin_t): Place of command creation
* @param content (cmd_t): The job to do
* @param o_value (uint16_t): Optional 16 bit value
* @note Although not certain, it is assumed that both origin and
* content are 8 bit wide, which makes the entire struct 32 bit
* wide, small enough for atomic access and able to be passed along
* wirh FreeRTOS task notifications
*/
typedef struct cmd_struct_t{
    origin_t origin = e_origin_undefined;
    cmd_t content = e_cmd_undefined;
    uint16_t o_value = 0;
}cmd_struct_t;

/*@brief Abstraction struct for tasks
* @param type (task_t): task type specified as enum
* @param handle (TaskHandle_t): handle to task (FreeRTOS)
* @param mem_size (uint32_t): allocated stack size for task
* @param priority (uint8_t): priority of task
* @param function void(*)(void* p): task itself, function pointer
* @param running (bool): Flag which indicates liveliness of task
*/
typedef struct task_struct_t{
    task_t type;
    TaskHandle_t handle = NULL;
    uint32_t mem_size = 0;
    uint8_t priority = 0;
    void (*function) (void* p) = NULL;
    bool running = false;
}task_struct_t;

/*@brief Task struct setter
* @param pt (task_struct_t*): task to set params to
* @param t (task_t): type of task as specified in enum
* @param h (TaskHandle_t): handle for task
* @param f (void (*)(void* p)): task itself, function pointer
* @returns status (err_t): `e_no_err` if OK
* @note Checks for plausible task parameters,
* e.g. forbids a mem size of 0 or empty
* task function pointers
*/
err_t set_task_config(task_struct_t* pt, 
                     task_t t, 
                     TaskHandle_t h,
                     uint32_t m,
                     uint8_t p, 
                     void (*f)(void* p));

/*@brief Task creation wrapper 
* @param pt (task_struct_t*): pointer to task handle
* @returns status (err_t), `e_no_err` if OK
* @note Checks if task memory could be allocated. Task handle
* is passed along to task itself for read-only purposes
*/
err_t create_task(task_struct_t* pt);

/*@brief Core initializer 
* @returns status (err_t), `e_no_err` if OK
* @note Creates all tasks in default mode (stopped)
*/
err_t core_init();
err_t core_selfcheck();

/*@brief Get task struct handle of specified task
* @param t (task_t) Enum describing task
* @return (task_struct_t*) pointer to task struct
* @note Note that every task also contains its handle
* as passed void* parameters, so there is no need to
* call this function from inside a task
*/
task_struct_t* core_get_task_handle(task_t t);

/*@brief Task notification wrapper for FreeRTOS "xTaskNotify()"
* @param t task_struct_t*, handle to task to be notified
* @param c cmd_struct_t, command to be sent
* @param from_isr bool, specifies ISR or non-ISR origin
*/
void task_notify(task_struct_t* t, cmd_struct_t c, bool from_isr);


/*@brief Task notification wrapper for FreeRTOS "ulTaskNotifyTake"
* @param ticks_to_wait TickType_t, timeout time, use "portMAX_DELAY" for blocking
* @returns cmd_struct_t, received command
*/
cmd_struct_t task_notify_take(TickType_t ticks_to_wait);

cmd_struct_t sleep_until_notified();

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

/*@brief Main core object, has one instance
* @param state (state_t): current state of core (fsm)
* @param task_list (task_struct_t*): pointers to task functions in array format
*/
typedef struct core_t{
    state_t state;
    task_struct_t task_list[NUM_TASKS];
}core_t;


#endif