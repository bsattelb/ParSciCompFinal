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
		// Isn't working right
		generateLR(L, R, text);
		for(int j = 0; j < 32; ++j) {
			cout << R[j] << endl;
		}
		// Seg fault here
		applyDES(L, R, key, true);
		
		generateText(L, R, text);
		writeOut(text, outFile);
	}
	// Terminate the file with a null character
	outFile->put('\0');
	
	return 0;
}
