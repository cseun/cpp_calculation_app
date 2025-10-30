#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <math.h>
#include <vector>
#include "validate_except.h"
#include "calculate_except.h"
#include "validate.h"
#include "normalize.h"
#include "calculate.h"
#include "calculator.h"

// --- 메인 클래스 ---
class Calculation
{
private:
	// 컴포지션
	std::unique_ptr<ValidateExpression> validater_;
	std::unique_ptr<NormalizeExpression> normalizer_;
	std::unique_ptr<CalculateExpression> calculator_;
public:
	// 결과와 계산식의 소유권은	계산기 클래스가 가짐
	double result;
	std::string expression_;

	Calculation(
		std::string expression,
		std::unique_ptr<ValidateExpression> validater,
		std::unique_ptr<NormalizeExpression> normalizer,
		std::unique_ptr<CalculateExpression> calculator
	);

	// 계산기 실행
	void run();
};