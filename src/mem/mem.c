#include "mem.h"

void memory_buffer_init(struct memory_buffer* mem_buf, uint64_t mem_size, enum endianess end) {
    if(mem_size < 0) {
        lwlog_emerg("invalid mem_size: %lu\n", mem_size);
        return;
    }

    mem_buf->buf = (uint8_t*)malloc(mem_size);
    mem_buf->end = end;
}

void memory_buffer_destroy(struct memory_buffer* mem_buf) {
    free(mem_buf->buf);
}
