#include "lwlog/lwlog.h"
#include "dev_manager/gameboy_dmg_01/gameboy_dmg_01.h"
#include "interface/interface_manager.h"

#define LOG_COLOR (1)
#define LOG_LEVEL (7)

int main(int argc, char** argv) {
    Interface_Manager* interface_manager = interface_manager_init();
    interface_manager_start(interface_manager, argc, argv);
    interface_manager_destroy(interface_manager);
    
    /*Gameboy_DMG_01* dev_manager = gameboy_dmg_01_init();
    gameboy_dmg_01_start(dev_manager);
    gameboy_dmg_01_destroy(dev_manager);*/

    return 0;
}
