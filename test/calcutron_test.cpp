#include <boost/test/unit_test.hpp>

#include "../include/calcutron.h"

using namespace calcutron;
using namespace std;

BOOST_AUTO_TEST_CASE(ReturnNumberTest)
{
	BOOST_CHECK(calc("1") == 1);
}

BOOST_AUTO_TEST_CASE(SumTest)
{
	BOOST_CHECK(calc("1+1") == 2);
}

BOOST_AUTO_TEST_CASE(SumWithSpacesTest)
{
	BOOST_CHECK(calc("1 + 1") == 2);
}

BOOST_AUTO_TEST_CASE(SumFloatTest)
{
	BOOST_CHECK(calc("1.5 + 0,5") == 2);
}

BOOST_AUTO_TEST_CASE(MinusTest)
{
	BOOST_CHECK(calc("3-1") == 2);
}

BOOST_AUTO_TEST_CASE(MinusWithSpacesTest)
{
	BOOST_CHECK(calc("3 - 1") == 2);
}