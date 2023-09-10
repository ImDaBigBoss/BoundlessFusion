#include <executable.h>

void write_exec(FILE* file, uint8_t* opcodes, uint64_t opcode_num) {
    uint8_t padding[4096] = {0};
    
    game_file_header_t game;
    game.magic = GAME_MAGIC;
    game.version = 1;
    game.size = sizeof(game_file_header_t) + sizeof(padding) + opcode_num;
    game.start_address = sizeof(padding);

    fwrite(&game, sizeof(game_file_header_t), 1, file);
    fwrite(padding, sizeof(padding), 1, file);
    fwrite(opcodes, opcode_num, 1, file);
}
