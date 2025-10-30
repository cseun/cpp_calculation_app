#pragma once
#include <string>
#include <math.h>
#include <vector>
#include "calculate_except.h"
#include "validate_except.h"

// --- 세부 클래스 ---
class CalculateExpression
{
private:
	// 연산 기능 분리
	int precedence(char op);
	double applyOp(double a, double b, char op);
public:
	// TODO 서브 함수 -> 보조 기능 분리 (스택 감소, 연산 기능 등)
	double calculate(std::string& expression);
	void calculateExpression(std::string& expression, double& finalResult);
};