#include "normalize.h"

void NormalizeExpression::normalize(std::string& expression) {
	int exprLen = expression.length();
	std::string temp = "";

	// 괄호 및 숫자 간 곱셈 생략 보정
	for (int i = 0; i < exprLen; i++) {
		char ch = expression[i];
		char next = i + 1 < exprLen ? expression[i + 1] : NULL;

		if (next) {
			// 0) 빈 괄호 제거
			if (ch == '(' && next == ')') {
				i++; // 빈괄호 건너뛰기
				continue;
			}

			// 1) 숫자 바로 뒤 '(' → * 추가
			if (isdigit(ch) && next == '(') {
				temp += ch;
				temp += '*';
			}

			// 2) ')' 바로 뒤 숫자 → * 추가
			else if (ch == ')' && isdigit(next)) {
				temp += ch;
				temp += '*';
			}

			// 3) ')' 바로 뒤 '(' → * 추가
			else if (ch == ')' && next == '(') {
				temp += ch;
				temp += '*';
			}

			// 4) '-(' 음수 괄호 처리 → -1*(...)
			else if (ch == '-' && next == '(') {
				temp += '-';
				temp += '1';
				temp += '*';
			}
			else {
				temp += ch;
			}
		}
		else {
			temp += ch;
		}
	}

	expression = temp;
	exprLen = expression.length();
	temp = "";
	for (int i = 0; i < exprLen; i++) {
		char ch = expression[i];
		// 단항 괄호 풀기
		if (ch == '(') {
			int innerStart = i + 1;
			int innerEnd = innerStart;

			// 단항 부호 포함
			if (expression[innerStart] == '-' || expression[innerStart] == '+') innerEnd++;
			// 숫자 및 소수점 포함
			while (isdigit(expression[innerEnd]) || expression[innerEnd] == '.') innerEnd++;
			// 단항 괄호이면 내부 내용만 복사
			if (expression[innerEnd] == ')' && innerEnd > innerStart) {
				for (int j = innerStart; j < innerEnd; j++)
				{
					temp += expression[j];
				}
				i = innerEnd;
				continue;
			}
		}
		temp += ch;
	}
	expression = temp;
}