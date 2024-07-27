#include "sharp_sm83_cpu.h"

void sharp_sm83_cpu_init(struct Sharp_SM83_CPU* cpu, struct Memory_Buffer* mem_buf) {
    cpu->running = false;
	memset(&(cpu->regs), 0, sizeof(struct Sharp_SM83_CPU_Registers));
	cpu->mem_buf = mem_buf;
}

void sharp_sm83_cpu_destroy(struct Sharp_SM83_CPU* cpu) {}

void sharp_sm83_cpu_cycle(struct Sharp_SM83_CPU* cpu, struct Memory_Buffer* mem_buf, uint64_t cycles) {
    while(cpu->running && cycles > 0) {
        lwlog_info("cpu pc register: 0x%x", cpu->regs.pc);

        uint8_t fetched_opcode;
        bool cb = sharp_sm83_fetch_opcode(cpu, mem_buf, &fetched_opcode);

        lwlog_info("fetched cpu opcode: 0x%x", fetched_opcode);

        cycles -= sharp_sm83_exec_opcode(cpu, fetched_opcode, cb);
    }
}

bool sharp_sm83_fetch_opcode(struct Sharp_SM83_CPU* cpu, struct Memory_Buffer* mem_buf, uint8_t* fetched_opcode) {
    uint8_t cur_opcode = mem_buf->buf[cpu->regs.pc];
    if(cur_opcode == 0xCB) {
        cpu->regs.pc++;
        *fetched_opcode = mem_buf->buf[cpu->regs.pc + 1];
        return true;
    }

    *fetched_opcode = cur_opcode;
    return false;
}

uint64_t sharp_sm83_exec_opcode(struct Sharp_SM83_CPU* cpu, uint8_t fetched_opcode, bool cb) {
    if(!cb) {
        switch(fetched_opcode) {
            default:
                lwlog_err("unimplemented opcode: 0x%x", fetched_opcode);
                cpu->running = false;
                return 0;
        }
    }

    switch(fetched_opcode) {
        default:
            lwlog_err("cb unimplemented opcode: 0x%x", fetched_opcode);
            cpu->running = false;
            return 0;
    }
}
