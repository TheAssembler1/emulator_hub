#ifndef MEM_H
#define MEM_H

#include <stdlib.h>
#include <stdint.h>

#include "../lwlog/lwlog.h"

enum endianess {
    LITTLE_ENDIANESS, 
    BIG_ENDIANESS 
};


struct memory_buffer {
    uint8_t* buf;
    enum endianess end;
};

void memory_buffer_init(struct memory_buffer* mem_buf, uint64_t mem_size, enum endianess end);
void memory_buffer_destroy(struct memory_buffer* mem_buf);

#endif
