#include <stdio.h>
#include <fstream>
#include <iostream>
#include "desSequentialAlgorithm.h"
#include "fileAndConversion.h"
#include "mpi.h"

using namespace std;

static const bool ENCRYPT = true;

int main(int argc, char* argv[]) {
	MPI::Init(argc,argv);
	int my_rank = MPI::COMM_WORLD.Get_rank();
	int num_cores = MPI::COMM_WORLD.Get_size();
	double begin_time = MPI::Wtime();

	bool* L = new bool[32];
	bool* R = new bool[32];
	bool* key = new bool[64];
	char* text = new char[8];
 

	ifstream* inFile;
	char* allOfTheText;
	char* partOfTheText;
	
	string input;
	string output;
	if (ENCRYPT) {
		input = "picture-wallpaper.jpg";
		output = "output.txt";
	}
	else {
		input = "output.txt";
		output = "picture-wallpaper.jpg";
	}
	char inputFile[input.size()];
	char outputFile[output.size()];
	for (int i = 0; i < input.size(); ++i) {
		inputFile[i] = input[i];
	}
	for (int i = 0; i < output.size(); ++i) {
		outputFile[i] = output[i];
	}
	
	
	
	int length;
	inFile = new ifstream(inputFile);

	if( my_rank == 0) {
		inFile->seekg (0, inFile->end);
		length = inFile->tellg();
		inFile->seekg(0, inFile->beg);
		allOfTheText = new char[length];
  	}

	MPI::COMM_WORLD.Bcast(&length, 1, MPI::INT, 0);

	int* count_vec = new int[num_cores];
	int* offset_vec = new int[num_cores];
	int temp = length/num_cores - (length/num_cores)%8;
	for( int i = 0; i < num_cores-1; ++i ) {
		count_vec[i] = temp;
		offset_vec[i] = i*temp;
	}
	count_vec[num_cores-1] = length-(num_cores-1)*temp;
	offset_vec[num_cores-1] = (num_cores-1)*temp;

	//MPI::COMM_WORLD.Scatterv( allOfTheText, count_vec , offset_vec, MPI::CHAR, 
	//                          partOfTheText, count_vec[my_rank], MPI:CHAR, 0);

	inFile->seekg (offset_vec[my_rank], inFile->beg);

	for (int i = 0; i < (count_vec[my_rank] / 8) + 1; ++i) {

		readIn(text, inFile);
	
		generateLR(L, R, text);
	
		applyDES(L, R, key, ENCRYPT);
	
		generateText(L, R, text); 

	}

	MPI::COMM_WORLD.Gatherv( partOfTheText, count_vec[my_rank], MPI::CHAR,
		                    allOfTheText, count_vec, offset_vec, MPI::CHAR, 0);

	if( my_rank == 0) {
		ofstream outFile(outputFile);
		for(int i = 0; i < length; ++i) {
			outFile.put(allOfTheText[i]);
		}
	}

	
	if( my_rank == 0) {
		double end_time = MPI::Wtime();
		double time = begin_time - end_time;
		cout << time << endl;
	}
	MPI::Finalize();

	return 0;
}
