#pragma once
#include <Windows.h>
#include <functional>

typedef HMODULE HMEMORYMODULEPP;
#include "Loader.hpp"
#include "Initialize.hpp"

#define MemoryModuleToModule(_hMemoryModule_) (_hMemoryModule_)

#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#endif

extern "C" {

	HMEMORYMODULEPP WINAPI LoadLibraryMemory(_In_ PVOID BufferAddress,
		const std::function<void(HMODULE)>& PreLoadCallback);

	HMEMORYMODULEPP WINAPI LoadLibraryMemoryExA(
		_In_ PVOID BufferAddress,
		_In_ size_t Reserved,
		_In_opt_ LPCSTR DllBaseName,
		_In_opt_ LPCSTR DllFullName,
		_In_ DWORD Flags,
		const std::function<void(HMODULE)>& PreLoadCallback
	);

	HMEMORYMODULEPP WINAPI LoadLibraryMemoryExW(
		_In_ PVOID BufferAddress,
		_In_ size_t Reserved,
		_In_opt_ LPCWSTR DllBaseName,
		_In_opt_ LPCWSTR DllFullName,
		_In_ DWORD Flags,
		const std::function<void(HMODULE)>& PreLoadCallback
	);

	BOOL WINAPI FreeLibraryMemory(_In_ HMEMORYMODULEPP hMemoryModule);

}

#define NtLoadDllMemory						LdrLoadDllMemory
#define NtLoadDllMemoryExA					LdrLoadDllMemoryExA
#define NtLoadDllMemoryExW					LdrLoadDllMemoryExW
#define NtUnloadDllMemory					LdrUnloadDllMemory
#define NtUnloadDllMemoryAndExitThread		LdrUnloadDllMemoryAndExitThread
#define FreeLibraryMemoryAndExitThread		LdrUnloadDllMemoryAndExitThread
#define NtQuerySystemMemoryModuleFeatures	LdrQuerySystemMemoryModuleFeatures

#ifdef UNICODE
#define LdrLoadDllMemoryEx LdrLoadDllMemoryExW
#define LoadLibraryMemoryEx LoadLibraryMemoryExW
#else
#define LdrLoadDllMemoryEx LdrLoadDllMemoryExA
#define LoadLibraryMemoryEx LoadLibraryMemoryExA
#endif
#define NtLoadDllMemoryEx LdrLoadDllMemoryEx



