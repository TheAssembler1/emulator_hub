#include "lwlog/lwlog.h"
#include "dev_manager/gameboy_dmg_01/gameboy_dmg_01.h"

#define LOG_COLOR (1)
#define LOG_LEVEL (7)

int main(int argc, const char** argv) {
    Gameboy_DMG_01* dev_manager = gameboy_dmg_01_init();
    gameboy_dmg_01_start(dev_manager);
    gameboy_dmg_01_destroy(dev_manager);

    return 0;
}
