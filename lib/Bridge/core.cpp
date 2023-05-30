#include "core.h"
#include "STM32FreeRTOS.h"

core_t core;

TaskHandle_t h_task_list[NUM_TASKS] = {NULL};

void set_task_config(task_struct_t* pt, 
                     task_t t, 
                     TaskHandle_t h,
                     uint32_t m,
                     uint8_t p, 
                     void (*f)(void* p)){
    pt->type = t;
    pt->handle = h;
    pt->mem_size = m;
    pt->priority = p;
    pt->function = f;
}

void create_task(task_struct_t* pt){
    xTaskCreate(pt->function,
                (char*)pt->type,
                pt->mem_size,
                NULL,
                pt->priority,
                &pt->handle);
}

void core_init(){
    core.state = e_state_init;

    for(uint8_t i = 0; i < NUM_TASKS; i++){
        set_task_config(&core.task_list[i],
                        (task_t)i, 
                        h_task_list[i],
                        2048,
                        1,
                        function_list[i]);
        // create_task(&core.task_list[i]);
    }
}