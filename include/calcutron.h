#ifndef _CALCUTRON_H
#define _CALCUTRON_H

#include <string>
#include <sstream>
#include <regex>
#include <map>

using namespace std;

namespace calcutron
{
	enum TokenType {
		NONE,				// нету такого
		VALUE,			// значение
		OPERATOR,		// оператор
		LP,				// левые скобки - по ним определяем, что надо парсить выражение
		RP,				// правые скобки - по ним определяем, что пора выходить
		PAR,				// в отдельное регулярное выражение складываем все пары,
							// чтобы убедиться, что мы берём скобки из правильной пары
		//EXPRESSION,		// выражение
		//VARIABLE			// переменная
		WS,				// whitespaces - пробелы и табуляция
		END				// конец строки. Строго говоря, это не токен, но для унификации делаем его типом
	};

	// Доступные приоритеты выполнения операторов
	enum OpPriority {
		LessGreat,	// меньше/больше
		PlusMinus,	// сложение и вычитание
		MulDiv,		// умножение и деление
		Unar			// для унарных операций
	};

	// карта соответствия доступных между типами токенов и регулярными выражениями,
	// определяющими разрешённые символы
	extern map<TokenType,regex> tokenRegExMap;

	class IOperator;
	
	// интерфейс, определяющий токен в целом. Выделяем пока операторы и значения
	class IToken
	{		
		friend class IOperator;	// чтобы устанавливать родителя ТОЛЬКО через методы setLeft и setRight класса IOperator 

	protected:
		IOperator* parent;

	public:
		IToken() { parent = nullptr; }
		virtual ~IToken() {}

		virtual double get() = 0;
		bool hasParent() { return (parent ? true : false); }
		IOperator* getParent() { return parent; }
	};

	// класс, описывающий тип значения
	class Value : public IToken
	{
		double value;

	public:
		Value(double d) { value = d; }

		static Value* readValue(const string& s);
		double get() { return value; }
	};

	// используется для описания выражений в скобках
	// поскольку мы хотим, чтобы оно снаружи обрабатывалось как самостоятельное число, используем производный от Value класс 
	class Expression : public Value
	{
		IToken* m_root;
	
	public:
		Expression(IToken* root) : Value(0) {  m_root = root; }
		~Expression() { delete m_root; }

		double get() { return m_root->get(); }
	};

	// интерфейс, описывающий тип оператора. Конкретные операторы будут реализацией этого интерфейса
	class IOperator : public IToken
	{
	protected:	// не private, чтобы облегчить доступ для наследников
		IToken* left;
		IToken* right;

	public:
		IOperator() { left = nullptr; right = nullptr; }
		virtual ~IOperator() {if (left) delete left; if (right) delete right; }

		//virtual double get() = 0;
		virtual OpPriority priority() = 0;	// TODO: не очень красиво, надо подумать на досуге. Эта информация касается класса целиком и должна быть статической
		virtual bool CanBeUnary() { return false; };

		IToken* getLeft() { return left; }
		void setLeft(IToken* token) { left = token; if (token) token->parent = this; }

		IToken* getRight() { return right; }
		void setRight(IToken* token) { right = token; if (token) token->parent = this; }
	};

	class Sum : public IOperator
	{
	public:
	   	double get() {
				if (left == nullptr && right) return right->get();
				if (left && right) return left->get() + right->get();

				throw runtime_error("can't execute Sum operator- missing value");
			}
		
			OpPriority priority() { return PlusMinus; }
			bool CanBeUnary() { return true; };
	};

	class Minus : public IOperator
	{
	public:
	   	double get() {
				if (left == nullptr && right) return -right->get();
				if (right && left) return left->get() - right->get();

				throw runtime_error("can't execute Minus operator - missing value");
			}

			OpPriority priority() { return PlusMinus; }
			bool CanBeUnary() { return true; };
	};

	class Mult : public IOperator
	{
	public:
	   	double get() {
				if (left && right) return left->get() * right->get();

				throw runtime_error("can't execute Mult operator - missing value");
			}

			OpPriority priority() { return MulDiv; }
	};

	class Div : public IOperator
	{
	public:
	   	double get() {
				if (left && right) 
				{
					double denominator = right->get();
					if (denominator == 0)
						throw runtime_error("can't divide by zero");
						
					return left->get() / right->get();
				}
				throw runtime_error("can't execute Div operator - missing value");
			}

			OpPriority priority() { return MulDiv; }
	};

	class LT : public IOperator
	{
	public:
		double get() {
			if (left && right) return left->get() < right->get();

			throw runtime_error("can't execute LT operator - missing value");
		}

		OpPriority priority() { return LessGreat; }
	};

	class GT : public IOperator
	{
	public:
		double get() {
			if (left && right) return left->get() > right->get();

			throw runtime_error("can't execute GT operator - missing value");
		}

		OpPriority priority() { return LessGreat; }
	};

	class EQ : public IOperator
	{
	public:
		double get() {
			if (left && right) return left->get() == right->get();

			throw runtime_error("can't execute EQ operator - missing value");
		}

		OpPriority priority() { return LessGreat; }
	};

	class OperatorFactory
	{
	public:
		static IOperator* NewOperator(const string& s);
	};


	double calculate(string input);		// парсим всю строку
	IToken* parse(istream* input, bool insideParentheses = false);		// парсим всю строку
	IToken* readToken(istream* input);	// считывает отдельный токен
	
	int error(const string& msg);	
}

#endif