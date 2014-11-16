CC = g++

all: fullySequential bruteForce

fullySequential: fullySequential.o desSequentialAlgorithm.o fileAndConversion.o
	$(CC) -o fullySequential fullySequential.o desSequentialAlgorithm.o fileAndConversion.o
bruteForce: bruteForce.o desSequentialAlgorithm.o fileAndConversion.o
	$(CC) -o bruteForce bruteForce.o desSequentialAlgorithm.o fileAndConversion.o
	
fullySequential.o: fullySequential.cpp
	$(CC) -c fullySequential.cpp
	
bruteForce.o: bruteForce.cpp
	$(CC) -c bruteForce.cpp
	
desSequentialAlgorithm.o: desSequentialAlgorithm.cpp
	$(CC) -c desSequentialAlgorithm.cpp
	
fileAndConversion.o: fileAndConversion.cpp
	$(CC) -c fileAndConversion.cpp

clean:
	rm -rf *.o fullySequential bruteForce