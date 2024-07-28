#include "mem.h"

void memory_buffer_init(struct Memory_Buffer* mem_buf, uint64_t mem_size, enum Endianess end) {
    mem_buf->buf = (uint8_t*)malloc(mem_size);
    memset(mem_buf->buf, 0, mem_size);
    mem_buf->end = end;
}

void memory_buffer_destroy(struct Memory_Buffer* mem_buf) {
    free(mem_buf->buf);
}

void memory_write_uint8_t(struct Memory_Buffer* mem_buf, uint64_t addr, uint8_t data) {
    mem_buf->buf[addr] = data;
}

void memory_write_uint16_t(struct Memory_Buffer* mem_buf, uint64_t addr, uint16_t data) {
    if (mem_buf->end == LITTLE_ENDIANESS) {
        // Write in little-endian order
        mem_buf->buf[addr] = (uint8_t)(data & 0x00FF);        // Lower 8 bits
        mem_buf->buf[addr + 1] = (uint8_t)((data & 0xFF00) >> 8); // Upper 8 bits
    } else {
        // Write in big-endian order
        mem_buf->buf[addr + 1] = (uint8_t)(data & 0x00FF);     // Lower 8 bits
        mem_buf->buf[addr] = (uint8_t)((data & 0xFF00) >> 8);  // Upper 8 bits
    }
}



uint8_t memory_read_uint8_t(struct Memory_Buffer* mem_buf, uint64_t addr) {
    return mem_buf->buf[addr];
}

uint16_t memory_read_uint16_t(struct Memory_Buffer* mem_buf, uint64_t addr) {
    if (mem_buf->end == LITTLE_ENDIANESS) {
        return (uint16_t)(mem_buf->buf[addr]) | ((uint16_t)(mem_buf->buf[addr + 1]) << 8);
    } else {
        return ((uint16_t)(mem_buf->buf[addr]) << 8) | (uint16_t)(mem_buf->buf[addr + 1]);
    }
}
