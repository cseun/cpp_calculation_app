#include "insert.h"

std::string InsertExpression::validCharacters = "123456789()+-*/.";
std::string InsertExpression::frontNotAllowedCharacters = "+*/).";
std::string InsertExpression::backNotAllowedCharacters = "+-*/(.";
std::string InsertExpression::exitCharacters = "xX";
bool InsertExpression::isExitCharacter(char ch) {
	return (ch != '\0' && exitCharacters.find(ch) != std::string::npos);
}
void InsertExpression::filterExpression() {
	this->expression.clear(); // 이전 입력값 잔존 방지
	if (validCharacters.empty() || insertValue.empty()) return;

	int lastDigit = -1;
	int lastOp = -1;
	int exprIdx = 0;
	bool isFirst = true;

	for (char ch : insertValue) {
		// 첫 문자 -> 종료 문자 검사
		if (isFirst) {
			// TODO 프로그램 종료 오류 throw 하여 메인에서 처리
			if (isExitCharacter(ch)) {
				std::cout << "프로그램이 2초 후 종료됩니다." << std::endl;
				std::this_thread::sleep_for(std::chrono::seconds(2));
				exit(0);
			}
			isFirst = false;
		}

		if (exprIdx == 0 && frontNotAllowedCharacters.find(ch) != std::string::npos)
			continue;

		if (validCharacters.find(ch) != std::string::npos) {
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
			if (backNotAllowedCharacters.find(expression[i]) != std::string::npos) {
				this->expression.erase(i, 1);
				i--;
				exprIdx--;
			}
		}
	}
};
void InsertExpression::insert() {
	this->insertValue.clear(); // 이전 입력값 리셋

	std::cout << std::endl << "[ 계산식 입력 ]" << std::endl;
	std::cout << "(입력 가능한 문자 외 자동 제거)" << std::endl;
	std::cout << ": ";

	getline(std::cin, insertValue);
	filterExpression();
}
std::string InsertExpression::getInsertValue() { return insertValue; }
std::string InsertExpression::getValidCharacters() { return validCharacters; }
void InsertExpression::setExitCharacters(const std::string& exitChars) { exitCharacters = exitChars; }
std::string InsertExpression::getExitCharacters() { return exitCharacters; }
std::string InsertExpression::getExpression() { return expression; }