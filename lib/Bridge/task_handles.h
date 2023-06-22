#ifndef _TASK_HANDLES_H_
#define _TASK_HANDLES_H_

/* Expose task handles to all submodules
*/

#include "Arduino.h"
#include "STM32FreeRTOS.h"
#include "core.h"

// defined in core.cpp
extern TaskHandle_t h_task_list[NUM_TASKS];

#endif