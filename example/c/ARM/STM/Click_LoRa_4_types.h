#ifndef _LORA4_T_
#define _LORA4_T_

#include "stdint.h"

#ifndef _LORA4_H_

#define T_LORA4_P const uint8_t* 
#define LORA4_RETVAL_T      uint8_t

typedef struct {

    uint8_t command_;
    uint8_t payLoadSize;
    uint8_t payLoad[ 255 ];

}T_lora4_message;

#endif
#endif