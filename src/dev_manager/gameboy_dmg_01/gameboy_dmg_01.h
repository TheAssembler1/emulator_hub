#ifndef GAMEBOY_DMG_01_H
#define GAMEBOY_DMG_01_H

#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>

#include "lwlog/lwlog.h"
#include "dev/cpu/sharp_sm83/sharp_sm83_cpu.h"
#include "mem/mem.h"

#define GAMEBOY_DMG_01_MEM_SIZE 0xFFFF
#define GAMEBOY_DMG_01_BOOT_ROM_PATH "./rom/gameboy/dmg_boot.bin"
#define GAMEBOY_DMG_01_BOOT_ROM_PATH_MAX_SIZE 1024

struct Gameboy_DMG_01 {
    struct Sharp_SM83_CPU* cpu;
    struct Memory_Buffer* mem_buf;
};

void gameboy_dmg_01_init(struct Gameboy_DMG_01* gameboy_dmg_01);
void gameboy_dmg_01_destroy(struct Gameboy_DMG_01* gameboy_dmg_01);
void gameboy_dmg_01_start(struct Gameboy_DMG_01* gameboy_dmg_01);
void gameboy_dmg_01_load_boot_rom(struct Gameboy_DMG_01* gameboy_dmg_01);

#endif
