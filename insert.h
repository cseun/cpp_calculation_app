#pragma once
#include <iostream>
#include <string>
#include <windows.h>
#include <thread>
#include <chrono>

// --- 인터페이스 ---
class IInsert
{
public:
	virtual void insert() = 0;
	virtual std::string getInsertValue() = 0;
	virtual std::string getValidCharacters() = 0;
	virtual void setExitCharacters(const std::string& exitChars) = 0;
	virtual std::string getExitCharacters() = 0;
	virtual ~IInsert() = default;
};

// --- 메인 클래스 ---
class InsertExpression : public IInsert
{
private:
	std::string insertValue;
	std::string expression;
	static std::string validCharacters;
	static std::string frontNotAllowedCharacters;
	static std::string backNotAllowedCharacters;
	static std::string exitCharacters;

	// 종료 문자 확인
	bool isExitCharacter(char ch);
	// 입력값 필터링
	void filterExpression();

public:
	void insert() override;

	std::string getInsertValue() override;
	std::string getValidCharacters() override;
	void setExitCharacters(const std::string& exitChars) override;
	std::string getExitCharacters() override;
	std::string getExpression();
};

