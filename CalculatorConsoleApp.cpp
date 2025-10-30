#include <iostream>
#include <string>
#include <windows.h>
#include <thread>
#include <chrono>
#include <memory>
#include <vector>
using namespace std;

// --- 인터페이스 ---
class IInsert
{
public:
	virtual void insert() = 0;
	virtual string getInsertValue() = 0;
	virtual string getValidCharacters() = 0;
	virtual void setExitCharacters(const string& exitChars) = 0;
	virtual string getExitCharacters() = 0;
	virtual ~IInsert() = default;
};

// --- 세부 클래스 선언 ---
class ValidateExpression;
class NormalizeExpression;
class CalculateExpression;

// --- 메인 클래스 선언 ---
class InsertExpression; // 입력
class Calculation; // 계산

// --- 예외 클래스 ---
#pragma region
class ValidateException : public runtime_error {
protected:
	string message;
public:
	explicit ValidateException(const string& message) : runtime_error(message), message(message) {}
	const char* what() const noexcept override {
		return message.c_str();
	}
};
class BracketMismatchException : public ValidateException {
public:
	BracketMismatchException() : ValidateException("괄호 짝이 맞지 않습니다.") {}
	BracketMismatchException(const string& msg) : ValidateException(msg) {}
};
class UnknownCharacterException : public ValidateException {
public:
	explicit UnknownCharacterException(char ch) : ValidateException("알 수 없는 문자: " + string(1, ch)) {}
};
class InvalidDecimalPointException : public ValidateException {
public:
	InvalidDecimalPointException() : ValidateException("잘못된 소수점 위치입니다.") {}
	InvalidDecimalPointException(const string& msg) : ValidateException(msg) {}
};
class CalculationException : public runtime_error {
protected:
	string message;
public:
	explicit CalculationException(const string& message) : runtime_error(message), message(message) {}
	const char* what() const noexcept override {
		return message.c_str();
	}
};
class DivisionByZeroException : public CalculationException {
public:
	DivisionByZeroException() : CalculationException("0으로 나눌 수 없습니다.") {}
	DivisionByZeroException(const string& msg) : CalculationException(msg) {}
};
class EmptyExpressionException : public CalculationException {
public:
	EmptyExpressionException() : CalculationException("남은 계산식이 없습니다.") {}
	EmptyExpressionException(const string& msg) : CalculationException(msg) {}
};

#pragma endregion

// --- 세부 클래스 ---
class ValidateExpression
{
private:
	string validOperators = "+-*/";
	
public:
	ValidateExpression() = default;

	void validate(string expression) {
		int depth = 0;
		bool hasDot = false;
		char prev = 0;

		for (char ch : expression) {
			if (ch == '(') {
				depth++;
			}
			else if (ch == ')') {
				depth--;
				if (depth < 0) {
					throw BracketMismatchException();
				}
			}
			else if (ch == '.') {
				if (hasDot || !isdigit(prev)) {
					throw InvalidDecimalPointException();
				}
				hasDot = true;
			}
			else if (validOperators.find(ch) != string::npos) {
				hasDot = false;
			}
			else if (isdigit(ch)) {}
			else {
				throw UnknownCharacterException(ch);
			}
			prev = ch;
		}
	}
};
class NormalizeExpression
{
public:
	NormalizeExpression() = default;

	// TODO 메인 함수 -> 함수 분리 (빈괄호 제거, 곱셈 보정, 단항 괄호 풀기)
	void normalize(string& expression) {
		int exprLen = expression.length();
		string temp = "";
		
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
};
class CalculateExpression
{
private:
	// 연산 기능 분리
	int precedence(char op) {
		if (op == '+' || op == '-') return 1;
		if (op == '*' || op == '/') return 2;
		return 0;
	}
	double applyOp(double a, double b, char op) {
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
public:
	// TODO 서브 함수 -> 보조 기능 분리 (스택 감소, 연산 기능 등)
	double calculate(string& expression)
	{
		double result = 0.0;
		vector<double> numbers;
		vector<char> operators;

		bool isNegative = false;
		int i = 0;
		while (i < expression.length()) {
			char ch = expression[i];
			// 음수 부호 처리 (처음이거나 이전이 연산자일 때)
			if (ch == '-') {
				if (i == 0 || string("+*/").find(expression[i - 1]) != string::npos) {
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
			if (string("+-*/").find(ch) != string::npos) {
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
	
	void calculateExpression(string& expression, double& finalResult)
	{
		string tempExpression = expression; // 계산식 임시저장
		string frontExpression = ""; // 괄호 앞 계산식
		string innerExpression = ""; // 괄호 내부 계산식
		string endExpression = "";	// 괄호 뒤 계산식

		int startIdx = string::npos;
		int endIdx = string::npos;
		double result = 0.0;

		// 가장 안쪽 괄호부터 계산
		while ((startIdx = expression.rfind('(')) != string::npos)
		{
			// 가장 안쪽 닫힘 괄호 찾기
			endIdx = expression.find(')');
			if (endIdx == string::npos) {
				throw BracketMismatchException();
			}

			// 괄호 내부 계산식 계산
			innerExpression = expression.substr(startIdx, endIdx + 1);
			result = calculate(innerExpression);

			// 괄호 내부 계산 결과 합치기
			frontExpression = expression.substr(0, startIdx);
			endExpression = expression.substr(endIdx + 1);
			tempExpression = frontExpression + to_string(result) + endExpression;
			
			expression = tempExpression; // 갱신
		}

		// 전체 계산식 계산
		result = calculate(expression);

		finalResult = round(result * 100.0) / 100.0;
	}
};

// --- 메인 클래스 ---
class InsertExpression : public IInsert
{
private:
	string insertValue;
	string expression;
	string validCharacters = "123456789()+-*/.";
	string frontNotAllowedCharacters = "+*/).";
	string backNotAllowedCharacters = "+-*/(.";
	string exitCharacters = "xX";

	// 종료 문자 확인
	bool isExitCharacter(char ch) {
		return (ch != '\0' && exitCharacters.find(ch) != string::npos);
	}
	// 입력값 필터링
	void filterExpression() {
		this->expression.clear(); // 이전 입력값 잔존 방지
		if (validCharacters.empty() || insertValue.empty()) return;

		int lastDigit = -1;
		int lastOp = -1;
		int exprIdx = 0;
		bool isFirst = true;

		for (char ch : insertValue) {
			// 첫 문자 -> 종료 문자 검사
			if (isFirst) {
				if (isExitCharacter(ch)) {
					cout << "프로그램이 2초 후 종료됩니다." << endl;
					this_thread::sleep_for(chrono::seconds(2));
					exit(0);
				}
				isFirst = false;
			}

			if (exprIdx == 0 && frontNotAllowedCharacters.find(ch) != string::npos)
				continue;

			if (validCharacters.find(ch) != string::npos) {
				this->expression += ch;
				if (isdigit(ch))
					lastDigit = exprIdx;
				else
					lastOp = exprIdx;

				exprIdx++;
			}
		}

		// 마지막 숫자 이후 불필요한 연산자 제거
		if (lastDigit != -1 && lastOp > lastDigit) {
			// erase 시 인덱스 조정
			for (int i = lastDigit + 1; i < exprIdx; i++) {
				if (backNotAllowedCharacters.find(expression[i]) != string::npos) {
					this->expression.erase(i, 1);
					i--;
					exprIdx--;
				}
			}
		}
	};

public:
	void insert() override {
		this->insertValue.clear(); // 이전 입력값 리셋
		
		cout << endl << "[ 계산식 입력 ]" << endl;
		cout << "(입력 가능한 문자 외 자동 제거)" << endl;
		cout << ": ";
		
		getline(cin, insertValue);
		filterExpression();
	}

	string getInsertValue() override { return insertValue; }
	string getValidCharacters() override { return validCharacters; }
	void setExitCharacters(const string& exitChars) override { exitCharacters = exitChars; }
	string getExitCharacters() override { return exitCharacters; }
	string getExpression() { return expression; }

};
class Calculation
{
private:
	// 결과와 계산식의 소유권은	계산기 클래스가 가짐
	double result = 0.0;
	string expression_;
	// 컴포지션
	unique_ptr<ValidateExpression> validater_;
	unique_ptr<NormalizeExpression> normalizer_;
	unique_ptr<CalculateExpression> calculator_;
public:
	Calculation(
		string expression,
		unique_ptr<ValidateExpression> validater,
		unique_ptr<NormalizeExpression> normalizer,
		unique_ptr<CalculateExpression> calculator)
		: expression_(expression), validater_(move(validater)), normalizer_(move(normalizer)), calculator_(move(calculator)) {};

	string getExpression() {
		return expression_;
	}
	double getResult() {
		return result;
	}
	// 계산기 실행
	void run() {
		validater_->validate(expression_);
		if (!expression_.length() > 0)
			throw EmptyExpressionException("필터링 후 남은 계산식이 없습니다.");
		cout << "- 필터링된 계산식: " << getExpression() << endl;

		normalizer_->normalize(expression_);
		if (!expression_.length() > 0) 
			throw EmptyExpressionException("보정 후 남은 계산식이 없습니다.");
		cout << "- 보정된 계산식: " << getExpression() << endl;
		
		calculator_->calculateExpression(expression_, result);
		cout << "- 계산 결과: " << getResult() << endl << endl;
	}
};

int main()
{
	cout << "== (●ˇ∀ˇ●) 계산기 프로그램 ==\n";
	cout << "[ 설명 ]\n";
	cout << "계산할 연산식을 입력하시오. (종료문자 x 혹은 X)\n";
	cout << "- 가능한 연산문자 +, -, *, /, 괄호(, )\n";
	cout << "- 소수점 2자리까지 표시됩니다.\n";
	cout << "- 허용된 연산문자 외의 문자는 제외하고 계산됩니다. \n";
	cout << "- 첫 글자가 종료문자(x, X)인 경우 프로그램이 종료됩니다. \n";

	while (1) {
		try {
			unique_ptr<InsertExpression> inserter = make_unique<InsertExpression>();
			inserter->setExitCharacters("xX");
			inserter->insert();

			cout << endl << "[ 실행 과정 ]" << endl;
			cout << "입력된 계산식(앞/뒤 오타 제외):" << inserter->getExpression() << endl;

			unique_ptr<Calculation> calculator = make_unique<Calculation>(
				inserter->getExpression(), 
				make_unique<ValidateExpression>(), 
				make_unique<NormalizeExpression>(), 
				make_unique<CalculateExpression>());

			calculator->run();

		}
		catch (const ValidateException& e) {
			cout << e.what() << endl;
		}
		catch (const CalculationException& e) {
			cout << e.what() << endl;
		}
		catch (...) {
			cout << "알 수 없는 오류가 발생" << endl;
		}
		fflush(stdout);
		
	}
}
	
