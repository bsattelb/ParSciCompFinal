#include "mpi.h"
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <cmath>
#include "DES.h"
#include "fileAndConversion.h"

static const bool timing = true;
static const int paritiesIncorrect = 3;

using namespace std;

int main(int argc, char* argv[]) {
	MPI::Init(argc,argv);
	double begin_time = MPI::Wtime();
	int num_cores = MPI::COMM_WORLD.Get_size();
	int my_rank = MPI::COMM_WORLD.Get_rank();
	
	bool* L = new bool[32];
	bool* R = new bool[32];
	bool* keyBool = new bool[64];
	
	char* text = new char[9];
	text[8] = '\0';
	char* signature = new char[8];
	for(int i = 0; i < 8; i++) {
		signature[i] = 'a';
	}
	
	ifstream inFile;
	inFile.open("encrypted.txt", ios::binary);
	inFile.seekg(0, inFile.end);
	int length = inFile.tellg();
	inFile.seekg(0, inFile.beg);
	
	struct key_val{
		int isKey;
		int keyNum;
	} inKey, outKey;
	
	inKey.isKey = 0;
	inKey.keyNum = 0;
	
	bool temp = false;
	int num_keys = pow(2,paritiesIncorrect * 7)/num_cores;
	bool isBreak;

	for(int key = my_rank*num_keys; key < my_rank*num_keys+num_keys; ++key) {
		for (int i = 0; i < 64; ++i) {
			if (i % 8 == 7) {}
			// Parity bit, so do nothing
			else {
				keyBool[i] = key & (int)pow(2, (i - i/8));
			}
		}

		inFile.seekg(0, inFile.beg);
		
		for (int i = 0; i < (length / 8); ++i) {
			// Encrypt the text
			readIn(text, &inFile);
			generateLR(L, R, text);
			applyDES(L, R, keyBool, false);
			generateText(L, R, text);
			
			temp = true;
			for (int j = 0; j < 8; ++j) {
				temp = temp && text[j] == signature[j];
			}
			if (temp && !timing) {
				break;
			}
			
		}
		if (temp) {
			inKey.isKey = 1;
			inKey.keyNum = key;
		}
		
		MPI::COMM_WORLD.Allreduce(&temp, &isBreak, 1, MPI::BOOL, MPI::LOR);
		
		if (isBreak && !timing) {
			break;
		}
	}

	MPI::COMM_WORLD.Reduce( &inKey, &outKey, 1, MPI::TWOINT, MPI::MAXLOC, 0);
	if(my_rank == 0){
		cout << "The key to decrypt this file is: " << endl;
		for (int i = 0; i < 64; ++i) {
			cout << (bool)(outKey.keyNum & (int)pow(2, i));
		}
		cout << endl;
		double end_time = MPI::Wtime();
		double time = end_time - begin_time;
		cout << "Time taken: " << time << endl;
	}
	
	MPI::Finalize();
	return 0;
}
