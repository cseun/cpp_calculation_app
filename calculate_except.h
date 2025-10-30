#pragma once
#include <string>
#include <stdexcept>

class CalculationException : public std::runtime_error {
protected:
	std::string message;
public:
	CalculationException(const std::string& message);
	const char* what() const noexcept override;
};

class DivisionByZeroException : public CalculationException {
public:
	DivisionByZeroException();
	DivisionByZeroException(const std::string& msg);
};
class EmptyExpressionException : public CalculationException {
public:
	EmptyExpressionException();
	EmptyExpressionException(const std::string& msg);
};