
#include "../include/calcutron.h"

using namespace std;

namespace calcutron
{
	// чтение токена
	// токен может быть числом, оператором или выражением.
	// по умолчанию следует считывать токен как выражение, чтобы затем рекурсивно применять к нему calc
	// приоритет указывает на то, какие операции нам надо будет учесть и обработать, а на каких надо будет остановиться
	IToken* readToken(istream* input)
	{
		if (input == nullptr || !*input)
			return nullptr;

		string s = "";
		TokenType currentType = NONE;

		char ch[2] = " "; // вообще мы будем считывать по одному символу, но используем c-строку, чтобы легко использовать regex_match

		// прокручиваем пробелы - при инициализации символом указан пробел, так что мы сюда автоматически попадём
		while (*input && (regex_match(ch, tokenRegExMap[WS])))
		{
			ch[0] = input->get();
		}

		// определяем, что за токен перед нами по соответствию первого символа регулярным выражениям
		for (auto tRegex : tokenRegExMap)
		{
			if (regex_match(ch, tRegex.second))
			{
				currentType = tRegex.first;
				break;
			}
		}

		// если докрутили до конца строки, выходим
		if (currentType == END) return nullptr;

		// если мы не знаем, что это за символ, бросаем ошибку
		if (currentType == NONE)
		{
			throw runtime_error(
				string("unknown token type starting with symbol ") + ch
				);
		}
			
		// если перед нами выражение в скобках
		if (currentType == LP)
		{
			auto expr = new Expression(parse(input, true));
			
			//если нету закрывающей скобки - бросаем исключение
			string par = ch;
			ch[0] = input->get();
			par += ch[0];
			if (!regex_match(par, tokenRegExMap[PAR]))
				throw runtime_error("expected ')");

			return expr;
		}

		// начинаем собирать строку
		s.push_back(ch[0]);

		// считываем символы, пока не закончится поток или пока не наткнёмся на новый токен
		do {
			ch[0] = input->get();

			// если новый символ не соответствует текущему типу
			if (!regex_match(ch, tokenRegExMap[currentType]))
			{
				// возвращаем символ на место, чтобы прочитать позже
				input->putback(ch[0]);
				break;
			}

			// добавляем символ к общей строке
			s.push_back(ch[0]);
		} while (*input);
		
		// убеждаемся, что вся строка целиком соответствует regex
		if (!regex_match(s, tokenRegExMap[currentType]))
		{
			throw runtime_error("incorrect token '" + s + "'");
				// можно было бы ещё прикрутить тип, но не хочу сейчас писать отдельную функцию конвертации TokenType -> string
		}

		switch (currentType)
		{
		case VALUE:
			return Value::readValue(s);
		case OPERATOR:
			return OperatorFactory::NewOperator(s);
		default:
			throw runtime_error("unsupported token type");
		}

		return nullptr;
	}
}
