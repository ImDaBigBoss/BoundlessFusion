#include <executable.h>

void write_exec(FILE* file, uint8_t* opcodes, uint64_t opcode_num, int padding) {
    game_file_header_t game;
    game.magic = GAME_MAGIC;
    game.version = 1;
    game.size = sizeof(game_file_header_t) + padding + opcode_num;
    game.start_address = sizeof(game_file_header_t) + padding;

    fwrite(&game, sizeof(game_file_header_t), 1, file);
    for (int i = 0; i < padding; i++) {
        fputc(0, file);
    }
    fwrite(opcodes, opcode_num, 1, file);
}
