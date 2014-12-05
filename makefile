CC = g++
mpi = mpicxx

all: fullySequential MPIencryption bruteParallel openMPencryption

fullySequential: fullySequential.o DES.o fileAndConversion.o
	$(CC) -o fullySequential fullySequential.o DES.o fileAndConversion.o

MPIencryption: MPIencryption.o fileAndConversion.o DES.o
	$(mpi) -o MPIencryption MPIencryption.o fileAndConversion.o DES.o

openMPencryption: openMPencryption.o fileAndConversion.o DES.o
	$(CC) -fopenmp -o openMPencryption openMPencryption.o fileAndConversion.o DES.o
	
bruteParallel: DES.o fileAndConversion.o bruteForceParallel.o
	$(mpi) -o bruteParallel DES.o fileAndConversion.o bruteForceParallel.o

fullySequential.o: fullySequential.cpp
	$(CC) -c fullySequential.cpp
	
bruteForce.o: bruteForce.cpp
	$(CC) -c bruteForce.cpp
	
DES.o: DES.cpp
	$(CC) -c DES.cpp
	
fileAndConversion.o: fileAndConversion.cpp
	$(CC) -c fileAndConversion.cpp

MPIencryption.o: MPIencryption.cpp
	$(mpi) -c MPIencryption.cpp

openMPencryption.o: openMPencryption.cpp
	$(CC) -fopenmp -c openMPencryption.cpp

bruteForceParallel.o: bruteForceParallel.cpp
	$(mpi) -c bruteForceParallel.cpp	
	
clean:
	rm -rf *.o fullySequential bruteForce MPIencryption openMPencryption bruteParallel
