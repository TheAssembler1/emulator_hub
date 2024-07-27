#include "lwlog/lwlog.h"

#define LOG_COLOR (1)
#define LOG_LEVEL (7)

int main(int argc, const char** argv) {
    lwlog_emerg("This is a merge log");

    return 0;
}
