# компилятор
CC=g++

# флаги
CFLAGS=-std=c++11 -c -Wall -g

# --- конфигурация для обычного запуска 
all: little
	
little: Run/main.o Run/calcutron.o Run/operatorFactory.o Run/readToken.o Run/calc.o
	$(CC) Run/main.o Run/calcutron.o Run/operatorFactory.o Run/readToken.o Run/calc.o -o Run/LittleCalcutron

Run/main.o: src/main.cpp
	$(CC) $(CFLAGS) src/main.cpp -o Run/main.o

Run/calcutron.o: src/calcutron.cpp
	$(CC) $(CFLAGS) src/calcutron.cpp -o Run/calcutron.o

Run/calc.o: src/calc.cpp
	$(CC) $(CFLAGS) src/calc.cpp -o Run/calc.o

Run/readToken.o: src/readToken.cpp
	$(CC) $(CFLAGS) src/readToken.cpp -o Run/readToken.o

Run/operatorFactory.o: src/operatorFactory.cpp
	$(CC) $(CFLAGS) src/operatorFactory.cpp -o Run/operatorFactory.o

# --- конфигурация для тестирования с BOOST 
BOOST_PATH=/usr/lib/x86_64-linux-gnu/
BOOST_NAME=boost_unit_test_framework
TEST_LIB=-L$(BOOST_PATH) -l$(BOOST_NAME)


test: Run/main_test.o Run/calcutron_test.o Run/calcutron.o Run/operatorFactory.o Run/readToken.o Run/calc.o Run/errors_test.o Run/readToken_test.o
	$(CC) Run/main_test.o Run/calcutron_test.o Run/calcutron.o Run/readToken.o Run/operatorFactory.o Run/calc.o Run/errors_test.o Run/readToken_test.o -o Run/LittleCalcutronTest $(TEST_LIB)

Run/main_test.o: test/main_test.cpp
	$(CC) $(CFLAGS) test/main_test.cpp -o Run/main_test.o

Run/calcutron_test.o: test/calcutron_test.cpp
	$(CC) $(CFLAGS) test/calcutron_test.cpp -o Run/calcutron_test.o

Run/errors_test.o: test/errors_test.cpp
	$(CC) $(CFLAGS) test/errors_test.cpp -o Run/errors_test.o

Run/readToken_test.o: test/readToken_test.cpp
	$(CC) $(CFLAGS) test/readToken_test.cpp -o Run/readToken_test.o

clean:
	rm -rf */*.o */LittleCalcutron