#include "mpi.h"
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <cmath>
#include "DES.h"
#include "fileAndConversion.h"


using namespace std;

static const bool ENCRYPT = true;

static const char INITIAL[] = "input.png";
static const char OUTPUT[] = "output.txt";
static const int MAXSIZE = 8192; // 2^13

int main(int argc, char* argv[]) {
	// Set up MPI
	MPI::Init(argc, argv);
	int my_rank = MPI::COMM_WORLD.Get_rank();
	int num_cores = MPI::COMM_WORLD.Get_size();
	double begin_time = MPI::Wtime();
	
	// Create pointer arrays for use later on
	bool* L = new bool[32];
	bool* R = new bool[32];
	bool* key = new bool[64];
	char* allOfTheText;
	char* partOfTheText;
	int* count_vec = new int[num_cores];
	int* offset_vec = new int[num_cores];
	
	
	// Declare the key
	for (int i = 0; i < 64; ++i) {
		key[i] = 0;
	}
	
	// Set up the file stream
	ifstream inFile;
	inFile.open(INITIAL, ios::binary);
	
	ofstream outFile;
	if (my_rank == 0) {
		outFile.open(OUTPUT, ios::binary);
	}
	
	// Calculate the length of the fiel
	int length;
	inFile.seekg(0, inFile.end);
	length = inFile.tellg();
	inFile.seekg(0, inFile.beg);
	
	// Calculate the number of iterations
	// to ensure that memory is not overused
	// and calculate the memory used
	int iterations = ceil((double)length / MAXSIZE);
	int perCoreMemory = MAXSIZE / num_cores;
	int memoryUsedPerIteration = perCoreMemory * num_cores;
	int charactersOver = perCoreMemory*num_cores*iterations - length;
	
	// Loop through the normal iterations
	// Create the arrays to be used
	partOfTheText = new char[perCoreMemory];
	if (my_rank == 0) {
		allOfTheText = new char[MAXSIZE];
	}
	for (int i = 0; i < iterations - 1; ++i) {
		
		// Find the proper file position for the iteration and core
		inFile.seekg(memoryUsedPerIteration * i + my_rank * perCoreMemory,
		             inFile.beg);
		
		// Apply the encryption
		for (int j = 0; j < perCoreMemory / 8.0; ++j) {
			readIn(&partOfTheText[j*8], &inFile);
			generateLR(L, R, &partOfTheText[j*8]);
			applyDES(L, R, key, ENCRYPT);
			generateText(L, R, &partOfTheText[j*8]);
		}
		
		// Gather the encrypted values
		MPI::COMM_WORLD.Gather(partOfTheText, perCoreMemory, MPI::CHAR, 
		                       allOfTheText, perCoreMemory, MPI::CHAR, 0);
		
		// Output the encrypted values
		if (my_rank == 0) {
			outFile.write(allOfTheText, MAXSIZE);
		}
	}
	// Deallocate the old memory
	delete [] partOfTheText;
	if (my_rank == 0) {
		delete [] allOfTheText;
	}
	
	// The last iteration
	
	// Create count and offset vectors
	// Calculate the remaining characters
	int totalCharactersLeft = length - MAXSIZE * (iterations - 1);
	int fileLocation = length - totalCharactersLeft;
	if (totalCharactersLeft%8 != 0) {
		totalCharactersLeft += 8 - totalCharactersLeft%8;
	}
	// Calculate the number of characters per core
	perCoreMemory = totalCharactersLeft / num_cores;
	// Calculate the number of characters given to the last core
	int lastCoreMemory = perCoreMemory + (totalCharactersLeft - perCoreMemory * num_cores);
	for (int i = 0; i < num_cores; ++i) {
		count_vec[i]  = perCoreMemory;
		offset_vec[i] = perCoreMemory*i;
	}
	count_vec[num_cores - 1] = lastCoreMemory;
		
	// Set up the memory
	partOfTheText = new char[count_vec[my_rank]];
	if (my_rank == 0) {
		allOfTheText = new char[totalCharactersLeft];
	}
	
	// Find the proper place in the file for a given core and iteration
	inFile.seekg(fileLocation + offset_vec[my_rank],
				 inFile.beg);		 
	// Apply the encryption
	for (int j = 0; j < count_vec[my_rank] / 8.0; ++j) {
		readIn(&partOfTheText[j*8], &inFile);
		generateLR(L, R, &partOfTheText[j*8]);
		applyDES(L, R, key, ENCRYPT);
		generateText(L, R, &partOfTheText[j*8]);
	}
	
	// Gather all the values to master
	MPI::COMM_WORLD.Gatherv(partOfTheText, count_vec[my_rank], MPI::CHAR,
							allOfTheText, count_vec, offset_vec, MPI::CHAR,
							0);
							
	// Output the values
	if (my_rank == 0) {
		outFile.write(allOfTheText, totalCharactersLeft);
	}
	
	// Deallocate pointers
	delete [] partOfTheText;
	delete [] L;
	delete [] R;
	delete [] key;
	delete [] count_vec;
	delete [] offset_vec;
	if (my_rank == 0) {
		delete [] allOfTheText;
	}
	
	if (my_rank == 0) {
		double end_time = MPI::Wtime();
		cout << end_time - begin_time << endl;;
	}
	
	
	MPI::Finalize();
	return 0;
}