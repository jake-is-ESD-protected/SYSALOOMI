#ifndef _AUDIO_BUFFER_H_
#define _AUDIO_BUFFER_H_

/*@file
* @brief Audio IO and storage */

#include "Arduino.h"
#include "bridge.h"

#define AUDIO_RES_BIT 16
#define BIT_DEPTH AUDIO_RES_BIT

/*@brief Stereo sample struct
* @param l (uint16_t): left channel sample
* @param r (uint16_t): right channel sample
* @note The whole audio system is limited to 16 bit audio 
*/
typedef struct sample{
    uint16_t l = 0;
    uint16_t r = 0;
}sample_t;

/*@brief Buffer struct
* @param sample (sample_t): audio sample in memory
* @note This has to used as array or this doesn't make sense
*/
typedef struct buffer{
    sample_t sample;
}buffer_t;

/*@brief Buffer handle struct
* @param len (uint32_t): length of buffer
* @param start (uint32_t): current start position within buffer
* @param stop (uint32_t): current stop position within buffer
* @param mem (buffer_t*): memory array, has to be buffer_t
*/
typedef struct buffer_handle{
    uint32_t len = 0;
    uint32_t start = 0;
    uint32_t stop = 0;
    buffer_t* mem = NULL;
}buffer_handle_t;

/*@brief Buffer initializer
* @param h_buf (buffer_handle_t*): buffer handle describing buffer 
* @param p_mem (buffer_t*): pointer to static memory buffer
* @returns status (err_t)
* `err_t` status, `e_no_err` if OK
 */
err_t initBuf(buffer_handle_t* h_buf, buffer_t* p_mem);

#endif