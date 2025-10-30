#pragma once
#include <string>
#include <stdexcept>
#include <iostream>

class ValidateException : public std::runtime_error {
protected:
	std::string message;
public:
	ValidateException(const std::string& message);
	const char* what() const noexcept override;
};
class BracketMismatchException : public ValidateException {
public:
	BracketMismatchException();
	BracketMismatchException(const std::string& msg);
};
class UnknownCharacterException : public ValidateException {
public:
	UnknownCharacterException(char ch);
};
class InvalidDecimalPointException : public ValidateException {
public:
	InvalidDecimalPointException();
	InvalidDecimalPointException(const std::string& msg);
};