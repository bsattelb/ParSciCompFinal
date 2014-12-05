#include <stdio.h>
#include <fstream>
#include <iostream>
#include "DES.h"
#include "fileAndConversion.h"

using namespace std;

static const bool ENCRYPT = true;
static const char* inputFile = "input.txt";
static const char* outputFile = "output.txt";


int main(int argc, char* argv[]) {
	bool* L = new bool[32];
	bool* R = new bool[32];
	bool* key = new bool[64];
	char* text = new char[8];
	
	// Initialize key values
	for (int i = 0; i < 64; ++i) {
		key[i] = 0;
	}

	// Set up the input file
	ifstream inFile; 
	inFile.open(inputFile, ios::binary);
	inFile.seekg (0, inFile.end);
	int length = inFile.tellg();
	inFile.seekg(0, inFile.beg);
	
	// Set up the output file
	ofstream outFile;
	outFile.open(outputFile, ios::binary);
	
	// Read in, encrypt, and write out
	int i;
	#pragma omp parallel private(i, L, R, text, inFile)
	{
		#pragma omp for
		for (i = 0; i < (length / 8.0); ++i) {
			readIn(text, &inFile);
			generateLR(L, R, text);
			applyDES(L, R, key, ENCRYPT);
			generateText(L, R, text);
			outFile.write(text, 8);
		}
	}
	
	delete [] L;
	delete [] R;
	delete [] key;
	delete [] text;
	return 0;
}
