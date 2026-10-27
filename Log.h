#pragma once
#include "windows.h"
#include <utility>
#include <iostream>
#include <mutex>
namespace FerryLog {

	static HANDLE hConsole;

	static std::mutex mtx;

	enum Level {
		NORMAL,
		ACCESS,
		WARN,
		ERR
	};

	template <typename... Args>
	static void LogBase(Level lv, const char* file, int line, const char* format, Args &&...args) {
		std::lock_guard<std::mutex> _(mtx);
		if (!hConsole)
			hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		// 保存当前控制台文本属性
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(FerryLog::hConsole, &csbi);
		static WORD defAttributes = csbi.wAttributes;
		// 设置控制台文本颜色
		switch (lv) {
		case Level::NORMAL:
			csbi.wAttributes = defAttributes;
			break;
		case Level::ACCESS:
			csbi.wAttributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
			break;
		case Level::WARN:
			csbi.wAttributes = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
			break;
		case Level::ERR:
			csbi.wAttributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
			break;
		}
		SetConsoleTextAttribute(FerryLog::hConsole, csbi.wAttributes);
		char buffer[1024]{ 0 };
		sprintf_s(buffer, format, std::forward<Args>(args)...);
		printf("FILE[%s] LINE[%d] %s\n", file, line, buffer);
		// 恢复原始控制台文本属性
		SetConsoleTextAttribute(FerryLog::hConsole, defAttributes);
	}
}

#define LOG_NORMAL(...) FerryLog::LogBase(FerryLog::NORMAL, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_ACCESS(...) FerryLog::LogBase(FerryLog::ACCESS, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARN(...)   FerryLog::LogBase(FerryLog::WARN,   __FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERR(...)    FerryLog::LogBase(FerryLog::ERR,    __FILE__, __LINE__, __VA_ARGS__)





