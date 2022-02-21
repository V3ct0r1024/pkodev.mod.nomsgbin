#include <Windows.h>

#include "loader.h"

#include "address.h"
#include "pointer.h"
#include "hook.h"
#include "structure.h"

// Write a byte to the proccess memory
void WriteByte(const unsigned int address, unsigned char byte);

// Entry point
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    // Nothing to do . . .
    return TRUE;
}

// Get mod information
void GetModInformation(mod_info& info)
{
    strcpy_s(info.name, TOSTRING(MOD_NAME));
    strcpy_s(info.version, TOSTRING(MOD_VERSION));
    strcpy_s(info.author, TOSTRING(MOD_AUTHOR));
    info.exe_version = MOD_EXE_VERSION;
}

// Start the mod
void Start(const char* path)
{
    // Disable logs
    WriteByte(pkodev::address::MOD_EXE_VERSION::MakeBinTable__Patch + 1, 0x00);
}

// Stop the mod
void Stop()
{
    // Enable logs
    WriteByte(pkodev::address::MOD_EXE_VERSION::MakeBinTable__Patch + 1, 0x01);
}

// Write a byte to the proccess memory
void WriteByte(const unsigned int address, unsigned char byte)
{
    // Access type
    DWORD access = PAGE_EXECUTE_READWRITE;

    // Change the access protection of the proccess
    VirtualProtect(reinterpret_cast<LPVOID>(address), sizeof(byte), access, &access);

    // Write patch bytes
    *reinterpret_cast<unsigned char*>(address) = byte;

    // Rollback the access protection of the proccess
    VirtualProtect(reinterpret_cast<LPVOID>(address), sizeof(byte), access, &access);
}