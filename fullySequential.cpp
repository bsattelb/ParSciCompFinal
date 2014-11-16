#include <stdio.h>
#include <fstream>
#include <iostream>
#include "desSequentialAlgorithm.h"
#include "fileAndConversion.h"

using namespace std;



int main(int argc, char* argv[]) {
	bool* L = new bool[32];
	bool* R = new bool[32];
	bool* key = new bool[64];
	char* text = new char[8];
	
	ifstream* inFile = new ifstream("input.txt");
	inFile->seekg (0, inFile->end);
	int length = inFile->tellg();
	inFile->seekg(0, inFile->beg);
	
	ofstream* outFile = new ofstream("out.txt");
	
	for (int i = 0; i < (length / 8) + 1; ++i) {
		readIn(text, inFile);
		
		generateLR(L, R, text);
		
		applyDES(L, R, key, true);
		
		generateText(L, R, text);

		writeOut(text, outFile);
	}
	// Terminate the file with a null character
	outFile->put('\0');
	
	return 0;
}
