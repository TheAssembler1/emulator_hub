#include "mem.h"

void memory_buffer_init(struct Memory_Buffer* mem_buf, uint64_t mem_size, enum Endianess end) {
    if(mem_size < 0) {
        lwlog_emerg("invalid mem_size: %lu", mem_size);
        return;
    }

    mem_buf->buf = (uint8_t*)malloc(mem_size);
    memset(mem_buf->buf, 0, mem_size);
    mem_buf->end = end;
}

void memory_buffer_destroy(struct Memory_Buffer* mem_buf) {
    free(mem_buf->buf);
}

static bool memory_buffer_is_native(enum Endianess mem_buf_end) {
    enum Endianess my_end; 
    int n = 1; 
    
    if(HOST_ENDIANESS_IS_LITTLE(n)) { 
        my_end = LITTLE_ENDIANESS; 
    } else { 
        my_end = BIG_ENDIANESS; 
    } 

    if(my_end == mem_buf_end) { 
        return true;
    } 

    return false;
}

static void swap_bytes(uint32_t data_num_bytes, uint8_t* data) {
    if(data_num_bytes > 8 || data_num_bytes < 2) {
        lwlog_err("invalid data_num_bytes: %d", data_num_bytes);
    }

    uint8_t* start_byte = data;
    uint8_t* end_byte = &(data[data_num_bytes - 1]);

    while(start_byte <= end_byte) {
        uint8_t temp = *start_byte;
        *start_byte = *end_byte;
        *end_byte = temp;

        start_byte++;
        end_byte--;
    }
}

void memory_write_uint8_t(struct Memory_Buffer* mem_buf, uint64_t addr, uint8_t data) {
    mem_buf->end, mem_buf->buf[addr] = data;
}

void memory_write_uint16_t(struct Memory_Buffer* mem_buf, uint64_t addr, uint16_t data) {
    if(!memory_buffer_is_native(mem_buf->end)) {
       swap_bytes(sizeof(uint16_t), (uint8_t*)&data); 
    }

    ((uint16_t*)mem_buf->buf)[addr] = data;
}

void memory_write_uint32_t(struct Memory_Buffer* mem_buf, uint64_t addr, uint32_t data) {
    if(!memory_buffer_is_native(mem_buf->end)) {
       swap_bytes(sizeof(uint32_t), (uint8_t*)&data); 
    }


    ((uint32_t*)mem_buf->buf)[addr] = data;
}

void memory_write_uint64_t(struct Memory_Buffer* mem_buf, uint64_t addr, uint64_t data) {
    if(!memory_buffer_is_native(mem_buf->end)) {
       swap_bytes(sizeof(uint64_t), (uint8_t*)&data); 
    }

    ((uint64_t*)mem_buf->buf)[addr] = data;
}

uint8_t memory_read_uint8_t(struct Memory_Buffer* mem_buf, uint64_t addr) {
    return mem_buf->buf[addr];
}

uint16_t memory_read_uint16_t(struct Memory_Buffer* mem_buf, uint64_t addr) {
    return ((uint16_t*)mem_buf->buf)[addr];
}

uint32_t memory_read_uint32_t(struct Memory_Buffer* mem_buf, uint64_t addr) {
    return ((uint32_t*)mem_buf->buf)[addr];
}

uint64_t memory_read_uint64_t(struct Memory_Buffer* mem_buf, uint64_t addr) {
    return ((uint64_t*)mem_buf->buf)[addr];
}
