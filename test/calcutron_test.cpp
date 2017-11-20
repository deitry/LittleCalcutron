#include <boost/test/unit_test.hpp>

#include "../include/calcutron.h"

using namespace calcutron;
using namespace std;


// FIXME: в рамках _модульного_ тестирования следует тестировать отдельные функции, а не всё целиком.
// Не только всё целиком.

BOOST_AUTO_TEST_CASE(ReturnNumberTest)
{
	BOOST_CHECK(calculate("1") == 1);
}

BOOST_AUTO_TEST_CASE(SumTest)
{
	BOOST_CHECK(calculate("1+1") == 2);
}

BOOST_AUTO_TEST_CASE(LeadingMinusTest)
{
	BOOST_CHECK(calculate("-1+1") == 0);
}

BOOST_AUTO_TEST_CASE(SumWithSpacesTest)
{
	BOOST_CHECK(calculate("1 + 1") == 2);
}

BOOST_AUTO_TEST_CASE(SumFloatTest)
{
	BOOST_CHECK(calculate("1.5 + 0,5") == 2);
}

BOOST_AUTO_TEST_CASE(MinusTest)
{
	BOOST_CHECK(calculate("3-1") == 2);
}

BOOST_AUTO_TEST_CASE(MinusWithSpacesTest)
{
	BOOST_CHECK(calculate("3 - 1") == 2);
}

BOOST_AUTO_TEST_CASE(MultTest)
{
	BOOST_CHECK(calculate("3*2") == 6);
}

BOOST_AUTO_TEST_CASE(DivTest)
{
	BOOST_CHECK(calculate("4/2") == 2);
}

BOOST_AUTO_TEST_CASE(DivFloatTest)
{
	BOOST_CHECK(calculate("3 / 1.5") == 2);
}

BOOST_AUTO_TEST_CASE(ParenthesesSimpleTest)
{
	BOOST_CHECK(calculate("(1+1)") == 2);
}

BOOST_AUTO_TEST_CASE(ParenthesesComplexTest)
{
	BOOST_CHECK(calculate("-1*(1+1)+3") == 1);
}

BOOST_AUTO_TEST_CASE(ErrorMultipleFloatPointsTest)
{
	try
	{
		string test = "1.1.1";
		calculate(test);
		BOOST_CHECK_MESSAGE(false, "'" + test + "' does not throw exception");
	}
	catch (const exception& e)
	{

	}
}

BOOST_AUTO_TEST_CASE(ErrorMissingOperatorParenthesesTest)
{
	try
	{
		string test = "(1(1))";
		calculate(test);
		BOOST_CHECK_MESSAGE(false, "'" + test + "' does not throw exception");
	}
	catch (const exception& e)
	{

	}
}

BOOST_AUTO_TEST_CASE(ErrorDivByZeroTest)
{
	try
	{
		string test = "1/0";
		calculate(test);
		BOOST_CHECK_MESSAGE(false, "'" + test + "' does not throw exception");
	}
	catch (const exception& e)
	{

	}
}

BOOST_AUTO_TEST_CASE(ErrorMissingOperatorTest)
{
	try
	{
		string test = "1 1 1";
		calculate(test);
		BOOST_CHECK_MESSAGE(false, "'" + test + "' does not throw exception");
	}
	catch (const exception& e)
	{
	
	}
}

BOOST_AUTO_TEST_CASE(ErrorUnsupportedOperatorTest)
{
	try
	{
		string test = "--1";
		calculate(test);
		BOOST_CHECK_MESSAGE(false, "'" + test + "' does not throw exception");
	}
	catch (const exception& e)
	{

	}
}