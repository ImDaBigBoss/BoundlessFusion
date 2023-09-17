#include <bof/game/syscalls.h>
#include <bof/game/game_memory.h>

#include <bof/io/debug.h>

#include <string.h>

vm_syscall_t vm_syscalls[SYSCALL_COUNT];

#define REGISTER_SYSCALL(id, function) \
    vm_syscalls[id - 1] = &function

void define_loop_address(game_vm_t* vm) {
    vm->game_loop_address = vm->registers.r1;
}

void get_memory_info(game_vm_t* vm) {
    vm->registers.r1 = vm->memory_pool.heap_start;
    vm->registers.r2 = vm->memory_pool.block_count;
}

void allocate_memory_blocks(game_vm_t* vm) {
    expand_pool(&vm->memory_pool, vm->registers.r1);
}

void debug_print(game_vm_t* vm) {
    if (vm->registers.r2 > 4096) {
        debug_error("Debug string too long: %d", vm->registers.r2);
        return;
    }

    char* string = (char*) malloc(vm->registers.r2 + 1);
    read_data(&vm->memory_pool, vm->registers.r3, (void*) string, vm->registers.r2);
    string[vm->registers.r2] = '\0';

    switch (vm->registers.r1) {
        case 0: {
            debug_raw("%s", string);
            break;
        }
        case 1: {
            debug_info("%s", string);
            break;
        }
        case 2: {
            debug_warn("%s", string);
            break;
        }
        case 3: {
            debug_error("%s", string);
            break;
        }
        default: {
            debug_error("Invalid debug level: %d", vm->registers.r1);
            break;
        }
    }

    free(string);
}

void dump_registers(game_vm_t* vm) {
    vm_register_dump(vm);
}

void register_syscalls() {
    memset(vm_syscalls, 0, sizeof(vm_syscalls));

    REGISTER_SYSCALL(1, define_loop_address);
    REGISTER_SYSCALL(2, get_memory_info);
    REGISTER_SYSCALL(3, allocate_memory_blocks);
    REGISTER_SYSCALL(4, debug_print);
    REGISTER_SYSCALL(5, dump_registers);
}
