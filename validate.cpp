#include "validate.h"

std::string ValidateExpression::validOperators = "+-*/";
void ValidateExpression::validate(std::string expression) {
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
		else if (validOperators.find(ch) != std::string::npos) {
			hasDot = false;
		}
		else if (isdigit(ch)) {}
		else {
			throw UnknownCharacterException(ch);
		}
		prev = ch;
	}
}