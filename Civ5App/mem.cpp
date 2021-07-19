#include "pch.h"
#include "mem.h"

void mem::PatchEx(HANDLE hProc, BYTE* dst, BYTE* src, unsigned int size)
{
	DWORD protect;
	VirtualProtectEx(hProc, dst, size, PAGE_EXECUTE_READWRITE, &protect);
	WriteProcessMemory(hProc, dst, src, size, NULL);
	VirtualProtectEx(hProc, dst, size, protect, &protect);
}

void mem::NopEx(HANDLE hProc, BYTE* dst, unsigned int size)
{

	BYTE* nopArray = new BYTE[size];
	memset(nopArray, 0x90, size);
	PatchEx(hProc, dst, nopArray, size);
	delete[] nopArray;
}