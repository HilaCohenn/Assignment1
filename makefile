all: link

link: compile
	@echo 'Linking'
	g++ -o bin/main bin/main.o bin/Auxiliary.o


compile: src/main.cpp src/Auxiliary.cpp
	@echo "Compiling source code"
	g++ -g -c -Wall -Wextra -std=c++11 -I./Include -o bin/main.o src/main.cpp
	g++ -g -c -Wall -Wextra -std=c++11 -I./Include -o bin/Auxiliary.o src/Auxiliary.cpp

clean: 
	@echo "Cleaning bin"
	rm -rf ./bin/*

