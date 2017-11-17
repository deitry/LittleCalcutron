# компилятор
CC=g++

# флаги
CFLAGS=-c -Wall

all: little
	
little: Run/main.o
	$(CC) Run/main.o -o Run/LittleCalcutron

Run/main.o: src/main.cpp
	$(CC) $(CFLAGS) src/main.cpp -o Run/main.o

clean:
	rm -rf */*.o */LittleCalcutron