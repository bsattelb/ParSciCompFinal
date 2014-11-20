#include <stdio.h>
#include <fstream>
#include <iostream>
#include <cmath>
#include "desSequentialAlgorithm.h"
#include "fileAndConversion.h"
#include "mpi.h"

using namespace std;

int main(int argc, char* argv[]) {
  MPI::Init(argc,argv);
  double begin_time = MPI::Wtime();
  int num_cores = MPI::COMM_WORLD.Get_size();
  int my_rank = MPI::COMM_WORLD.Get_rank();
	bool* L = new bool[32];
	bool* R = new bool[32];
	bool* keyBool = new bool[64];
	char* text = new char[8];
	char* signature = new char[8];
	ifstream* inFile = new ifstream("input.txt", ios::binary);
	inFile->seekg (0, inFile->end);
	int length = inFile->tellg();
  struct key_val{
    int isKey;
    int keyNum;
  } inKey, outKey;
  inKey.isKey = 0;
  inkey.keyNum = 0;
	
	bool temp;
  int num_keys = pow(2,56)/num_cores;

  if(my_rank != num_cores - 1) {
	for(int key = my_rank*num_keys; key < my_rank*num_keys+num_keys ; ++key) {
		for (int i = 0; i < 64; ++i) {
			if (i % 8 == 7) {}
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
			inKey.isKey = 1;
      inKey.keyNum = key;
			break;
		}
	}
  }

  else {
	for(int key = my_rank*num_keys; key < pow(2,56) ; ++key) {
		for (int i = 0; i < 64; ++i) {
			if (i % 8 == 7) {}
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
			inKey.isKey = 1;
      inKey.keyNum = key;
			break;
		}
	}
  }

  MPI::COMM_WORLD.Reduce( &inKey, &outKey, 1, MPI::INT_INT, MPI::MAXLOC, 0);
  cout << "The key to decrypt this file is: " << outKey.keyNum << endl;


  double end_time = MPI::Wtime();
	double time = end_time - begin_time;
  MPI::Finalize();

	return 0;
}