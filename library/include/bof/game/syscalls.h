#pragma once

#include <bof/game/game_vm.h>

#define SYSCALL_COUNT 5

typedef void (*vm_syscall_t)(game_vm_t* vm);
extern vm_syscall_t vm_syscalls[SYSCALL_COUNT];

void register_syscalls();
