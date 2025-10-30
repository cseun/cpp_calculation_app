#include "calculate_except.h"

CalculationException::CalculationException(const std::string& message) : std::runtime_error(message), message(message) {}
const char* CalculationException::what() const noexcept { return message.c_str(); }
DivisionByZeroException::DivisionByZeroException() : CalculationException("0으로 나눌 수 없습니다.") {}
DivisionByZeroException::DivisionByZeroException(const std::string& msg) : CalculationException(msg) {}
EmptyExpressionException::EmptyExpressionException() : CalculationException("남은 계산식이 없습니다.") {}
EmptyExpressionException::EmptyExpressionException(const std::string& msg) : CalculationException(msg) {}