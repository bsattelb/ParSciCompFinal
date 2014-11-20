#include <stdio.h>
#include <fstream>
#include <iostream>
#include <omp.h>
#include "desSequentialAlgorithm.h"
#include "fileAndConversion.h"

using namespace std;

static const bool ENCRYPT = false;


int main(int argc, char* argv[]) {
	bool* L = new bool[32];
	bool* R = new bool[32];
	bool* key = new bool[64];
	char* text = new char[8];
	
	bool test [64] = {0,0,0,1,0,0,1,1, 0,0,1,1,0,1,0,0, 0,1,0,1,0,1,1,1, 0,1,1,1,1,0,0,1, 1,0,0,1,1,0,1,1, 1,0,1,1,1,1,0,0, 1,1,0,1,1,1,1,1, 1,1,1,1,0,0,0,1};
	for (int i = 0; i < 64; ++i) {
		key[i] = test[i];
	}
	
	string input;
	string output;
	if (ENCRYPT) {
		input = "input.txt";
		output = "output.txt";
	}
	else {
		input = "output.txt";
		output = "output2.txt";
	}
	char inputFile[input.size()];
	char outputFile[output.size()];
	for (int i = 0; i < input.size(); ++i) {
		inputFile[i] = input[i];
	}
	for (int i = 0; i < output.size(); ++i) {
		outputFile[i] = output[i];
	}
	ifstream* inFile = new ifstream(inputFile, ios::binary);
	inFile->seekg (0, inFile->end);
	int length = inFile->tellg();
	inFile->seekg(0, inFile->beg);
	cout << length << endl;
	ofstream outFile;
	outFile.open(outputFile, ios::binary);
	int i;
	int numLoops = (length / 8.0) + (length / 8 == length / 8.0);
	char* allText = new char[length];
	inFile->read(allText, length);
	#pragma omp parallel private(L, R, text, inFile, i) num_threads(2)
	{
	#pragma omp for 
	for (i = 0; i < numLoops; ++i) {
		char text[8];
		for (int j = 0; j < 8; ++j) {
			text[j] = allText[i*8 + j];
		}
		generateLR(L, R, text);
		
		applyDES(L, R, key, ENCRYPT);
		
		generateText(L, R, text);
	}
	}
	
	delete [] L;
	delete [] R;
	delete [] key;
	delete [] text;
	delete inFile;
	return 0;
}
