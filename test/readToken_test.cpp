/**
 * Тестирование функции readToken() - чтение одиночного токена
 * и создание объекта на основе типа токена
 */

#include <boost/test/unit_test.hpp>

#include "../include/calcutron.h"

using namespace calcutron;
using namespace std;

template <typename T> bool check_readToken(const string& input)
{
	string line = input;
	auto str = istringstream(line.c_str());
	auto token = readToken(&str);

	return dynamic_cast<T*>(token) != nullptr;
}

template <typename T> void check_exception_readToken(const string& input)
{
	try { 
		if (check_readToken<Value>(input)) BOOST_CHECK_MESSAGE(false, "'" + input + "' does not throw exception");
	} catch (const exception& e) {}
}

BOOST_AUTO_TEST_CASE(ReadValueTest)
{
	BOOST_CHECK(check_readToken<Value>("10101"));
	BOOST_CHECK(check_readToken<Value>("10.101"));
	BOOST_CHECK(check_readToken<Value>("10,101"));
	BOOST_CHECK(check_readToken<Value>(".101"));
	BOOST_CHECK(check_readToken<Value>("10."));
	check_exception_readToken<Value>(".");
	check_exception_readToken<Value>("0.10.101");		
	check_exception_readToken<Value>("0,10,101");
}

BOOST_AUTO_TEST_CASE(ReadOperatorTest)
{
	// строго говоря, чтение оператора зависит ещё и от OperatorFactory::NewOperator,
	// но та функция до того проста, что отдельно её не тестируем
	BOOST_CHECK(check_readToken<IOperator>("+"));
	BOOST_CHECK(check_readToken<IOperator>("plus"));	// фича
	BOOST_CHECK(check_readToken<IOperator>("-"));
	BOOST_CHECK(check_readToken<IOperator>("*"));
	BOOST_CHECK(check_readToken<IOperator>("/"));
	BOOST_CHECK(check_readToken<IOperator>(">"));
	BOOST_CHECK(check_readToken<IOperator>("<"));
	BOOST_CHECK(check_readToken<IOperator>("=="));
	check_exception_readToken<IOperator>("++");	// два знака плюса подряд считаются как самостоятельный оператор, который мы не поддерживаем
}

// чтение выражений в скобках вызывает parse, так что это ещё и тестирование parse
BOOST_AUTO_TEST_CASE(ReadExpressionTest)
{
	BOOST_CHECK(check_readToken<Expression>("(1+1)"));
	check_exception_readToken<IOperator>("(1+1");
}