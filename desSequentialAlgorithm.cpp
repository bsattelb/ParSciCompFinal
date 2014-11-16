#include <cmath>
#include "desSequentialAlgorithm.h"
#include <iostream>

using namespace std;

void generateSubkey(bool* key, int keyNum, bool* subKey) {
	// These should be a total of 56 long
	bool* C0 = new bool[32];
	bool* D0 = new bool[32];
	for (int i =0; i<32; i++){
		C0[i] = key[PC1_index[i]];
		D0[i] = key[PC1_index[i+32]];
	}
	
	int totalShift = 0;
	for (int i = 0; i < keyNum; i++) {
		totalShift += keyShifts[i];
	}
	
	bool* Ci = new bool[32];
	bool* Di = new bool[32];
	for (int i = totalShift; i < 32; i++) {
		Ci[i-totalShift] = C0[i];
		Di[i-totalShift] = D0[i];
	}
	for (int i = 0; i < totalShift; i++){
		Ci[32-totalShift+i] = C0[i]; 
		Di[32-totalShift+i] = D0[i]; 
	}
	delete[] C0;
	delete[] D0;

	bool* almostThere = new bool[64];

	for (int i = 0; i < 32; i++) {
		almostThere[i] = Ci[i];
		almostThere[i+32] = Di[i];
	}
	delete[] Ci;
	delete[] Di;

	for (int i = 0; i < 48; i++) {
		subKey[i] = almostThere[PC2_index[i]];
	}

}
void IP(bool* L, bool* R) {
	bool* block = new bool[64];
	for (int i = 0; i < 32; ++i) {
		block[i] = L[i];
		block[i + 32] = R[i];
	}
	
	for (int i = 0; i < 32; ++i) {
		L[i] = block[IP_index[i]];
		R[i] = block[IP_index[i+32]];
	}
}
void FP(bool* L, bool* R) {
	bool* block = new bool[64];
	for (int i = 0; i < 32; ++i) {
		block[i] = L[i];
		block[i + 32] = R[i];
	}
	
	for (int i = 0; i < 32; ++i) {
		L[i] = block[FP_index[i]];
		R[i] = block[FP_index[i+32]];
	}
}

void F(bool* R, bool* key) {
	bool* result = new bool[48];
	for (int i = 0; i < 48; ++i) {
		result[i] = key[i] != R[E_index[i]];
	}
	for (int i = 0; i < 8; ++i) {
		int index = (result[i*6]*2 + result[i*6 + 5])*16;
		index += result[i*6 + 1]*8;
		index += result[i*6 + 2]*4;
		index += result[i*6 + 3]*2;
		index += result[i*6 + 4];
		int value = S_indices[i][index];
		R[i*4 + 0] = (value & 8);
		R[i*4 + 1] = (value & 4);
		R[i*4 + 2] = (value & 2);
		R[i*4 + 3] = (value & 1);
	}
	for (int i = 0; i < 32; ++i) {
		R[i] = R[P_index[i]];
	}
	
}
// This is the same as decrypt with the key schedule reversed
void applyDES(bool* L, bool* R, bool* key, bool isEncryption) {
	
	bool** subkeys = new bool*[16];
	for (int i = 0; i < 16; ++i) {
		subkeys[i] = new bool[48];
	}
	
	if (isEncryption) {
		for (int i = 1; i <= 16; ++i) {
			generateSubkey(key, i, subkeys[i-1]);
		}
	}
	else {
		for (int i = 16; i >= 1; -i) {
			generateSubkey(key, i, subkeys[i-1]);
		}
	}
	IP(L, R);
	
	for (int i = 0; i < 16; ++i) {
		F(R, subkeys[i]);
		for (int j = 0; j < 32; ++j) {
			bool temp = R[j];
			R[j] = L[j] != R[j];
			L[j] = temp;
		}
	}
	
	FP(L, R);
}
