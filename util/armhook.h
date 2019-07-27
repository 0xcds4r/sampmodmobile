#pragma once

void InstallMethodHook(uintptr_t addr, uintptr_t func);
void WriteMemory(uintptr_t dest, uintptr_t src, size_t size);
void ReadMemory(uintptr_t dest, uintptr_t src, size_t size);
void CodeInject(uintptr_t addr, uintptr_t func, int register);
void SetUpHook(uintptr_t addr, uintptr_t func, uintptr_t *orig);
void JMPCode(uintptr_t func, uintptr_t addr);
void UnFuck(uintptr_t ptr);
void NOP(uintptr_t addr, unsigned int count);