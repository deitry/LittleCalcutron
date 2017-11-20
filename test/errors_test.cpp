/**
 * Тестирование калькулятора на различные ошибки. Представленные здесь тесты 
 * должны "ломаться", если функция отрабатывает без выбрасывания искючения
 */
#include <boost/test/unit_test.hpp>

#include "../include/calcutron.h"

using namespace calcutron;
using namespace std;

void check_exception_calculate(string input)
{
	try
	{
		calculate(input);
		BOOST_CHECK_MESSAGE(false, "'" + input + "' does not throw exception");
	}
	catch (const exception& e)
	{

	}
}

BOOST_AUTO_TEST_CASE(ErrorParenthesesTest)
{
	check_exception_calculate("(1(1))");
	//check_exception_calculate("(1+1");	// это же самое мы тестируем в readValue
	check_exception_calculate("1+1)");	// а такое нет
	check_exception_calculate("(1+1]");
}

BOOST_AUTO_TEST_CASE(ErrorDivByZeroTest)
{
	check_exception_calculate("1/0");
}

BOOST_AUTO_TEST_CASE(ErrorUnsupportedOperatorTest)
{
	check_exception_calculate("--1");
}

BOOST_AUTO_TEST_CASE(ErrorMissingOperatorTest)
{
	check_exception_calculate("1 1 1");
}

// Больше не считается ошибочным вариантом - подобный случай будет чекаться в другом срр
// Теперь, если оператор (в данном случае, минус) разрешает унарные операции, подобного рода конструкции разрешены
// - это обеспечит поддержку операций вида 5 * sin x, где sin - унарный оператор
// BOOST_AUTO_TEST_CASE(ErrorMissingValueTest)
// {
// 	check_exception_calculate("1* -1");
//}