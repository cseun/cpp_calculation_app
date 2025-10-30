#include "calculator.h"

Calculation::Calculation(
	std::string expression,
	std::unique_ptr<ValidateExpression> validater,
	std::unique_ptr<NormalizeExpression> normalizer,
	std::unique_ptr<CalculateExpression> calculator
)
	: validater_(move(validater)),
	normalizer_(move(normalizer)),
	calculator_(move(calculator)),
	expression_(std::move(expression)),
	result(0.0)
{
};

// 계산기 실행
void Calculation::run() {
	validater_->validate(expression_);
	if (!expression_.length() > 0)
		throw EmptyExpressionException("필터링 후 남은 계산식이 없습니다.");
	std::cout << "- 필터링된 계산식: " << expression_ << std::endl;

	normalizer_->normalize(expression_);
	if (!expression_.length() > 0)
		throw EmptyExpressionException("보정 후 남은 계산식이 없습니다.");
	std::cout << "- 보정된 계산식: " << expression_ << std::endl;

	calculator_->calculateExpression(expression_, result);
	std::cout << "- 계산 결과: " << result << std::endl << std::endl;
}
