#include "lwlog/lwlog.h"
#include "mem/mem.h"
#include "dev/cpu/sharp_sm83/sharp_sm83_cpu.h"

#define LOG_COLOR (1)
#define LOG_LEVEL (7)

int main(int argc, const char** argv) {
    struct Sharp_SM83_CPU cpu;
    struct Memory_Buffer mem_buf;
    
    lwlog_info("creating memory buffer");
    memory_buffer_init(&mem_buf, 64, LITTLE_ENDIANESS);
    lwlog_info("creating sm83 cpu");
    sharp_sm83_cpu_init(&cpu, &mem_buf);    

    sharp_sm83_cpu_cycle(&cpu, 1);

    lwlog_info("destroying sm83 cpu");
    sharp_sm83_cpu_destroy(&cpu);
    lwlog_info("destroying memory buffer");
    memory_buffer_destroy(&mem_buf);

    return 0;
}
