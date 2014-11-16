#include <stdio.h>
#include <fstream>
#include <cmath>
#include "fileAndConversion.h"


void generateLR(bool* L, bool* R, char* text) {
	for (int i = 0; i < 4; ++i) {
		L[i*8 + 0] = text[i] & 128;
		L[i*8 + 1] = text[i] & 64;
		L[i*8 + 2] = text[i] & 32;
		L[i*8 + 3] = text[i] & 16;
		L[i*8 + 4] = text[i] & 8;
		L[i*8 + 5] = text[i] & 4;
		L[i*8 + 6] = text[i] & 2;
		L[i*8 + 7] = text[i] & 1;
		
		R[i*8 + 0] = text[i+4] & 128;
		R[i*8 + 1] = text[i+4] & 64;
		R[i*8 + 2] = text[i+4] & 32;
		R[i*8 + 3] = text[i+4] & 16;
		R[i*8 + 4] = text[i+4] & 8;
		R[i*8 + 5] = text[i+4] & 4;
		R[i*8 + 6] = text[i+4] & 2;
		R[i*8 + 7] = text[i+4] & 1;
	}
}

void generateText(bool* L, bool* R, char* text) {
	for (int i = 0; i < 8; ++i) {
		// Use the values in L and R to generate characters
		int cha = 0;
		for (int j = 0; j < 8; ++j) {
			// Big-endian, so 7-i
			if (i < 4) {
				cha += L[j + i*8] * pow(2, 7 - j);
			}
			else {
				cha += R[j + (i-4)*8] * pow(2, 7 - j);
			}
		}
		text[i] = (char) cha;
	}
}

void readIn(char* text, ifstream* file) {
	// Initialize every element to the null character
	// So that we can know when the end is reached
	for (int i = 0; i < 8; ++i) {
		text[i] = '\0';
	}
	// Read in eight characters
	file->read(text, 8);
}

void writeOut(char* text, ofstream* outFile) {
	for (int i = 0; i < 8; ++i) {
		// Don't output the null character
		if (text[i] != '\0') {
			outFile->put(text[i]);
		}
	}
	
}
