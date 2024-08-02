#include "mem.h"

Memory_Buffer* memory_buffer_init(uint64_t mem_size, Endianess end) {
    Memory_Buffer* mem_buf = (Memory_Buffer*)calloc(sizeof(Memory_Buffer), 1);
    mem_buf->buf = (uint8_t*)calloc(mem_size, sizeof(uint8_t));
    memset(mem_buf->buf, 0, mem_size);
    mem_buf->end = end;

    return mem_buf;
}

void memory_buffer_destroy(Memory_Buffer* mem_buf) {
    free(mem_buf->buf);
    free(mem_buf);
}

void memory_write_uint8_t(Memory_Buffer* mem_buf, uint64_t addr, uint8_t data) {
    mem_buf->buf[addr] = data;
}

void memory_write_uint16_t(Memory_Buffer* mem_buf, uint64_t addr, uint16_t data) {
    if (mem_buf->end == LITTLE_ENDIANESS) {
        mem_buf->buf[addr] = (uint8_t)(data & 0x00FF);
        mem_buf->buf[addr + 1] = (uint8_t)((data & 0xFF00) >> 8);
    } else {
        // Write in big-endian order
        mem_buf->buf[addr + 1] = (uint8_t)(data & 0x00FF);
        mem_buf->buf[addr] = (uint8_t)((data & 0xFF00) >> 8);
    }
}

uint8_t memory_read_uint8_t(Memory_Buffer* mem_buf, uint64_t addr) {
    return mem_buf->buf[addr];
}

uint16_t memory_read_uint16_t(Memory_Buffer* mem_buf, uint64_t addr) {
    if (mem_buf->end == LITTLE_ENDIANESS) {
        return (uint16_t)(mem_buf->buf[addr]) | ((uint16_t)(mem_buf->buf[addr + 1]) << 8);
    } else {
        return ((uint16_t)(mem_buf->buf[addr]) << 8) | (uint16_t)(mem_buf->buf[addr + 1]);
    }
}
