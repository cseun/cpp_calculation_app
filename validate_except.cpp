#include "validate_except.h"

ValidateException::ValidateException(const std::string& message) : runtime_error(message), message(message) {}
const char* ValidateException::what() const noexcept { return message.c_str(); }
BracketMismatchException::BracketMismatchException() : ValidateException("괄호 짝이 맞지 않습니다.") {}
BracketMismatchException::BracketMismatchException(const std::string& msg) : ValidateException(msg) {}
UnknownCharacterException::UnknownCharacterException(char ch) : ValidateException("알 수 없는 문자: " + std::string(1, ch)) {}
InvalidDecimalPointException::InvalidDecimalPointException() : ValidateException("잘못된 소수점 위치입니다.") {}
InvalidDecimalPointException::InvalidDecimalPointException(const std::string& msg) : ValidateException(msg) {}