#include "audio_buffer.h"

err_t initBuf(buffer_handle_t* h_buf, buffer_t* p_mem){
    if(h_buf == NULL || p_mem == NULL){
        return e_mem_null;
    }
    if(h_buf->len == 0){
        return e_is_zero;
    }

    h_buf->stop = h_buf->len - 1;
    h_buf->mem = p_mem;
    return e_no_err;
}