# компилятор
CC=g++ -g

# флаги
CFLAGS=-std=c++11 -c -Wall

# --- конфигурация для обычного запуска 
all: little
	
little: Run/main.o Run/calcutron.o
	$(CC) Run/main.o Run/calcutron.o -o Run/LittleCalcutron

Run/main.o: src/main.cpp
	$(CC) $(CFLAGS) src/main.cpp -o Run/main.o

Run/calcutron.o: src/calcutron.cpp
	$(CC) $(CFLAGS) src/calcutron.cpp -o Run/calcutron.o

Run/operatorFactory.o: src/operatorFactory.cpp
	$(CC) $(CFLAGS) src/operatorFactory.cpp -o Run/operatorFactory.o

# --- конфигурация для тестирования с BOOST 
BOOST_PATH=/usr/lib/x86_64-linux-gnu/
BOOST_NAME=boost_unit_test_framework
TEST_LIB=-L$(BOOST_PATH) -l$(BOOST_NAME)


test: Run/main_test.o Run/calcutron_test.o Run/calcutron.o Run/operatorFactory.o
	$(CC) Run/main_test.o Run/calcutron_test.o Run/calcutron.o Run/operatorFactory.o -o Run/LittleCalcutron $(TEST_LIB)

Run/main_test.o: test/main_test.cpp
	$(CC) $(CFLAGS) -I/usr/include test/main_test.cpp -o Run/main_test.o

Run/calcutron_test.o: test/calcutron_test.cpp
	$(CC) $(CFLAGS) -I/usr/include test/calcutron_test.cpp -o Run/calcutron_test.o

clean:
	rm -rf */*.o */LittleCalcutron