#include "lwlog/lwlog.h"
#include "mem/mem.h"

#define LOG_COLOR (1)
#define LOG_LEVEL (7)

int main(int argc, const char** argv) {

    struct memory_buffer mem_buf;
    memory_buffer_init(&mem_buf, 64, LITTLE_ENDIANESS);
    memory_buffer_destroy(&mem_buf);

    return 0;
}
