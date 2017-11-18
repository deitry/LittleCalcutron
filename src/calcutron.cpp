#include "../include/calcutron.h"


namespace calcutron
{
	map<TokenType, regex> tokenRegExMap = 
	{
		{VALUE, regex("[0-9.,]") },
		{OPERATOR, regex("[-+]")}
	};

	double calc(string input)
	{
		stringstream* s = new stringstream(input);
		double out = calc(s);
		delete s;

		return out;
	}

	double calc(stringstream* input)
	{
		// если на входе ничего нет - сразу выходим
		if (!*input) return 0;

		// 1. считываем хоть что-нибудь
		auto token = readToken(input);
		// будет ли readToken выбрасывать ошибки? Если да, их надо будет отлавливать
		if (token == nullptr) return 0;

		double result;

		// если первый прочитанный токен - значение, запоминаем его как левый операнд
		auto left = dynamic_cast<Value*>(token);

		IOperator* op = nullptr;
		if (left == nullptr)
		{
			// если оказалось, что это не значение, то пробуем распознать оператор
			op = dynamic_cast<IOperator*>(token);
		}

		// 2. пробуем считать оператор, если его ещё нет
		if (op == nullptr)
		{
			token = readToken(input);
			// если вообще ничего нет - просто возвращаем left
			if (token == nullptr)
			{
				result = left->get();

				delete left;
				return result;
			}				
			
			op = dynamic_cast<IOperator*>(token);

			if (op == nullptr)
			{
				delete left;
				delete token;
				return 0;	// ОШИБКА, operator expected
			}
		}

		// 3. считываем левый операнд
		token = readToken(input);
		if (token == nullptr)
		{
			// TODO: если вообще нет, предпринимаем попытку применить оператор только к левому операнду - вдруг оператор унарный
		}
		
		auto right = dynamic_cast<Value*>(token);
		
		if (right) // - предполагается, что всё остальное уже проверено на наличие
		{
			// применяем оператор к операндам
			result = op->execute(left, right);
			delete left; delete op; delete right;
			return result;
		}

		return 0;
	}

	IToken* readToken(stringstream* input)
	{
		// считываем строку из потока
		//*input >> s;
		
		string s = "";
		TokenType currentType = NONE;

		char ch[2] = " "; // вообще мы будем считывать по одному символу, но используем c-строку, чтобы легко использовать regex_match
		
		// прокручиваем пробелы - при инициализации символом указан пробел, так что мы сюда автоматически попадём
		while (*input && (ch[0] == ' ' || ch[0] == '\t'))
		{
			ch[0] = input->get();
		}

		// определяем, что за токен перед нами по соответствию символа регулярным выражениям
		// FIXME: лучше проверять всю строку целиком, чтобы потом не получилось выражений вида '...,.,01,.01,' которые тоже будут считаться числом
		for (auto tRegex : tokenRegExMap)
		{
			if (regex_match(ch, tRegex.second))
			{
				currentType = tRegex.first;
			}
		}

		// если мы не знаем, что это за символ, пропускаем
		if (currentType == NONE)
			return nullptr;

		// начинаем собирать строку
		s.push_back(ch[0]);

		// считываем символы, пока не закончится поток или пока не наткнёмся на новый токен
		do {
			ch[0] = input-> get();
			
			// если новый символ не соответствует текущему типу
			// TODO: со скобками будет сложнее
			if (!regex_match(ch, tokenRegExMap[currentType]))
			{
				// возвращаем символ на место, чтобы прочитать позже
				input->putback(ch[0]);
				break;
			}

			// добавляем символ к общей строке
			s.push_back(ch[0]);
		} while (*input);
		
		switch (currentType)
		{
		case VALUE:
			return Value::readValue(s);	// FIXME: привести к единому виду "конструкторы" - или с "прямой" строкой, или по указателю
		case OPERATOR:
			return OperatorFactory::NewOperator(&s);
		default:
			return nullptr;
		}

		return nullptr;
	}

	Value* Value::readValue(string input)
	{
		try
		{
			// во-первых, если в качестве используемого разделителя дробной части используется запятая, заменяем её точкой
			std::replace( input.begin(), input.end(), ',', '.'); 
			
			// а во-вторых, спокойно конвертируем в число
			double a = stod(input.c_str());
			Value* val = new Value(a);
			return val;
		}
		catch (exception& e)
		{
			return nullptr;
		}
	}
}