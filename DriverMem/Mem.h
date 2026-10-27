#pragma once
#include "Ar_Driver.h"
#include "string"
#include "vector"

#define DriverKey 
class Mem
{
	Mem();
	~Mem();
	Mem(Mem const&) = delete;
	VOID operator=(Mem const&) = delete;

	ULONG32 processId;

public:

	bool LoadDriver(LPSTR windowName, LPSTR ModuleName); //集合初始化 注册驱动 -> 获取PID -> ModuleBase
	bool LoadDriverInternal();
	bool GetProcessIdByName(LPSTR windowName);
	bool GetModulebase(LPSTR ModuleName);
	UINT64 GetModulebaseEx(LPSTR ModuleName);
public:
	uintptr_t moduleBase;

	unsigned long moduleSize;
	uintptr_t StringtoHex(std::string HexString);
	uintptr_t StringtoDec(std::string DecString);
	std::string Utf_16toUtf_8(const std::wstring& utf16_str);

	bool HOOK_NOP(uintptr_t Addr, ULONG32 Size);
public:

	static Mem& GetInstance();

	std::wstring ReadStringW(uintptr_t address);

	std::string ReadStringA(uintptr_t address);

	inline uintptr_t ReadChain(uintptr_t base, int count, ...) {

		va_list args;
		va_start(args, count);
		uint64_t buff = base;
		for (int i = 0; i < count; ++i) {
			uint32_t offset = va_arg(args, uint64_t);//必须截断
			uint64_t address = buff + offset;
			buff = ReadMemory<uintptr_t>(address);
		}
		va_end(args); // 清理va_list
		return buff;

	}

	template<typename T >
	inline T ReadMemory(uintptr_t address)
	{
		T data = T();
		if (ReadMemory_Physical(processId, (PVOID)address, &data, sizeof(T)) == 1)
		{
			return  data;
		}
		return T();
	}

	template<typename T>
	inline bool WriteMemory(uintptr_t address, T data)
	{
		return WriteFor_SY_Physical(processId, (PVOID)address, &data, sizeof(T));
	}

};

