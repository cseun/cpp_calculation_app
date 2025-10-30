#pragma once
#include <string>

// --- 세부 클래스 ---
class NormalizeExpression
{
public:
	// TODO 메인 함수 -> 함수 분리 (빈괄호 제거, 곱셈 보정, 단항 괄호 풀기)
	void normalize(std::string& expression);
};