#include "core.h"
#include "STM32FreeRTOS.h"

core_t core;

TaskHandle_t h_task_list[NUM_TASKS] = {NULL};

err_t set_task_config(task_struct_t* pt, 
                     task_t t, 
                     TaskHandle_t h,
                     uint32_t m,
                     uint8_t p, 
                     void (*f)(void* p)){
    
    if(pt == NULL || f == NULL){
        return e_is_zero;
    }
    if(m = 0 || p == 0){
        return e_is_zero;
    }

    pt->type = t;
    pt->handle = h;
    pt->mem_size = m;
    pt->priority = p;
    pt->function = f;

    return e_no_err;
}

err_t create_task(task_struct_t* pt){
    BaseType_t stat;
    stat = xTaskCreate(pt->function,
                (char*)pt->type,
                pt->mem_size,
                NULL,
                pt->priority,
                &pt->handle);
    if(stat != pdPASS){
        return e_mem_null;
    }
    return e_no_err;
}

err_t core_init(){
    core.state = e_state_init;

    err_t stat;

    for(uint8_t i = 0; i < NUM_TASKS; i++){
        stat = set_task_config(&core.task_list[i],
                        (task_t)i, 
                        h_task_list[i],
                        2048,
                        1,
                        function_list[i]);
        if(stat != e_no_err){
            return stat;
        }
        stat = create_task(&core.task_list[i]);
        if(stat != e_no_err){
            return stat;
        }
    }

    return e_no_err;
}

void task_sampler(void* p){
    
}

void task_metronome(void* p){

}

void task_parser(void* p){

}