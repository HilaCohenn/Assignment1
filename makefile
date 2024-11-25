all: clean run

run: bin/Auxiliary.o bin/main.o
	g++ -o bin/main bin/Auxiliary.o bin/main.o

bin/Auxiliary.o: src/Auxiliary.cpp
	g++ -g -wall -weffc++ -std=c++11 -Iinclude -o bin/Auxiliary.o src/Auxiliary.cpp

bin/main.o: src/main.cpp
	g++ -g -wall -weffc -std=c++11 -c -Iinclude -o bin/main.o src/main/cpp

clean:
	rm -f bin/*.o