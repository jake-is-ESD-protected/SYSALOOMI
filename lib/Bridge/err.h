#ifndef _ERR_H_
#define _ERR_H_

/* Error interpreter and handling */

#define INFO    0
#define WARNING 1
#define ERROR   2
#define SEVERE  3

typedef enum err{
    e_no_err,
    e_mem_null,
    e_is_zero,
    e_param,
    e_peripheral_block,
    e_core_fail,
}err_t;

/* General error handler
* PARAMS
`err_t e`: Error to handle
`void* args`: additional arguments
*/
void errHandler(err_t e, void* args);

#endif
