#ifndef _AUDIO_BUFFER_H_
#define _AUDIO_BUFFER_H_

/* Audio IO and storage */

#include "Arduino.h"
#include "bridge.h"

typedef struct sample{
    uint16_t l = 0;
    uint16_t r = 0;
}sample_t;

typedef struct buffer{
    sample_t sample;
}buffer_t;

typedef struct buffer_handle{
    uint32_t len = 0;
    uint32_t start = 0;
    uint32_t stop = 0;
    buffer_t* mem = NULL;
}buffer_handle_t;

/* Buffer initializer
* PARAMS
* `buffer_handle_t* h_buf`: buffer handle describing buffer
* `buffer_t* p_mem`: pointer to static memory buffer
* RETURNS
* `err_t` status, `e_no_err` if OK
 */
err_t initBuf(buffer_handle_t* h_buf, buffer_t* p_mem);

#endif