#pragma once
#include <ios>
#include <stdio.h>
#include <windows.h>



//请勿用来进行违法犯罪行为，
//所有由使用本驱动文件导致的法律纠纷，
//均与作者本人没有关系，
//且我方将保留法律追究责任权！


#pragma comment(lib,"Ar64-MD.lib")


//======================================================
//安装驱动初始化  
//======================================================
bool AriverVerify(std::string Key);

//普通内核读
bool ReadMemory(ULONG PID, PVOID Target, PVOID Buffer, SIZE_T Size);

//物理无附加读
bool ReadMemory_Physical(ULONG PID, PVOID Target, PVOID Buffer, SIZE_T Size);

//物理页表无附加读
bool ReadMemory_YBPhysical(ULONG PID, PVOID Target, PVOID Buffer, SIZE_T Size);

//物理无附加PTE刷页读
bool ReadMemory_SY_Physical(ULONG PID, PVOID Target, PVOID Buffer, SIZE_T Size);

//物理映射读
bool ReadMemory_MapPhysical(ULONG PID, PVOID Target, PVOID Buffer, SIZE_T Size);

//物理无附加写
bool WriteFor_Physical(ULONG PID, PVOID Target, PVOID Buffer, SIZE_T Size);

//物理无附加PTE刷页写
bool WriteFor_SY_Physical(ULONG PID, PVOID Target, PVOID Buffer, SIZE_T Size);

//MDL强写
bool WriteFor_MDL(ULONG PID, PVOID Target, PVOID Buffer, SIZE_T Size);

//普通内核写
bool WriteFor_Kernel(ULONG PID, PVOID Target, PVOID Buffer, SIZE_T Size);

//进程名取ID
ULONG64 ProcessNameID(const char* Name);

//进程取Peb
ULONG64 GetProcessPeb(ULONG PID);

//内核SetWindowDisplayAffinity 无标识
bool AntiScreen(HWND TargetHwnd, ULONG flag);

//Ex隐藏进程 隐藏进程后无需还原进程可随意退出进程不PG 不蓝屏 支持隐藏多个进程
bool Exhidden_Process(ULONG PID);

//强杀进程
bool ZeroKill(ULONG PID);

//只支持64位进程 支持DLL加壳 注入后自动抹除PE头 隐藏DLL内存 (隐蔽性注入成功率最高)
// 
//1 VAD模式  2 Pte模式 3模式加强Pte隐藏内存模式(如果部分进程会崩溃的话就使用2或1模式)  0 默认高位
void X64Reflection_InjectDll(ULONG PID, PVOID Buffer, SIZE_T Length, ULONG Pte);

//X64无痕注入 只支持X64 注入后自动抹除PE头 隐藏DLL内存
//
//如果注入后DLL无法继续执行或进程崩溃 Pte选项可填 1
void X64Hijack_InjectDll(ULONG PID, PVOID Buffer, SIZE_T Length, ULONG Pte);

//内核拷贝创建线程注入DLL，支持X86 X64 
//
//支持运行的DLL内存内核拷贝到内核中运行然后注入到指定进程中，隐藏DLL的内存来启动DLL
void XCInjectDLL(ULONG PID, PVOID Buffer, SIZE_T Length);

//强删文件
void DelFile(const char* path);

//保护文件 Mos 1 保护文件 0取消保护
bool LockFileS(ULONG Mos, const char* path);

//修改内存属性
bool ProtectVirtualMemory(ULONG PID, PVOID TargetAddress, SIZE_T Size, PVOID Buffer);

//取主进程模块地址
ULONG64 GetMoudleBase(ULONG PID);

//取进程模块
ULONG64 GetProcessModule(ULONG PID, const char* ModuleName);

//取进程模块函数地址
ULONG64 GetModuleExportAddress(ULONG PID, ULONG64 TargetAddress, std::string ModuleName, ULONG x64Module);

//动态鼠标移动
void MOUSE(int X, int Y);

//鼠标按键 1 左键按下 2 左键弹起 4 右键按下 8 右键弹起 10 中间按钮按下 20 中间按钮弹起
bool MOUSE_BUTTON(int Key_VK);

//按键弹起
bool KeyUp(int Key_VK);

//按键按下
bool KeyDown(int Key_VK);

//解密Cr3 
bool Cr3_Decrypt(ULONG PID);

//硬件虚拟
bool Machine();

//隐藏进程 退出进程必须还原 1 隐藏进程 0 还原进程
bool Hidden_Process(ULONG PID, ULONG Mos);

//伪装进程
bool Fake_Process(ULONG PID, ULONG SetPID);

//降权进程
bool Decentralization_Process(ULONG PID);

//提权进程
bool Process_Process(ULONG PID);

//保护进程
bool Protecting_Process(ULONG PID);

//修改进程 退出前请还原，否则容易蓝屏 Mos 1修改进程 0还原进程
bool Modif_Process(ULONG PID, ULONG SetPID, ULONG Mos);

//打开进程
ULONG64 Open_Process(ULONG PID, ULONG64 TargetAddress);

//挂起进程  Mos 1 暂停进程 0 还原进程 
bool Pause_Process(ULONG PID, ULONG Mos);

//禁止新的进程创建 Mos 1 禁止 0 恢复 
bool Prohibit_new(ULONG PID, ULONG Mos);

//隐藏内存
bool Hidememory(ULONG64 TargetAddress);

//释放内存
bool FreeMemory(ULONG PID, ULONG64 TargetAddress);

//低位申请内存 
ULONG64 AllocMemory_low(ULONG PID, SIZE_T Size, ULONG64 TargetAddress, ULONG VadHide);

//高位申请内存 
ULONG64 AllocMemory_High(ULONG PID, SIZE_T Size, ULONG64 TargetAddress, ULONG VadHide);

//申请内存 Map模式
ULONG64 AllocMemory_MAP(ULONG PID, SIZE_T Size, ULONG VadHide);

//无痕远程创建线程
bool TraceCall(ULONG PID, ULONG64 TargetAddress);

//远程创建线程
bool RemoteCall(ULONG PID, ULONG64 TargetAddress);

//查询属性
bool Query_attributes(ULONG PID, PVOID TargetAddress, PMEMORY_BASIC_INFORMATION MemBasciInfo);

//特征码搜索
uintptr_t Feature_code_search(ULONG PID, std::string pattern, ULONG64 scan_base, ULONG64 scan_size);