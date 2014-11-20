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
	bool* keyBool = new bool[64];
	char* text = new char[8];
	char* signature = new char[8];
	ifstream* inFile = new ifstream("input.txt", ios::binary);
	inFile->seekg (0, inFile->end);
	int length = inFile->tellg();
	
	bool temp;
 
	for(int key = 0; key < pow(2, 56); ++key) {
		for (int i = 0; i < 64; ++i) {
			if (i % 8 == 0) {}
			else {
				keyBool[i] = key & (int)pow(2, (i- i /8));
			}
		}
		inFile->seekg(0, inFile->beg);
		
		for (int i = 0; i < (length / 8) + 1; ++i) {
			readIn(text, inFile);
			generateLR(L, R, text);
			
			applyDES(L, R, keyBool, false);
			generateText(L, R, text);
			
			temp = true;
			for (int i = 0; i < 8; ++i) {
				temp = temp && text[i] == signature[i];
			}
			if (temp) {
				break;
			}
			
		}
		if (temp) {
			// Output key
			break;
		}
	}
	
	return 0;
}