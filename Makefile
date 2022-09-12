all: task1 task2 task3

task1: task1.cpp Utils.h Utils.cpp
	g++ -Wall -Werror -std=c++11 -O -o task1 task1.cpp Utils.cpp

task2: task2.cpp Utils.h Utils.cpp
	g++ -Wall -Werror -std=c++11 -O -o task2 task2.cpp Utils.cpp

task3: task3.cpp Utils.h Utils.cpp
	g++ -Wall -Werror -std=c++11 -O -o task3 task3.cpp Utils.cpp -lpthread 

clean:
	rm task1 task2 task3 