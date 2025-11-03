#pragma once
#include <windows.h>
#include <string>
#include <strsafe.h>
#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib") // 자동으로 링커에 DbgHelp.lib를 추가
#include <shellapi.h>
#include <shlobj.h>

class HandleDump {
private:
	static HandleDump* instance; // 포인터, 문자열, 객체 등은 클래스 외부에서 초기화

public:
	const WCHAR* path;
	const WCHAR* appName;
	const WCHAR* appVersion;

	// 싱글톤 패턴
	HandleDump(const WCHAR* path, const WCHAR* appName, const WCHAR* appVersion);

	LONG HandleException(_EXCEPTION_POINTERS* pExceptionPointers);
	static LONG WINAPI UnHandledExceptionFilter(_EXCEPTION_POINTERS* pExceptionPointers)
	{
		if (instance) {
			return instance->HandleException(pExceptionPointers);
		}
		return EXCEPTION_EXECUTE_HANDLER;
	}
};
