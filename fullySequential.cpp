#include <stdio.h>
#include <fstream>
#include <iostream>
#include "desSequentialAlgorithm.h"
#include "fileAndConversion.h"

using namespace std;

static const bool ENCRYPT = true;


int main(int argc, char* argv[]) {
	bool* L = new bool[32];
	bool* R = new bool[32];
	bool* key = new bool[64];
	char* text = new char[8];
	
	bool test [64] = {0,0,0,1,0,0,1,1, 0,0,1,1,0,1,0,0, 0,1,0,1,0,1,1,1, 0,1,1,1,1,0,0,1, 1,0,0,1,1,0,1,1, 1,0,1,1,1,1,0,0, 1,1,0,1,1,1,1,1, 1,1,1,1,0,0,0,1};
	for (int i = 0; i < 64; ++i) {
		key[i] = 0;
	}
	key[0]=1;
	key[4]=1;
	key[8] = 1;
	

	ifstream* inFile = new ifstream("input.txt", ios::binary);
	inFile->seekg (0, inFile->end);
	int length = inFile->tellg();
	inFile->seekg(0, inFile->beg);
	cout << length << endl;
	ofstream outFile;
	outFile.open("output.txt", ios::binary);
	
	for (int i = 0; i < (length / 8.0); ++i) {
		readIn(text, inFile);
		
		generateLR(L, R, text);
		
		applyDES(L, R, key, ENCRYPT);
		
		generateText(L, R, text);
		
		outFile.write(text, 8);
	}
	
	delete [] L;
	delete [] R;
	delete [] key;
	delete [] text;
	delete inFile;
	return 0;
}
