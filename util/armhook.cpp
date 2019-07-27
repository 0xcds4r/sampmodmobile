#include "../main.h"
#include "armhook.h"
#include <sys/mman.h>

// ---------------------------------------------------------------------------------- //
#define HOOK_PROC "\x01\xB4\x01\xB4\x01\x48\x01\x90\x01\xBD\x00\xBF\x00\x00\x00\x00"
// -------------------------------------------------------------------------------- //
uintptr_t mmap_start 	= 0;
uintptr_t mmap_end		= 0;
// ----------------------------------------------------------------------------- //
uintptr_t memlib_start	= 0;
uintptr_t memlib_end	= 0;


// ----------------------------------------------------------------------- //

void InitHookStuff()
{
    //LOGI("ARM: InitHookStuff");

    memlib_start = g_libGTASA + 0x180044;
    memlib_end = memlib_start + 0x290;

    mmap_start = (uintptr_t)mmap(0, PAGE_SIZE, PROT_WRITE | PROT_READ | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    mprotect((void*)(mmap_start & 0xFFFFF000), PAGE_SIZE, PROT_READ | PROT_EXEC | PROT_WRITE);
    mmap_end = (mmap_start + PAGE_SIZE);
}

void UnFuck(uintptr_t ptr)
{
    //LOGI("ARM: UnFuck");

	mprotect((void*)(ptr & 0xFFFFF000), PAGE_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC);
}

void WriteMemory(uintptr_t dest, uintptr_t src, size_t size)
{
    //LOGI("ARM: WriteMemory");

    UnFuck(dest);
    memcpy((void*)dest, (void*)src, size);
    cacheflush(dest, dest+size, 0);
}

void ReadMemory(uintptr_t dest, uintptr_t src, size_t size)
{
    //LOGI("ARM: ReadMemory");

    UnFuck(src);
    memcpy((void*)dest, (void*)src, size);
}

void NOP(uintptr_t addr, unsigned int count)
{
    //LOGI("ARM: NOP");

    UnFuck(addr);

    for(uintptr_t ptr = addr; ptr != (addr+(count*2)); ptr += 2)
    {
        *(char*)ptr = 0x00;
        *(char*)(ptr + 1) = 0x46;
    }

    cacheflush(addr, (uintptr_t)(addr + count*2), 0);
}

void WriteHookProc(uintptr_t addr, uintptr_t func)
{
    //LOGI("ARM: WriteHookProc");

    char code[16];
    memcpy(code, HOOK_PROC, 16);
    *(uint32_t*)&code[12] = (func | 1);
    WriteMemory(addr, (uintptr_t)code, 16);
}

void SetUpHook(uintptr_t addr, uintptr_t func, uintptr_t *orig)
{
    //LOGI("ARM: SetUpHook");

    ReadMemory(mmap_start, addr, 4);
    WriteHookProc(mmap_start+4, addr+4);
    *orig = mmap_start+1;
    mmap_start += 32;

    JMPCode(addr, memlib_start);
    WriteHookProc(memlib_start, func);
    memlib_start += 16;
}

void InstallMethodHook(uintptr_t addr, uintptr_t func)
{
    //LOGI("ARM: InstallMethodHook");

    UnFuck(addr);
    *(uintptr_t*)addr = func;
}

void CodeInject(uintptr_t addr, uintptr_t func, int reg)
{
    //LOGI("ARM: CodeInject");

    char injectCode[12];

    injectCode[0] = 0x01;
    injectCode[1] = 0xA0 + reg;
    injectCode[2] = (0x08 * reg) + reg;
    injectCode[3] = 0x68;
    injectCode[4] = 0x87 + (0x08 * reg);
    injectCode[5] = 0x46;
    injectCode[6] = injectCode[4];
    injectCode[7] = injectCode[5];
    
    *(uintptr_t*)&injectCode[8] = func;

    WriteMemory(addr, (uintptr_t)injectCode, 12);
}

void JMPCode(uintptr_t func, uintptr_t addr)
{
    //LOGI("ARM: JMPCode");

    uint32_t code = ((addr-func-4) >> 12) & 0x7FF | 0xF000 | ((((addr-func-4) >> 1) & 0x7FF | 0xB800) << 16);
    WriteMemory(func, (uintptr_t)&code, 4);
}