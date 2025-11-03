#pragma once
#include "handle_dump.h"

// 싱글톤 패턴
HandleDump::HandleDump(const WCHAR* appName, const WCHAR* appVersion) : appName(appName), appVersion(appVersion) {
	instance = this;
};

LONG HandleDump::HandleException(_EXCEPTION_POINTERS* pExceptionPointers) {
	DWORD dwBufferSize = MAX_PATH;
	WCHAR szPath[MAX_PATH];
	GetTempPath(dwBufferSize, szPath);

	WCHAR szFileName[MAX_PATH];
	StringCchPrintf(szFileName, MAX_PATH, L"%s%s", szPath, appName);
	CreateDirectory(szFileName, NULL);
	SYSTEMTIME stLocalTime;
	GetLocalTime(&stLocalTime);

	StringCchPrintf(szFileName, MAX_PATH, L"%s%s\\%s-%04d%02d%02d-%02d%02d%02d-%ld-%ld.dmp",
		szPath, appName, appVersion,
		stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay,
		stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond,
		GetCurrentProcessId(), GetCurrentThreadId());

	// 덤프 파일 생성
	HANDLE hFile = CreateFile(
		szFileName,
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	// 덤프 파일에 예외 정보 기록
	MINIDUMP_EXCEPTION_INFORMATION DumpExceptionInfo;
	DumpExceptionInfo.ThreadId = GetCurrentThreadId(); // Thread ID 설정
	DumpExceptionInfo.ExceptionPointers = pExceptionPointers; // Exception 정보 설정
	DumpExceptionInfo.ClientPointers = FALSE; // 호출 프로세스에서 로컬 메모리에 액세스하는 경우 FALSE

	// 위에서 받은 내용들을 토대로 덤프 파일을 만든다.
	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &DumpExceptionInfo, NULL, NULL);

	CloseHandle(hFile);

	return EXCEPTION_EXECUTE_HANDLER;
};

HandleDump* HandleDump::instance = nullptr;