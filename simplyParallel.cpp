#include "mpi.h"
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <cmath>
#include "desSequentialAlgorithm.h"
#include "fileAndConversion.h"


using namespace std;

static const bool ENCRYPT = false;

static const char INITIAL[] = "output2.txt";
static const char OUTPUT[] = "decrypt2.txt";
static const int MAXSIZE = 16384;

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
	int iterations = (length / MAXSIZE);
	int perCoreMemory = MAXSIZE / num_cores;
	int memoryUsedPerIteration = MAXSIZE;
	int charactersLeft = length - MAXSIZE * iterations;
	


	// Loop through
	for (int i = 0; i < iterations; ++i) { 
		
		// LOAD BALANCE
		for (int j = 0; j < num_cores; ++j) {
			count_vec[j] = perCoreMemory;
			offset_vec[j] = (j > 0) ? count_vec[j-1] : 0;
			offset_vec[j] += (j > 0) ? offset_vec[j-1] : 0;
		}
		
		if (i == iterations - 1) {
			count_vec[num_cores - 1] += charactersLeft;
		}
		
		int count = 0;
		for(int k = 0; k < num_cores; ++k) {
			count += count_vec[k];
		}
		
		// Set up the character memory
		partOfTheText = new char[count_vec[my_rank]];
		if (my_rank == 0) {
			allOfTheText = new char[count];
		}
		//cout << memoryUsedPerIteration << endl;
		//cout << count << endl;
		
		// Find the proper place in the file for a given core and iteration
		inFile.seekg(memoryUsedPerIteration * i + offset_vec[my_rank],
		             inFile.beg);
					 
		// Apply the encryption
		for (int j = 0; j < count_vec[my_rank] / 8.0; ++j) {
			readIn(&partOfTheText[j*8], &inFile);
			/*if (iterations == i + 1 && my_rank + 1 == num_cores) {
				cout << j << " ";
				for (int k = 0; k < 8; k++) {
					cout << (int)partOfTheText[j*8 + k] << " ";
				}
				cout << endl;
			}*/
			generateLR(L, R, &partOfTheText[j*8]);
			applyDES(L, R, key, ENCRYPT);
			generateText(L, R, &partOfTheText[j*8]);
			/*if (iterations == i + 1 && my_rank + 1 == num_cores) {
				cout << j << " ";
				for (int k = 0; k < 8; k++) {
					cout << (int)partOfTheText[j*8 + k] << " ";
				}
				cout << endl;
			}*/
		}
		
		// Gather all the values to master
		MPI::COMM_WORLD.Gatherv(partOfTheText, count_vec[my_rank], MPI::CHAR,
		                        allOfTheText, count_vec, offset_vec, MPI::CHAR,
		                        0);
		                        
		// Output the values
		if (my_rank == 0) {
			outFile.write(allOfTheText, count);
		}
		
		// Prevent memory leaks
		delete [] partOfTheText;
		if (my_rank == 0) {
			delete [] allOfTheText;
		}
		
	}
	if (my_rank == 0) {
		char* text = new char[8];
		inFile.seekg(iterations*MAXSIZE, inFile.beg);
		for (int i = 0; i < charactersLeft / 8.0; ++i) {
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