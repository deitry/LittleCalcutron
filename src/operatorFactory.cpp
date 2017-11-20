#include "../include/calcutron.h"

using namespace calcutron;
using namespace std;

namespace calcutron
{
	IOperator* OperatorFactory::NewOperator(const string& s)
	{
		if (s == "+" || s == "plus")
		 	return new Sum;

		if (s == "-" || s == "minus")
		 	return new Minus;

		if (s == "*" || s == "x")
		 	return new Mult;

		if (s == "/")
		 	return new Div;

		if (s == ">")
		 	return new GT;

		if (s == "<")
		 	return new LT;

		if (s == "==")
		 	return new EQ;

		throw runtime_error("unsupported operator");

		return nullptr;
	}
}
