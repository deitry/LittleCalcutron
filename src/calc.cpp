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
			
			// пробрасываем исключение наверх
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

		char ch[2] = ""; // чтобы можно было проверять на соответствие regex

		while (*input)
		{
			ch[0] = input->get();
			if (regex_match(ch,tokenRegExMap[END])) break;
			if (regex_match(ch,tokenRegExMap[RP]))
			{
				if (insideParentheses)
				{
					// кладём скобку назад, чтобы потом проверить
					input->putback(ch[0]);
					break;
				}
				else
				{
					throw runtime_error("unexpected ')");
				}
			}
			
			input->putback(ch[0]);

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
				IOperator* opParent = nullptr;

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
					
					// если есть родитель, берём его на заметку
					opParent = value->getParent();
				}

				// оцениваем приоритет выполнения оператора
				OpPriority prior = op->priority();

				// если последний токен был оператором
				auto lastOp = dynamic_cast<IOperator*>(last);
				if (lastOp)
				{
					// интересуемся, может ли новый оператор выполнять унарные операции
					if (op->CanBeUnary()) prior = Unar;
					else throw runtime_error("operator can't be unar");
						// TODO: какие-нибудь указания на то, какой именно оператор. Пока нету положения в строке
					opParent = lastOp;
				}
				
				// определяем ближайший оператор меньшего или равного приоритета
				while (opParent && opParent->priority() >= prior)
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
			
			// вставляем куда надо
		}

		return root;
	}
}