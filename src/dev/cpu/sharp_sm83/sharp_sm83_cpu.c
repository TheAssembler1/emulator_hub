#include "sharp_sm83_cpu.h"

void sharp_sm83_cpu_init(struct Sharp_SM83_CPU* cpu, struct Memory_Buffer* mem_buf) {
	memset(&(cpu->regs), 0, sizeof(struct Sharp_SM83_CPU_Registers));
	cpu->mem_buf = mem_buf;
}

void sharp_sm83_cpu_destroy(struct Sharp_SM83_CPU* cpu) {}


void sharp_sm83_cpu_cycle(struct Sharp_SM83_CPU* cpu, uint64_t cycles) {
    while(cycles > 0) {
        uint16_t fetched_opcode;
        bool cb = sharp_sm83_fetch_opcode(cpu, &fetched_opcode); 
        cycles -= sharp_sm83_exec_opcode(cpu, fetched_opcode, cb);
    }
}


bool sharp_sm83_fetch_opcode(struct Sharp_SM83_CPU* cpu, uint16_t* fetched_opcode) {
    return false;
}

uint64_t sharp_sm83_exec_opcode(struct Sharp_SM83_CPU* cpu, uint16_t fetched_opcode, bool cb) {
    if(!cb) {
        switch(fetched_opcode) {
            default:
                lwlog_err("unimplemented opcode: %x", fetched_opcode);
                return 0;
        }
    }

    
    switch(fetched_opcode) {
        default:
            lwlog_err("cb unimplemented opcode: %x", fetched_opcode);
            return 0;
    }
}
