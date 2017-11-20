#include "../include/calcutron.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
	istream* input = nullptr;

	switch (argc)
	{
	case 1:
		input = &cin;
		break;
	case 2:
		input = new istringstream(argv[1]);
		break;
	default:
		calcutron::error("too many arguments");
	}

	char ch;
	double result;

	while (true)
	{
		ch = input->get();
		if (ch == '\n') continue;
		if (ch == 'q' || ch == '\0') break;
		input->putback(ch);
		
		// для симпатичного отображения вывода считываем строку и прикидываем её длину
		// на данный момент не учитывается табуляция в исходной строке
		string line;
		getline(*input, line);
		cout.width(line.length());
		cout.fill(' ');
		cout << ' ' << " = ";

		//auto tmp = istringstream(line.c_str());
		auto tmp = new istringstream(line.c_str());

		try
		{
			auto tree = calcutron::parse(tmp);
			if (tree)
			{
				result = tree->get();
				cout << result << endl;
				delete tree;
			}
		}
		catch (const runtime_error &e)
		{
			calcutron::error(e.what());
		}

		delete tmp;
	}

	if (input != &cin)
		delete input;

	return 0;
}
