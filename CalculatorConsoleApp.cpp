#pragma once
#pragma region
#include <iostream>
#include <string>
#include <memory>
#include "insert.h"
#include "validate_except.h"
#include "calculate_except.h"
#include "validate.h"
#include "normalize.h"
#include "calculate.h"
#include "calculator.h"
#include "handle_dump.h"
#pragma endregion

int main()
{
	const WCHAR* appName = L"CalculatorConsoleApp";
	const WCHAR* appVersion = L"v1.0";
	std::unique_ptr<HandleDump> dumpHandler =  std::make_unique<HandleDump>(appName, appVersion);
	SetUnhandledExceptionFilter(dumpHandler->UnHandledExceptionFilter);

	std::cout << "== (●ˇ∀ˇ●) 계산기 프로그램 ==\n";
	std::cout << "[ 설명 ]\n";
	std::cout << "계산할 연산식을 입력하시오. (종료문자 x 혹은 X)\n";
	std::cout << "- 가능한 연산문자 +, -, *, /, 괄호(, )\n";
	std::cout << "- 소수점 2자리까지 표시됩니다.\n";
	std::cout << "- 허용된 연산문자 외의 문자는 제외하고 계산됩니다. \n";
	std::cout << "- 첫 글자가 종료문자(x, X)인 경우 프로그램이 종료됩니다. \n";

	while (1) {
		try {
			std::unique_ptr<InsertExpression> inserter = std::make_unique<InsertExpression>();
			inserter->setExitCharacters("xX");
			inserter->insert();

			std::cout << std::endl << "[ 실행 과정 ]" << std::endl;
			std::cout << "입력된 계산식(앞/뒤 오타 제외):" << inserter->getExpression() << std::endl;

			std::unique_ptr<Calculation> calculator = std::make_unique<Calculation>(
				inserter->getExpression(),
				std::make_unique<ValidateExpression>(),
				std::make_unique<NormalizeExpression>(),
				std::make_unique<CalculateExpression>());

			calculator->run();

		}
		catch (const ValidateException& e) {
			std::cout << e.what() << std::endl;
		}
		catch (const CalculationException& e) {
			std::cout << e.what() << std::endl;
		}
		catch (...) {
			std::cout << "알 수 없는 오류가 발생" << std::endl;
		}
		fflush(stdout);
		
	}
}