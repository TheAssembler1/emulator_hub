#include "gameboy_dmg_01.h"

void gameboy_dmg_01_init(struct Gameboy_DMG_01* gameboy_dmg_01) {
    lwlog_info("initializing gameboy dmg-01");

    gameboy_dmg_01->cpu = (struct Sharp_SM83_CPU*)malloc(sizeof(struct Sharp_SM83_CPU));
    gameboy_dmg_01->mem_buf = (struct Memory_Buffer*)malloc(sizeof(struct Memory_Buffer));

    lwlog_info("initializing gameboy dmg-01 cpu");
    memory_buffer_init(gameboy_dmg_01->mem_buf, GAMEBOY_DMG_01_MEM_SIZE, LITTLE_ENDIANESS);
    lwlog_info("initializing gameboy dmg-01 memory_buffer");
    sharp_sm83_cpu_init(gameboy_dmg_01->cpu, gameboy_dmg_01->mem_buf);
    lwlog_info("loading boot rom for gameboy dmg-01");
    gameboy_dmg_01_load_boot_rom(gameboy_dmg_01);
}

void gameboy_dmg_01_destroy(struct Gameboy_DMG_01* gameboy_dmg_01) {
    lwlog_info("destroying gameboy dmg-01");

    lwlog_info("destroying gameboy dmg-01 cpu");
    sharp_sm83_cpu_destroy(gameboy_dmg_01->cpu);
    lwlog_info("destroying gameboy dmg-01 memory_buffer");
    memory_buffer_destroy(gameboy_dmg_01->mem_buf);

    free(gameboy_dmg_01->cpu);
    free(gameboy_dmg_01->mem_buf);
}

void gameboy_dmg_01_start(struct Gameboy_DMG_01* gameboy_dmg_01) {
    gameboy_dmg_01->cpu->running = true;
    sharp_sm83_cpu_cycle(gameboy_dmg_01->cpu, gameboy_dmg_01->mem_buf, 64);
}

void gameboy_dmg_01_load_boot_rom(struct Gameboy_DMG_01* gameboy_dmg_01) {
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