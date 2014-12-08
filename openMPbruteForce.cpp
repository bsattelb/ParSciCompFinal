#include <omp.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <cmath>
#include "DES.h"
#include "fileAndConversion.h"

static const bool TIMING = true;
static const int paritiesIncorrect = 3;

using namespace std;

int main(int argc, char* argv[]) {
	
	double start = omp_get_wtime();
	
	bool* L;
	bool* R;
	bool* keyBool;
	
	char* text;
	
	char* signature = new char[8];
	for(int i = 0; i < 8; i++) {
		signature[i] = 'a';
	}
	
	ifstream inFile;
	
	bool keyFound = false;
	int num_keys = pow(2,paritiesIncorrect * 7);
	
	int realKey = -1;
	
	int key;
	int j;
	int i;
	#pragma omp parallel private(inFile, text, L, R, keyBool, key, i, j, keyFound)
	{
		L = new bool[32];
		R = new bool[32];
		keyBool = new bool[64];
		
		text = new char[9];
		text[8] = '\0';
		
		inFile.open("encrypted.txt", ios::binary);
		inFile.seekg(0, inFile.end);
		int length = inFile.tellg();
		inFile.seekg(0, inFile.beg);
		
		#pragma omp for 
		for(key = 0; key < num_keys; ++key) {
			if (!keyFound || TIMING) {
				
				for (i = 0; i < 64; ++i) {
					if (i % 8 == 7) {}
					// Parity bit, so do nothing
					else {
						keyBool[i] = key & (int)pow(2, (i - i/8));
					}
				}
				
				inFile.seekg(0, inFile.beg);
				
				for (i = 0; i < (length / 8); ++i) {
					// Encrypt the text
					readIn(text, &inFile);
					generateLR(L, R, text);
					applyDES(L, R, keyBool, false);
					generateText(L, R, text);
					
					keyFound = true;
					for (j = 0; j < 8; ++j) {
						keyFound = keyFound && (text[j] == signature[j]);
					}
					if (keyFound) {
						realKey = key;
					}
					
				}
			}
		}
	}

	if (realKey == -1) {
		cout << "Signature not found." << endl;
	}
	else {
		cout << "The key to decrypt this file is: " << endl;
		for (int i = 0; i < 64; ++i) {
			cout << (bool)(realKey & (int)pow(2, i));
		}
		cout << endl;
	}
	
	double end = omp_get_wtime();
	cout << "Time taken is " << end - start << endl;
	return 0;
}
