#ifndef SHARP_SM83_CPU_H
#define SHARP_SM83_CPU_H

#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>

#include "lwlog/lwlog.h"
#include "mem/mem.h"

typedef enum {
    SET_FLAG,
    CLEAR_FLAG,
    MASK_FLAG,
} Sharp_SM83_Mut_Flag;

typedef struct {
    union{
        struct{
            uint8_t f;
            uint8_t a;
        };
        uint16_t af;
    };
    
    union{
        struct{
            uint8_t c;
            uint8_t b;
        };
        uint16_t bc;
    };

    union{
        struct{
            uint8_t e;
            uint8_t d;
        };
        uint16_t de;
    };

    union{
        struct{
            uint8_t l;
            uint8_t h;
        };
        uint16_t hl;
    };

    uint16_t sp;
    uint16_t pc;
} Sharp_SM83_CPU_Registers;

typedef struct {
    bool running;
    Sharp_SM83_CPU_Registers regs;
    Memory_Buffer* mem_buf;
} Sharp_SM83_CPU;

Sharp_SM83_CPU* sharp_sm83_cpu_init(Memory_Buffer* mem_buf);
void sharp_sm83_cpu_destroy(Sharp_SM83_CPU* cpu);
void sharp_sm83_cpu_cycle(Sharp_SM83_CPU* cpu, Memory_Buffer* mem_buf, uint64_t cycles);
bool sharp_sm83_fetch_opcode(Sharp_SM83_CPU* cpu, Memory_Buffer* mem_buf, uint8_t* fetched_opcode);
uint64_t sharp_sm83_exec_opcode(Sharp_SM83_CPU* cpu, Memory_Buffer* mem_buf, uint8_t fetched_opcode, bool cb);

#endif
