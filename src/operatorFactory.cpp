#include "../include/calcutron.h"

using namespace calcutron;
using namespace std;

namespace calcutron
{
	IOperator* OperatorFactory::NewOperator(string* s)
	{
		if (*s == "+")
		 	return new Sum;

		if (*s == "-")
		 	return new Minus;

		return nullptr;
	}
}