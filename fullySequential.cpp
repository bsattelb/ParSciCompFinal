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
	
	ifstream* inFile = new ifstream("out.txt");
	inFile->seekg (0, inFile->end);
	int length = inFile->tellg();
	inFile->seekg(0, inFile->beg);
	cout << length << endl;
	ofstream outFile;
	outFile.open("out2.txt");
	for (int i = 0; i < 64; ++i) {
		key[i] = 0;
	}
	for (int i = 0; i < (length / 8); ++i) {
		readIn(text, inFile);
		
		generateLR(L, R, text);
		
		applyDES(L, R, key, false);
		
		generateText(L, R, text);
		
		for (int i = 0; i < 8; ++i) {
			if (text[i] != '\0') {
				outFile.put(text[i]);
			}
		}
	}
	
	delete [] L;
	delete [] R;
	delete [] key;
	delete [] text;
	delete inFile;
	return 0;
}
