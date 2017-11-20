#include "../include/calcutron.h"

using namespace std;

namespace calcutron
{	
	double calculate(string input)
	{
		istream* s = new istringstream(input);

		IToken* tree = nullptr;
		double result;

		try
		{
			tree = parse(s);
			result = tree->get();
		}
		catch (const runtime_error& e)
		{
			// удаляем, если успели что создать
			if (tree) delete tree;
			delete s;
			
			// пробрасываем исколючение
			throw e;
		}
	
		if (tree) delete tree;
		delete s;
		
		return result;
	}

	// insideParentheses передаём как параметр, чтобы знать, останавливаться нам на скобке или ошибку кидать
	IToken* parse(istream* input, bool insideParentheses)
	{
		IToken* root = nullptr;
		IToken* last = nullptr;
		IToken* current = nullptr;

		char ch;

		while (*input)
		{
			ch = input->get();
			if (ch == '\0' || ch == '\n' || ch == '\377') break;
			if (ch == ')')
			{
				if (insideParentheses)
				{
					// кладём скобку назад, чтобы потом проверить
					input->putback(ch);
					break;
				}
				else
				{
					throw runtime_error("unexpected ')");
				}
			}
			
			input->putback(ch);

			// считываем токен
			current = readToken(input);
			
			if (root == nullptr)
			{
				last = root = current;
				continue;
			}

			// определяем тип
			auto value = dynamic_cast<Value*>(current);
			if (value)
			{
				if (IOperator* op = dynamic_cast<IOperator*>(last))
				{
					// если значение встретилось после оператора - ставим его справа
					op->setRight(current);
					last = current;
					continue;
				}

				// значение есть, оператора нет - бросить исключение?
				throw runtime_error("expected operator");
			}

			// если оператор
			auto op = dynamic_cast<IOperator*>(current);
			if (op)
			{
				// проверяем, является ли последний токен значением
				auto value = dynamic_cast<Value*>(last);
				if (value)
				{
					// интересуемся, если ли родитель
					if (!value->hasParent())
					{
						// забираем крошку себе
						op->setLeft(value);
						root = last = op;
						continue;
					}
					
					// определяем ближайший оператор меньшего приоритета
					auto opParent = value->getParent();
					while (opParent && opParent->priority() >= op->priority())
						opParent = opParent->getParent();
					
					// если подходящий родитель всё-таки нашёлся
					if (opParent)
					{
						op->setLeft(opParent->getRight());
						opParent->setRight(op);
						last = op;
						continue;
					}
					else
					{
						// если у нового оператора наименьший приоритет, делаем этот оператор новым рутом
						op->setLeft(root);
						root = last = op;
						continue;
					}
				}
			}
			
			// вставляем куда надо
		}

		return root;
	}
}