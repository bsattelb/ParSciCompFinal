#include <stdio.h>
#include <fstream>
#include <iostream>
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
