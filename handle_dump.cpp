#pragma once
#include "handle_dump.h"
#include <iostream>

// 싱글톤 패턴
HandleDump::HandleDump(const WCHAR* path, const WCHAR* appName, const WCHAR* appVersion) : path(path), appName(appName), appVersion(appVersion) {
	instance = this;
};

LONG HandleDump::HandleException(_EXCEPTION_POINTERS* pExceptionPointers) {
	DWORD dwBufferSize = MAX_PATH;
	WCHAR szPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szPath);

	WCHAR szFileName[MAX_PATH];
	StringCchPrintf(szFileName, MAX_PATH, L"%s%s", szPath, path);

	int result = SHCreateDirectoryExW(NULL, szFileName, NULL);

	SYSTEMTIME stLocalTime;
	GetLocalTime(&stLocalTime);

	StringCchPrintf(szFileName, MAX_PATH, L"%s\\%s-%04d%02d%02d-%02d%02d%02d.dmp",
		szFileName, appVersion,
		stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay,
		stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond);

	//StringCchPrintf(szFileName, MAX_PATH, L"%s\\%s-%04d%02d%02d-%02d%02d%02d-%ld-%ld.dmp",
	//	szFileName, appVersion,
	//	stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay,
	//	stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond,
	//	GetCurrentProcessId(), GetCurrentThreadId());
	
	// 생성 위치 및 파일명 확인
	 std::wcout << "파일 위치 및 파일명" << std::endl << szFileName;
	 Sleep(2000);
	
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
	bool dumpWritten = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &DumpExceptionInfo, NULL, NULL);

	if (!dumpWritten) {
		std::wcerr << L"MiniDumpWriteDump failed. Error: " << GetLastError() << std::endl;
	}

	CloseHandle(hFile);

	return EXCEPTION_EXECUTE_HANDLER;
};

HandleDump* HandleDump::instance = nullptr;