all:
	g++ -std=c++1z -isystem. -Wall -Wextra -pedantic main.cpp -o main
	./main
