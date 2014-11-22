#include <stdio.h>
#include <fstream>
#include <iostream>
#include "desSequentialAlgorithm.h"
#include "fileAndConversion.h"
#include "mpi.h"

using namespace std;

static const bool ENCRYPT = true;
static const string INITIAL = "input.txt";
static const string ENCRYPTED = "encrypted.txt";
static const string FINAL = "output.txt";

int main(int argc, char* argv[]) {
	// Set up MPI
	MPI::Init(argc, argv);
	int my_rank = MPI::COMM_WORLD.Get_rank();
	int num_cores = MPI::COMM_WORLD.Get_size();
	double begin_time = MPI::Wtime();
	
	// Create pointer arrays for use in functions
	bool* L = new bool[32];
	bool* R = new bool[32];
	bool* key = new bool[64];
	char* text = new char[8];
	char* allOfTheText;
	char* partOfTheText;
	for( int i = 0; i < 64; ++i)
		key[i]=0;
	key[63]=1;
	
	// Create the file input stream
	ifstream inFile;
	

	// Create the input file and set up the length
	int length;
	inFile.open("input.txt", ios::binary);


	if( my_rank == 0) {
		inFile.seekg (0, inFile.end);
		length = inFile.tellg();
		inFile.seekg(0, inFile.beg);
  	}
  	
  	MPI::COMM_WORLD.Bcast(&length, 1, MPI::INT, 0);

	// Set up the count and offset
	int* count_vec = new int[num_cores];
	int* offset_vec = new int[num_cores];
	int temp = length/num_cores;
	
	for( int i = 0; i < num_cores; ++i ) {
		count_vec[i] = temp;
		offset_vec[i] = i*temp;
	}
	count_vec[num_cores - 1] += length - temp*num_cores;
	
	partOfTheText = new char[count_vec[my_rank]];
	
	inFile.seekg(offset_vec[my_rank], inFile.beg);
	
	for (int i = 0; i < count_vec[my_rank]/8.0; ++i) {
		readIn(&partOfTheText[i*8], &inFile);
		generateLR(L, R, &partOfTheText[i*8]);
		
		applyDES(L, R, key, ENCRYPT);
		
		generateText(L, R, &partOfTheText[i*8]);
	}
	
	allOfTheText = new char[length];
	
	MPI::COMM_WORLD.Gatherv(partOfTheText, count_vec[my_rank], MPI::CHAR,
	                        allOfTheText, count_vec, offset_vec, MPI::CHAR, 0);
	
	
	MPI::COMM_WORLD.Barrier();
	if( my_rank == 0) {
		ofstream outFile;
		outFile.open("output.txt", ios::binary);
		for(int i = 0; i < length; ++i) {
			outFile.put(allOfTheText[i]);
		}
	}

	
	if( my_rank == 0) {
		double end_time = MPI::Wtime();
		double time = end_time - begin_time;
		cout << time << endl;
	}
	
	MPI::Finalize();
	
	return 0;
}
