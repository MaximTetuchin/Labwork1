all: Labwork1

Labwork1: main.o imageFunction.o
	g++ main.o imageFunction.o -o Labwork1

main.o: main.cpp
	g++ -c main.cpp

imageFunction.o: imageFunction.cpp
	g++ -c imageFunction.cpp

clean:
	rm -rf *.o Labwork1