#ifndef _CALCUTRON_H
#define _CALCUTRON_H

#include <string>
#include <sstream>
#include <regex>
#include <map>

using namespace std;

namespace calcutron
{
	double calc(string input);
	double calc(stringstream* s);

	enum TokenType {
		NONE, VALUE, OPERATOR, EXPRESSION, VARIABLE
	};

	extern map<TokenType,regex> tokenRegExMap;

	class IToken
	{
		
	public:
		// virtual string get() = 0;
		virtual ~IToken() {}
	};

	class Value : public IToken
	{
		double value;

	public:
		Value(double d) { value = d; }

		static Value* readValue(string s);
		double get() { return value; }
	};

	class IOperator : public IToken
	{
	public:
		virtual double execute(Value* left, Value* right) = 0;
	};

	class Sum : public IOperator
	{
	public:
	   	double execute(Value* left, Value* right) { return left->get() + right->get(); }
	};

	class Minus : public IOperator
	{
	public:
	   	double execute(Value* left, Value* right) { return left->get() - right->get(); }
	};

	class OperatorFactory
	{
	public:
		static IOperator* NewOperator(string* s);
	};

	IToken* readToken(stringstream* input);
}

#endif