#include "gameboy_dmg_01.h"

Gameboy_DMG_01* gameboy_dmg_01_init() {
    lwlog_info("initializing gameboy dmg-01");
    Gameboy_DMG_01* gameboy_dmg_01 = (Gameboy_DMG_01*)calloc(1, sizeof(Gameboy_DMG_01));
    gameboy_dmg_01->cpu = (Sharp_SM83_CPU*)calloc(1, sizeof(Sharp_SM83_CPU));
    gameboy_dmg_01->mem_buf = (Memory_Buffer*)calloc(1, sizeof(Memory_Buffer));

    lwlog_info("initializing gameboy dmg-01 cpu");
    gameboy_dmg_01->mem_buf = memory_buffer_init(GAMEBOY_DMG_01_MEM_SIZE, LITTLE_ENDIANESS);
    lwlog_info("initializing gameboy dmg-01 memory_buffer");
    gameboy_dmg_01->cpu = sharp_sm83_cpu_init(gameboy_dmg_01->mem_buf);
    lwlog_info("loading boot rom for gameboy dmg-01");
    gameboy_dmg_01_load_boot_rom(gameboy_dmg_01);

    return gameboy_dmg_01;
}

void gameboy_dmg_01_destroy(Gameboy_DMG_01* gameboy_dmg_01) {
    lwlog_info("destroying gameboy dmg-01");

    lwlog_info("destroying gameboy dmg-01 cpu");
    sharp_sm83_cpu_destroy(gameboy_dmg_01->cpu);
    lwlog_info("destroying gameboy dmg-01 memory_buffer");
    memory_buffer_destroy(gameboy_dmg_01->mem_buf);

    free(gameboy_dmg_01->cpu);
    free(gameboy_dmg_01->mem_buf);
    free(gameboy_dmg_01);
}

void gameboy_dmg_01_start(Gameboy_DMG_01* gameboy_dmg_01) {
    gameboy_dmg_01->cpu->running = true;
    while(1) {
        sharp_sm83_cpu_cycle(gameboy_dmg_01->cpu, gameboy_dmg_01->mem_buf, 104);
    }
    lwlog_info("ran out of cycles");
}

void gameboy_dmg_01_load_boot_rom(Gameboy_DMG_01* gameboy_dmg_01) {
    char cwd[GAMEBOY_DMG_01_BOOT_ROM_PATH_MAX_SIZE];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        lwlog_info("current working directory: %s", cwd);
    } else {
        lwlog_err("getcwd() error, errno: %s", strerror(errno));
    }

    FILE* fd = fopen(GAMEBOY_DMG_01_BOOT_ROM_PATH, "rb");
    if(!fd) {
        lwlog_err("failed to open file %s, errno: %s", GAMEBOY_DMG_01_BOOT_ROM_PATH, strerror(errno));
        return;
    }

    if(fseek(fd, 0L, SEEK_END) == -1) {
        lwlog_err("failed to fseek: %s, errno: %s", GAMEBOY_DMG_01_BOOT_ROM_PATH, strerror(errno));
        return;
    };

    unsigned int f_size = ftell(fd);
    rewind(fd);

    if(fread(gameboy_dmg_01->mem_buf->buf, f_size, 1, fd) != 1) {
        lwlog_err("failed to read file: %s, bytes requested: %d, errno: %s", GAMEBOY_DMG_01_BOOT_ROM_PATH, f_size,
                  strerror(errno));
        return;
    }

    fclose(fd);
}
