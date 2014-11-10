#include <stdio.h>
#include <fstream>
#include <iostream>
#include <cmath>
#include "desSequentialAlgorithm.h"
#include "fileAndConversion.h"

using namespace std;

int main(int argc, char* argv[]) {

	bool* L = new bool[32];
	bool* R = new bool[32];
	bool* key = new bool[64];
	char* text = new char[8];
	
	ifstream* inFile;
	inFile.open("input.txt");
	inFile.seekg (0, inFile.end);
	int length = inFile.tellg();	
	
	for(int key = 0; key < pow(2, 56); ++key) {
		// Code to calculate useable keys
		inFile.seekg(0, inFile.beg);
		
		for (int i = 0; i < (length / 8) + 1; ++i) {
			readIn(text, inFile);
			generateLR(L, R, text);
			
			applyDES(L, R, key, true);
			generateOutText(L, R, text);
			// Check if portion of signature detected
			// If so, keep it to test against next time
		}
	}
	return 0;
}