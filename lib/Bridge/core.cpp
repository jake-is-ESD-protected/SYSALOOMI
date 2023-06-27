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
                (void*)pt,
                pt->priority,
                &pt->handle);
    if(stat != pdPASS){
        return e_mem_null;
    }
    pt->running = true;
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

    core.state = e_state_synthesizer; // default job: this will change
    // xTaskNotifyGive(core_get_cur_task_handle());

    return e_no_err;
}

task_struct_t* core_get_task_handle(task_t t){
    return &core.task_list[t];
}

void task_notify(task_struct_t* t, cmd_struct_t c, bool from_isr){
    if(from_isr){
        BaseType_t dummy = pdFALSE;
        c.origin = e_origin_interrupt;
        xTaskNotifyFromISR(t->handle, 
                           *(uint32_t*)&c, 
                           eSetValueWithOverwrite, 
                           &dummy);
    }
    else{
        xTaskNotify(t->handle, *(uint32_t*)&c, eSetValueWithOverwrite);
    }
}

cmd_struct_t task_notify_take(TickType_t ticks_to_wait){
    uint32_t c = ulTaskNotifyTake(pdTRUE, ticks_to_wait);
    return *(cmd_struct_t*)&c;
}

cmd_struct_t sleep_until_notified(){
    return task_notify_take(portMAX_DELAY);
}

void task_synthesizer(void* p){

}

void task_sampler(void* p){
    task_struct_t* pt = (task_struct_t*)p;
    while(1){
        task_notify_take(portMAX_DELAY);
    }
    
}

void task_looper(void* p){

}

void task_midicontroller(void* p){

}


void task_metronome(void* p){

}

void task_parser(void* p){

}