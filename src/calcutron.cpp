#include "../include/calcutron.h"

#include <iostream>

namespace calcutron
{
	map<TokenType, regex> tokenRegExMap = 
	{
		{VALUE, regex("[0-9]*[.,]?[0-9]*") },
		{OPERATOR, regex("[<*/a-zA-Z+>=-]+")},
		{PARENTHESES, regex("[(]")}	// проверяем на соответствие только левые скобки. Если приспичит, можно будет потом добавить [ и {, но надо будет чуточку подумать, как проверять соответствие закрывающих скобок
	};

	int error(const string& msg)
	{
		cerr << "error: " << msg << '\n';
		return 1;
	}

	Value* Value::readValue(const string& input)
	{
		try
		{
			auto tmp = input;
			// во-первых, если в качестве используемого разделителя дробной части используется запятая, заменяем её точкой
			std::replace(tmp.begin(), tmp.end(), ',', '.'); 
			
			// а во-вторых, спокойно конвертируем в число
			double a = stod(tmp.c_str());
			Value* val = new Value(a);
			return val;
		}
		catch (exception& e)
		{
			throw runtime_error("cannot read value: " + input);
		}

		return nullptr;
	}
}