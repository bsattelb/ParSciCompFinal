CC = g++
mpi = mpicxx

all: fullySequential MPIencryption bruteParallel openMPbruteForce MPI3DES

fullySequential: fullySequential.o DES.o fileAndConversion.o
	$(CC) -o fullySequential fullySequential.o DES.o fileAndConversion.o

MPIencryption: MPIencryption.o fileAndConversion.o DES.o
	$(mpi) -o MPIencryption MPIencryption.o fileAndConversion.o DES.o

MPI3DES: MPI3DES.o fileAndConversion.o DES.o
	$(mpi) -o MPI3DES MPI3DES.o fileAndConversion.o DES.o
	
bruteParallel: DES.o fileAndConversion.o bruteForceParallel.o
	$(mpi) -o bruteParallel DES.o fileAndConversion.o bruteForceParallel.o
	
openMPbruteForce: openMPbruteForce.o fileAndConversion.o DES.o
	$(CC) -o openMPbruteForce -fopenmp openMPbruteForce.o fileAndConversion.o DES.o

	
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

MPI3DES.o: MPI3DES.cpp
	$(mpi) -c MPI3DES.cpp

bruteForceParallel.o: bruteForceParallel.cpp
	$(mpi) -c bruteForceParallel.cpp

openMPbruteForce.o: openMPbruteForce.cpp
	$(CC) -fopenmp -c openMPbruteForce.cpp
	
clean:
	rm -rf *.o fullySequential bruteForce MPIencryption openMPencryption bruteParallel openMPbruteForce MPI3DES
