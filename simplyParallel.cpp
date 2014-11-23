#include <stdio.h>
#include <fstream>
#include <iostream>
#include <cmath>
#include "desSequentialAlgorithm.h"
#include "fileAndConversion.h"
#include "mpi.h"

using namespace std;

static const bool ENCRYPT = true;

static const char INITIAL[] = "input.txt";
static const char OUTPUT[] = "out.txt";
static const int MAXSIZE = 8000;

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
	int perCoreMemory = (length / iterations) / num_cores;
	perCoreMemory -= perCoreMemory % 8;
	int memoryUsedPerIteration = perCoreMemory * num_cores;
	
	int charactersLeft = length - memoryUsedPerIteration * iterations;
	cout << charactersLeft << endl;
	// Loop through
	for (int i = 0; i < iterations; ++i) { 
		// LOAD BALANCE
		for (int j = 0; j < num_cores; ++j) {
			count_vec[j] = perCoreMemory;
			offset_vec[j] = (j > 0) ? count_vec[j-1] : 0;
			offset_vec[j] += (j > 0) ? offset_vec[j-1] : 0;
		}
		
		
		// Set up the character memory
		partOfTheText = new char[perCoreMemory];
		if (my_rank == 0) {
			allOfTheText = new char[memoryUsedPerIteration];
		}
		
		// Find the proper place in the file for a given core and iteration
		inFile.seekg(memoryUsedPerIteration * i + offset_vec[my_rank],
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
			outFile.write(allOfTheText, memoryUsedPerIteration);
		}
		
		// Prevent memory leaks
		delete [] partOfTheText;
		if (my_rank == 0) {
			delete [] allOfTheText;
		}
		
	}
	
	if (my_rank == 0) {
		char* text = new char[8];
		for (int j = 0; j < charactersLeft / 8.0; ++j) {
			readIn(text, &inFile);
			generateLR(L, R, text);
			applyDES(L, R, key, ENCRYPT);
			generateText(L, R, text);
			outFile.write(text, 8);
		}
		delete [] text;
	}
	
	if (my_rank == 0) {
		double end_time = MPI::Wtime();
		cout << end_time - begin_time << endl;;
	}
	
	
	MPI::Finalize();
	return 0;
}
