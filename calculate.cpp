#include "calculate.h"

int CalculateExpression::precedence(char op) {
	if (op == '+' || op == '-') return 1;
	if (op == '*' || op == '/') return 2;
	return 0;
}
double CalculateExpression::applyOp(double a, double b, char op) {
	switch (op) {
	case '+': return a + b;
	case '-': return a - b;
	case '*': return a * b;
	case '/':
		if (b == 0) {
			throw DivisionByZeroException();
		}
		return a / b;
	}
	return 0;
}
void CalculateExpression::calculateExpression(std::string& expression, double& finalResult)
{
	std::string tempExpression = expression; // 계산식 임시저장
	std::string frontExpression = ""; // 괄호 앞 계산식
	std::string innerExpression = ""; // 괄호 내부 계산식
	std::string endExpression = "";	// 괄호 뒤 계산식

	int startIdx = std::string::npos;
	int endIdx = std::string::npos;
	double result = 0.0;

	// 가장 안쪽 괄호부터 계산
	while ((startIdx = expression.rfind('(')) != std::string::npos)
	{
		// 가장 안쪽 닫힘 괄호 찾기
		endIdx = expression.find(')');
		if (endIdx == std::string::npos) {
			throw BracketMismatchException();
		}

		// 괄호 내부 계산식 계산
		innerExpression = expression.substr(startIdx, endIdx + 1);
		result = calculate(innerExpression);

		// 괄호 내부 계산 결과 합치기
		frontExpression = expression.substr(0, startIdx);
		endExpression = expression.substr(endIdx + 1);
		tempExpression = frontExpression + std::to_string(result) + endExpression;

		expression = tempExpression; // 갱신
	}

	// 전체 계산식 계산
	result = calculate(expression);

	finalResult = round(result * 100.0) / 100.0;
}
double CalculateExpression::calculate(std::string& expression)
{
	double result = 0.0;
	std::vector<double> numbers;
	std::vector<char> operators;

	bool isNegative = false;
	int i = 0;
	while (i < expression.length()) {
		char ch = expression[i];
		// 음수 부호 처리 (처음이거나 이전이 연산자일 때)
		if (ch == '-') {
			if (i == 0 || std::string("+*/").find(expression[i - 1]) != std::string::npos) {
				isNegative = true;
				i++;
				continue;
			}
			else if (expression[i + 1] == '-') {
				i++;
				continue;
			}
			else if (expression[i - 1] == '-') {
				ch = '+';
			}
		}

		// 숫자 처리
		if (isdigit(ch)) {
			bool isFraction = false;
			double val = 0;
			double divisor = 1.0;
			while (isdigit(expression[i]) || expression[i] == '.')
			{
				if (expression[i] == '.') {
					isFraction = true;
					i++;
					continue;
				}

				val = val * 10 + (expression[i] - '0');
				if (isFraction) divisor *= 10.0;

				i++;
			}

			val /= divisor;

			if (isNegative) {
				val = -val;
				isNegative = false;
			}

			numbers.push_back(val);
			continue;
		}

		// 연산자 계산
		if (std::string("+-*/").find(ch) != std::string::npos) {
			while (operators.size() > 0 && precedence(operators.back()) >= precedence(ch)) {
				double num2 = numbers.back();
				numbers.pop_back();
				double num1 = numbers.back();
				numbers.pop_back();
				char op = operators.back();
				operators.pop_back();
				numbers.push_back(applyOp(num1, num2, op));
			}
			operators.push_back(ch);
		}

		i++;
	}

	if (numbers.empty()) {
		throw EmptyExpressionException("계산할 숫자가 없습니다.");
	}

	// 남은 연산 처리
	while (operators.size() > 0 && numbers.size() >= 2) {
		double num2 = numbers.back();
		numbers.pop_back();
		double num1 = numbers.back();
		numbers.pop_back();
		char op = operators.back();
		operators.pop_back();
		numbers.push_back(applyOp(num1, num2, op));
	}

	if (numbers.empty()) {
		throw EmptyExpressionException("계산할 숫자가 없습니다.");
	}

	return numbers.back();
}