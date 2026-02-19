#include "stdafx.h"
#include <cstdlib>

HMEMORYMODULEPP WINAPI LoadLibraryMemoryExW(
	_In_ PVOID BufferAddress,
	_In_ size_t Reserved,
	_In_opt_ LPCWSTR DllBaseName,
	_In_opt_ LPCWSTR DllFullName,
	_In_ DWORD Flags,
	const std::function<void(HMODULE)>& PreLoadCallback) {
	HMEMORYMODULEPP hMemoryModule = nullptr;
	NTSTATUS status = LdrLoadDllMemoryExW(&hMemoryModule, nullptr, Flags, BufferAddress, Reserved, DllBaseName, DllFullName, PreLoadCallback);
	if (!NT_SUCCESS(status) || status == STATUS_IMAGE_MACHINE_TYPE_MISMATCH) {
		SetLastError(RtlNtStatusToDosError(status));
	}
	return hMemoryModule;
}

HMEMORYMODULEPP WINAPI LoadLibraryMemoryExA(
	_In_ PVOID BufferAddress,
	_In_ size_t Reserved,
	_In_opt_ LPCSTR DllBaseName,
	_In_opt_ LPCSTR DllFullName,
	_In_ DWORD Flags,
	const std::function<void(HMODULE)>& PreLoadCallback) {
	LPWSTR _DllName = nullptr, _DllFullName = nullptr;
	size_t size;
	HMEMORYMODULEPP result = nullptr;
	HANDLE heap = NtCurrentPeb()->ProcessHeap;

	do {
		if (DllBaseName) {
			size = strlen(DllBaseName) + 1;
			_DllName = (LPWSTR)RtlAllocateHeap(heap, 0, sizeof(WCHAR) * size);

			if (!_DllName) {
				RtlNtStatusToDosError(STATUS_INSUFFICIENT_RESOURCES);
				break;
			}

			mbstowcs_s(nullptr, _DllName, size, DllBaseName, size);
		}

		if (DllFullName) {
			size = strlen(DllFullName) + 1;
			_DllFullName = (LPWSTR)RtlAllocateHeap(heap, 0, sizeof(WCHAR) * size);

			if (!_DllFullName) {
				RtlNtStatusToDosError(STATUS_INSUFFICIENT_RESOURCES);
				break;
			}

			mbstowcs_s(nullptr, _DllFullName, size, DllFullName, size);
		}

		result = LoadLibraryMemoryExW(BufferAddress, 0, _DllName, _DllFullName, Flags, PreLoadCallback);
	} while (false);

	RtlFreeHeap(heap, 0, _DllName);
	RtlFreeHeap(heap, 0, _DllFullName);
	return result;
}

HMEMORYMODULEPP WINAPI LoadLibraryMemory(_In_ PVOID BufferAddress, const std::function<void(HMODULE)>& PreLoadCallback) {
	return LoadLibraryMemoryExW(BufferAddress, 0, nullptr, nullptr, 0, PreLoadCallback);
}

BOOL WINAPI FreeLibraryMemory(_In_ HMEMORYMODULEPP hMemoryModule) {
	NTSTATUS status = LdrUnloadDllMemory(hMemoryModule);
	if (!NT_SUCCESS(status)) {
		SetLastError(RtlNtStatusToDosError(status));
		return FALSE;
	}
	return TRUE;
}
