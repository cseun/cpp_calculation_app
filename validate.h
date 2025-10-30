#pragma once
#include <string>
#include "validate_except.h"

// --- 세부 클래스 ---
class ValidateExpression
{
private:
	static std::string validOperators;
public:
	void validate(std::string expression);
};