#include "Mem.h"

Mem::~Mem()
{
}

Mem::Mem()
{
}

bool Mem::LoadDriver(LPSTR windowName, LPSTR ModuleName)
{

	if (!LoadDriverInternal()) return false;

	if (!GetProcessIdByName(windowName)) return false;

	if (!GetModulebase(ModuleName)) return false;

	return true;
}

bool Mem::LoadDriverInternal()
{
	bool Ret = AriverVerify(DriverKey);

	return Ret;
}

bool Mem::GetProcessIdByName(LPSTR windowName)
{
	HWND hwnd = FindWindowA(NULL, windowName);
	if (!hwnd) return false;

	GetWindowThreadProcessId(hwnd, (LPDWORD)&processId);

	if (!processId && !Cr3_Decrypt(processId)) return false;
	return true;
}

bool Mem::GetModulebase(LPSTR ModuleName)
{
	moduleBase = GetProcessModule(processId, ModuleName);
	return moduleBase ? true : false;
}

UINT64 Mem::GetModulebaseEx(LPSTR ModuleName)
{
	return  GetProcessModule(processId, ModuleName);
}


uintptr_t Mem::StringtoHex(std::string HexString)
{
	return std::strtoull(HexString.c_str(), NULL, 16);
}

uintptr_t Mem::StringtoDec(std::string DecString)
{
	return std::strtoull(DecString.c_str(), NULL, 10);
}

std::string Mem::Utf_16toUtf_8(const std::wstring& utf16_str)
{
	if (utf16_str.empty()) return "";

	int utf8_len = WideCharToMultiByte(CP_UTF8, 0, utf16_str.c_str(), -1, NULL, 0, NULL, NULL);
	if (utf8_len == 0) {
		return "";
	}

	std::string utf8_str(utf8_len, '\0');
	WideCharToMultiByte(CP_UTF8, 0, utf16_str.c_str(), -1, &utf8_str[0], utf8_len, NULL, NULL);
	utf8_str.resize(utf8_len - 1);
	return utf8_str;
}

bool Mem::HOOK_NOP(uintptr_t Addr, ULONG32 Size)
{
	DWORD protect = PAGE_EXECUTE_READWRITE;
	if (!ProtectVirtualMemory(processId, (PVOID)Addr, 0x100, &protect)) return false;

	for (size_t i = 0; i < Size; i++)
	{
		Mem::WriteMemory<BYTE>(Addr + i, 0x90);
	}
	protect = PAGE_EXECUTE;

	ProtectVirtualMemory(processId, (PVOID)Addr, 0x100, &protect);

	return true;
}


Mem& Mem::GetInstance()
{
	static Mem Instance;
	return Instance;
}

std::wstring Mem::ReadStringW(uintptr_t address)
{
	std::vector<wchar_t> StrBuffer(MAX_PATH, 0);

	ReadMemory_Physical(processId, (PVOID)address, StrBuffer.data(), StrBuffer.size());

	return std::wstring(StrBuffer.begin(), StrBuffer.begin() + StrBuffer.size());
}

std::string Mem::ReadStringA(uintptr_t address)
{
	std::vector<char> StrBuffer(MAX_PATH, 0);

	ReadMemory_Physical(processId, (PVOID)address, StrBuffer.data(), StrBuffer.size());

	return std::string(StrBuffer.begin(), StrBuffer.begin() + StrBuffer.size());
}