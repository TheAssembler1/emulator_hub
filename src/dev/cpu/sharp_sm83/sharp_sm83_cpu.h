#ifndef SHARP_SM83_CPU_H
#define SHARP_SM83_CPU_H

#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>

#include "lwlog/lwlog.h"
#include "mem/mem.h"

enum Sharp_SM83_Mut_Flag {
    SET_FLAG,
    CLEAR_FLAG,
    MASK_FLAG,
};

struct Sharp_SM83_CPU_Registers{
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
};

struct Sharp_SM83_CPU {
    bool running;
    struct Sharp_SM83_CPU_Registers regs;
    struct Memory_Buffer* mem_buf;
};

void sharp_sm83_cpu_init(struct Sharp_SM83_CPU* cpu, struct Memory_Buffer* mem_buf);
void sharp_sm83_cpu_destroy(struct Sharp_SM83_CPU* cpu);
void sharp_sm83_cpu_cycle(struct Sharp_SM83_CPU* cpu, struct Memory_Buffer* mem_buf, uint64_t cycles);
bool sharp_sm83_fetch_opcode(struct Sharp_SM83_CPU* cpu, struct Memory_Buffer* mem_buf, uint8_t* fetched_opcode);
uint64_t sharp_sm83_exec_opcode(struct Sharp_SM83_CPU* cpu, struct Memory_Buffer* mem_buf, uint8_t fetched_opcode, bool cb);

#endif
