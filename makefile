CC = g++
mpi = mpiCC

all: fullySequential bruteForce openMPMain

fullySequential: fullySequential.o desSequentialAlgorithm.o fileAndConversion.o
	$(CC) -o fullySequential fullySequential.o desSequentialAlgorithm.o fileAndConversion.o
bruteForce: bruteForce.o desSequentialAlgorithm.o fileAndConversion.o
	$(CC) -o bruteForce bruteForce.o desSequentialAlgorithm.o fileAndConversion.o

simplyParallel: simplyParallel.o fileAndConversion.o desSequentialAlgorithm.o
	$(mpi) -o simplyParallel simplyParallel.o fileAndConversion.o desSequentialAlgorithm.o

openMPMain: openMPMain.o fileAndConversion.o desSequentialAlgorithm.o
	$(CC) -fopenmp -o openMPMain openMPMain.o fileAndConversion.o desSequentialAlgorithm.o

fullySequential.o: fullySequential.cpp
	$(CC) -c fullySequential.cpp
	
bruteForce.o: bruteForce.cpp
	$(CC) -c bruteForce.cpp
	
desSequentialAlgorithm.o: desSequentialAlgorithm.cpp
	$(CC) -c desSequentialAlgorithm.cpp
	
fileAndConversion.o: fileAndConversion.cpp
	$(CC) -c fileAndConversion.cpp

simplyParallel.o: simplyParallel.cpp
	$(mpi) -c simplyParallel.cpp

openMPMain.o: openMPMain.cpp
	$(CC) -fopenmp -c openMPMain.cpp
	
clean:
	rm -rf *.o fullySequential bruteForce simplyParallel openMPMain
