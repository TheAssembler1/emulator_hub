#ifndef MEM_H
#define MEM_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "lwlog/lwlog.h"
#define HOST_ENDIANESS_IS_LITTLE(n) (*(char*)&n == 1)

typedef enum {
    LITTLE_ENDIANESS, 
    BIG_ENDIANESS 
} Endianess;


typedef struct {
    uint8_t* buf;
    Endianess end;
} Memory_Buffer;

Memory_Buffer* memory_buffer_init(uint64_t mem_size, Endianess end);
void memory_buffer_destroy(Memory_Buffer* mem_buf);

void memory_write_uint8_t(Memory_Buffer* mem_buf, uint64_t addr, uint8_t data);
void memory_write_uint16_t(Memory_Buffer* mem_buf, uint64_t addr, uint16_t data);
void memory_write_uint32_t(Memory_Buffer* mem_buf, uint64_t addr, uint32_t data);
void memory_write_uint64_t(Memory_Buffer* mem_buf, uint64_t addr, uint64_t data);

uint8_t memory_read_uint8_t(Memory_Buffer* mem_buf, uint64_t addr);
uint16_t memory_read_uint16_t(Memory_Buffer* mem_buf, uint64_t addr);
uint32_t memory_read_uint32_t(Memory_Buffer* mem_buf, uint64_t addr);
uint64_t memory_read_uint64_t(Memory_Buffer* mem_buf, uint64_t addr);

#endif
