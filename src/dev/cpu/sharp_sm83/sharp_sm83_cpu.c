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

        cycles -= sharp_sm83_exec_opcode(cpu, mem_buf, fetched_opcode, cb);
    }
}

bool sharp_sm83_fetch_opcode(struct Sharp_SM83_CPU* cpu, struct Memory_Buffer* mem_buf, uint8_t* fetched_opcode) {
    uint8_t cur_opcode = mem_buf->buf[cpu->regs.pc];
    if(cur_opcode == 0xCB) {
        *fetched_opcode = mem_buf->buf[cpu->regs.pc + 1];
        return true;
    }

    *fetched_opcode = cur_opcode;
    return false;
}

#define SHARP_SM83_CPU_ZERO_FLAG  0b10000000
#define SHARP_SM83_CPU_SUB_FLAG   0b01000000
#define SHARP_SM83_CPU_HFC_FLAG   0b00100000
#define SHARP_SM83_CPU_CARRY_FLAG 0b00010000

static void sharp_sm83_set_flags(struct Sharp_SM83_CPU* cpu,
        enum Sharp_SM83_Mut_Flag z_flag, enum Sharp_SM83_Mut_Flag sub_flag,
        enum Sharp_SM83_Mut_Flag hfc_flag, enum Sharp_SM83_Mut_Flag carry_flag) {

    switch(z_flag) {
        case SET_FLAG:
            cpu->regs.f |= SHARP_SM83_CPU_ZERO_FLAG;
            break;
        case CLEAR_FLAG:
            cpu->regs.f &= ~(SHARP_SM83_CPU_ZERO_FLAG);
            break;
        case MASK_FLAG: break;
        default:
            lwlog_err("invalid set_flag value");
            break;
    }

    switch(sub_flag) {
        case SET_FLAG:
            cpu->regs.f |= SHARP_SM83_CPU_SUB_FLAG;
            break;
        case CLEAR_FLAG:
            cpu->regs.f &= ~(SHARP_SM83_CPU_SUB_FLAG);
            break;
        case MASK_FLAG: break;
        default:
            lwlog_err("invalid set_flag value");
            break;
    }

    switch(hfc_flag) {
        case SET_FLAG:
            cpu->regs.f |= SHARP_SM83_CPU_HFC_FLAG;
            break;
        case CLEAR_FLAG:
            cpu->regs.f &= ~(SHARP_SM83_CPU_HFC_FLAG);
            break;
        case MASK_FLAG: break;
        default:
            lwlog_err("invalid set_flag value");
            break;
    }

    switch(carry_flag) {
        case SET_FLAG:
            cpu->regs.f |= SHARP_SM83_CPU_CARRY_FLAG;
            break;
        case CLEAR_FLAG:
            cpu->regs.f &= ~(SHARP_SM83_CPU_CARRY_FLAG);
            break;
        case MASK_FLAG: break;
        default:
            lwlog_err("invalid set_flag value");
            break;
    }
}

#define HALF_CARRY_UINT8_T_CHECK(op, a, b) \
    (((((a) & 0xF) op ((b) & 0xF)) & 0x10) != 0)
#define HALF_CARRY_UINT16_T_CHECK(op, a, b) \
    (((((a) & 0xFF) op ((b) & 0xFF)) & 0x100 != 0)
#define CARRY_UINT8_T_OVERFLOW_CHECK(op, a, b) \
    (((uint16_t)(a) op (b)) > 0xFF)
#define CARRY_UINT8_T_UNDERFLOW_CHECK(op, a, b) \
    (((int16_t)(a) op (b)) < 0)

static bool carry_add_uint8_t(uint8_t a, uint8_t b) {
    return (uint16_t)(a + b) > 0xFF;
}

static bool carry_sub_uint8_t(uint8_t a, uint8_t b) {
    return (int16_t)(a - b) < 0;
}

static bool carry_add_uint16_t(uint16_t a, uint16_t b) {
    return (uint32_t)(a + b) > 0xFFFF;
}

static bool carry_sub_uint16_t(uint16_t a, uint16_t b) {
    return (int32_t)(a - b) < 0;
}

uint64_t sharp_sm83_exec_opcode(struct Sharp_SM83_CPU* cpu, struct Memory_Buffer* mem_buf, uint8_t fetched_opcode, bool cb) {
    enum Sharp_SM83_Mut_Flag z_flag = MASK_FLAG;
    enum Sharp_SM83_Mut_Flag sub_flag = MASK_FLAG;
    enum Sharp_SM83_Mut_Flag hfc_flag = MASK_FLAG;
    enum Sharp_SM83_Mut_Flag c_flag = MASK_FLAG;
    uint8_t cycles = 0;
    uint8_t pc_inc = 0;

    if(!cb) {
        switch(fetched_opcode) {
            case 0x00: // NOTE: NOP | 1 | 4 | ----
                pc_inc = 1; cycles = 4;
                break;
            case 0x02: // NOTE: LD [BC], A | 1, 8 | ----
                memory_write_uint8_t(mem_buf, cpu->regs.bc, cpu->regs.a);
                pc_inc = 1; cycles = 8;
                break;
            case 0x05: // NOTE: DEC B | 1 | 4 | Z1H-
                hfc_flag = HALF_CARRY_UINT8_T_CHECK(-, cpu->regs.b, 1) ? SET_FLAG : CLEAR_FLAG;
                z_flag = (--(cpu->regs.b)) ? CLEAR_FLAG : SET_FLAG;
                sub_flag = SET_FLAG;
                pc_inc = 1; cycles = 4;
                break;
            case 0x06: // NOTE: LD B, n8 | 2 | 8 | ----
                cpu->regs.b = memory_read_uint8_t(mem_buf, cpu->regs.pc + 1);
                pc_inc = 2; cycles = 8;
                break;
            case 0x0C: // NOTE: INC C | 1 | 4 | Z0H-
                hfc_flag = HALF_CARRY_UINT8_T_CHECK(+, cpu->regs.c, 1) ? SET_FLAG : CLEAR_FLAG;
                cpu->regs.c++;
                z_flag = (!cpu->regs.c) ? SET_FLAG : CLEAR_FLAG;
                sub_flag = CLEAR_FLAG;
                pc_inc = 1; cycles = 4;
                break;
            case 0x0E: // NOTE:  LD C, n8 | 2 | 8 | ----
                cpu->regs.c = memory_read_uint8_t(mem_buf, cpu->regs.pc + 1);
                pc_inc = 2; cycles = 8;
                break;
            case 0x11: // NOTE: LD DE, n16 | 3 | 12 | ----
                cpu->regs.de = memory_read_uint16_t(mem_buf, cpu->regs.pc + 1);
                pc_inc = 3; cycles = 12;
                break;
            case 0x13: // NOTE: INC DE | 1 | 8 | ----
                cpu->regs.de++;
                pc_inc = 1; cycles = 8;
                break;
            case 0x17: // NOTE: RLA | 1 | 4 | 000C
                c_flag = (CARRY_UINT8_T_OVERFLOW_CHECK(<<, cpu->regs.a, 1)) ? SET_FLAG : CLEAR_FLAG;
                cpu->regs.a = (cpu->regs.a << 1) | (cpu->regs.a >> (8 - 1));
                z_flag = CLEAR_FLAG; sub_flag = CLEAR_FLAG; hfc_flag = CLEAR_FLAG;
                pc_inc = 1; cycles = 4;
                break;
            case 0x1A: // NOTE:  LD A, [DE] | 1 | 8 | ----
                cpu->regs.a = memory_read_uint8_t(mem_buf, cpu->regs.de);
                pc_inc = 1; cycles = 8;
                break;
            case 0x20: // NOTE: JR NZ, e8 | 2 | 12 / 8 | ----
                if(cpu->regs.f & SHARP_SM83_CPU_ZERO_FLAG) {
                    cycles = 8;
                } else {
                    cpu->regs.pc += (int8_t)memory_read_uint8_t(mem_buf, cpu->regs.pc + 1);
                    cycles = 12;
                }

                pc_inc = 2;
                break;
            case 0x21: // NOTE: LD HL, n16 | 3 | 12 | ----
                cpu->regs.hl = memory_read_uint16_t(mem_buf, memory_read_uint16_t(mem_buf, cpu->regs.pc + 1));
                pc_inc = 3; cycles = 12;
                break;
            case 0x22: // NOTE: LD [HL+], A | 1 | 8 | ----
                memory_write_uint8_t(mem_buf, cpu->regs.hl++, cpu->regs.a);
                pc_inc = 1; cycles = 8;
                break;
            case 0x23: // NOTE: INC HL | 1 | 8 | ----
                cpu->regs.hl++;
                pc_inc = 1; cycles = 8;
                break;
            case 0x28: // NOTE: JR Z, e8 | 2 | 12/8 | ----
                if(cpu->regs.f & SHARP_SM83_CPU_ZERO_FLAG) {
                    cpu->regs.pc += (int8_t)memory_read_uint8_t(mem_buf, cpu->regs.pc + 1);
                    cycles = 12;
                } else {
                    cycles = 8;
                }

                pc_inc = 2;
                break;
            case 0x31: // NOTE: LD SP, n16 | 3 | 12
                cpu->regs.sp = memory_read_uint16_t(mem_buf, memory_read_uint16_t(mem_buf, cpu->regs.pc + 1));
                pc_inc = 3; cycles = 12;
                break;
            case 0x32: // NOTE: LD [HL-], A | 1 | 8 | ----
                memory_write_uint8_t(mem_buf, cpu->regs.hl--, cpu->regs.a);
                pc_inc = 1; cycles = 8;
                break;
            case 0x3D: // NOTE: DEC A | 1 | 4 | Z1H-
                hfc_flag = HALF_CARRY_UINT8_T_CHECK(-, cpu->regs.a, 1) ? SET_FLAG : CLEAR_FLAG;
                z_flag = (--cpu->regs.a) ? CLEAR_FLAG : SET_FLAG;
                sub_flag = SET_FLAG;
                pc_inc = 1; cycles = 4;
                break;
            case 0x3E: // NOTE: LD A, n8 | 2 | 8 | ----
                cpu->regs.a = memory_read_uint8_t(mem_buf, cpu->regs.pc + 1);
                pc_inc = 2; cycles = 8;
                break;
            case 0x4F: // NOTE: LD C, A | 1 | 4 | ----
                cpu->regs.c = cpu->regs.a;
                pc_inc = 1; cycles = 4;
                break;
            case 0x66: // NOTE:  LD H, [HL] | 1 | 8 | ----
                cpu->regs.h = memory_read_uint8_t(mem_buf, cpu->regs.hl);
                pc_inc = 1; cycles = 8;
                break;
            case 0x77: // NOTE: LD [HL], A | 1 | 8 | ----
                memory_write_uint8_t(mem_buf, cpu->regs.hl, cpu->regs.a);
                pc_inc = 1; cycles = 8;
                break;
            case 0x7B: // NOTE: LD A, E | 1 | 4 | ----
                cpu->regs.a = cpu->regs.e;
                pc_inc = 1; cycles = 4;
                break;
            case 0xAF: // NOTE: XOR A, A | 1 | 4 | 1000
                cpu->regs.a = cpu->regs.a ^ cpu->regs.a;
                z_flag = SET_FLAG; sub_flag = CLEAR_FLAG; hfc_flag = CLEAR_FLAG; c_flag = CLEAR_FLAG;
                pc_inc = 1; cycles = 4;
                break;
            case 0xC1: // NOTE:  POP BC | 1 | 12 | ----
                cpu->regs.bc = memory_read_uint16_t(mem_buf, cpu->regs.sp);
                cpu->regs.sp += 2;
                pc_inc = 1; cycles = 12;
                break;
            case 0xC5: //NOTE: PUSH BC | 1 | 16 | ----
                cpu->regs.sp -= 2;
                memory_write_uint16_t(mem_buf, cpu->regs.sp, cpu->regs.bc);
                pc_inc = 1; cycles = 16;
                break;
            case 0xC9: // NOTE:  RET | 1 | 16 | ----
                cpu->regs.pc = memory_read_uint16_t(mem_buf, cpu->regs.sp);
                cpu->regs.sp += 2;
                cycles = 16;
                break;
            case 0xCC: // NOTE: CALL Z, a16 | 3 | 24/12 | ----
                if(cpu->regs.f & SHARP_SM83_CPU_ZERO_FLAG) {
                    cpu->regs.sp -= 2;
                    memory_write_uint16_t(mem_buf, cpu->regs.sp, cpu->regs.pc + 3);
                    cpu->regs.pc = memory_read_uint16_t(mem_buf, cpu->regs.pc + 1);
                    cycles = 24;
                } else {
                    pc_inc = 3; cycles = 12;
                }

                break;
            case 0xCD: // NOTE: CALL a16 | 3 | 24 ----
                cpu->regs.sp -= 2;
                memory_write_uint16_t(mem_buf, cpu->regs.sp, cpu->regs.pc + 3);
                cpu->regs.pc = memory_read_uint16_t(mem_buf, cpu->regs.pc + 1);
                cycles = 24;
                break;
            case 0xCE: // NOTE: ADC A, n8 | 2 | 8 | Z0HC
                hfc_flag = HALF_CARRY_UINT8_T_CHECK(+, cpu->regs.a, (memory_read_uint8_t(mem_buf, cpu->regs.pc) + 1 + (cpu->regs.f & SHARP_SM83_CPU_CARRY_FLAG))) ? SET_FLAG : CLEAR_FLAG;
                cpu->regs.a = memory_read_uint8_t(mem_buf, cpu->regs.pc + 1) + ((cpu->regs.f & SHARP_SM83_CPU_CARRY_FLAG) ? 1 : 0);
                c_flag = CARRY_UINT8_T_OVERFLOW_CHECK(+, cpu->regs.a, (memory_read_uint8_t(mem_buf, cpu->regs.pc) + 1 + (cpu->regs.f & SHARP_SM83_CPU_CARRY_FLAG))) ? SET_FLAG : CLEAR_FLAG;
                z_flag = (cpu->regs.a) ? CLEAR_FLAG : SET_FLAG;
                sub_flag = SET_FLAG;
                pc_inc = 2; cycles = 8;
                break;
            case 0xE0: // NOTE: LDH [a8], A | 2 | 12 | ----
                memory_write_uint8_t(mem_buf, 0xFF00 | memory_read_uint8_t(mem_buf, cpu->regs.pc + 1), cpu->regs.a);
                pc_inc = 2; cycles = 12;
                break;
            case 0xE2: // NOTE: LD [C], A | 1 | 8 | ----
                memory_write_uint8_t(mem_buf, cpu->regs.c, cpu->regs.a);
                pc_inc = 1; cycles = 8;
                break;
            case 0xEA: // NOTE:D [a16], A | 3 | 16 | ----
                memory_write_uint8_t(mem_buf, memory_read_uint16_t(mem_buf, cpu->regs.pc + 1), cpu->regs.a);
                pc_inc = 3; cycles = 16;
                break;
            case 0xF9: // NOTE: LD SP, HL | 1 | 8 | ----
                cpu->regs.sp = cpu->regs.hl;
                pc_inc = 1; cycles = 8;
                break;
            case 0xFE: // NOTE:  CP A, n8 | 2 | 8 | Z1HC
                hfc_flag = (HALF_CARRY_UINT8_T_CHECK(-, cpu->regs.a, memory_read_uint8_t(mem_buf, cpu->regs.pc + 1))) ? SET_FLAG : CLEAR_FLAG;
                z_flag = (cpu->regs.a - memory_read_uint8_t(mem_buf, cpu->regs.pc + 1)) ? CLEAR_FLAG : SET_FLAG;
                c_flag = (CARRY_UINT8_T_UNDERFLOW_CHECK(-, cpu->regs.a, memory_read_uint8_t(mem_buf, cpu->regs.pc + 1))) ? SET_FLAG : CLEAR_FLAG;
                sub_flag = SET_FLAG;
                pc_inc = 2; cycles = 8;
                break;
            default:
                lwlog_err("unimplemented opcode: 0x%x", fetched_opcode);
                cpu->running = false;
                break;
        }
    } else {
        switch (fetched_opcode) {
            case 0x11: // NOTE: RL C | 2 | 8 | Z00C
                c_flag = (CARRY_UINT8_T_OVERFLOW_CHECK(<<, cpu->regs.c, 1)) ? SET_FLAG : CLEAR_FLAG;
                cpu->regs.c = (cpu->regs.c << 1) | (cpu->regs.c >> (8 - 1));
                z_flag = (cpu->regs.c) ? CLEAR_FLAG : SET_FLAG;
                pc_inc = 2; cycles = 8;
                break;
            case 0x20: // NOTE: SLA B | 2 | 8 | Z00C
                c_flag = (CARRY_UINT8_T_OVERFLOW_CHECK(<<, cpu->regs.b, 1)) ? SET_FLAG : CLEAR_FLAG;
                cpu->regs.b = cpu->regs.b << 1;
                z_flag = (cpu->regs.b) ? CLEAR_FLAG : SET_FLAG;
                sub_flag = CLEAR_FLAG; hfc_flag = CLEAR_FLAG;
                pc_inc = 2; cycles = 8;
                break;
            case 0x7C: // NOTE: BIT 7, H | 2 | 8 | Z01-
                z_flag = (cpu->regs.h & 0b10000000) ? CLEAR_FLAG : SET_FLAG;
                sub_flag = CLEAR_FLAG; hfc_flag = SET_FLAG;
                pc_inc = 2; cycles = 8;
                break;
            default:
                lwlog_err("cb unimplemented opcode: 0x%x", fetched_opcode);
                cpu->running = false;
                break;
        }
    }

    sharp_sm83_set_flags(cpu, z_flag, sub_flag, hfc_flag, c_flag);
    cpu->regs.pc += pc_inc;
    return cycles;
}
