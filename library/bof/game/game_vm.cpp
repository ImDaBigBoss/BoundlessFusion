#include <bof/game/game_vm.h>

#include <bof/io/debug.h>

#include <stdlib.h>
#include <string.h>

void vm_init(game_vm_t* vm, game_program_t* program) {
    memset(vm, 0, sizeof(game_vm_t)); //This will set the registers to 0

    //Allocate memory pool
    init_pool(&vm->memory_pool, program);

    //Stack start addess: vm->memory_pool.stack_start vm->memory_pool.stack_size
    vm->registers.sp = vm->memory_pool.stack_start + vm->memory_pool.stack_size;
}

void vm_destroy(game_vm_t* vm) {
    destroy_pool(&vm->memory_pool);   
}
