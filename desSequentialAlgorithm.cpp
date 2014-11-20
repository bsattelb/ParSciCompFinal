#include <cmath>
#include "desSequentialAlgorithm.h"
#include <iostream>

using namespace std;

void generateSubkey(bool* key, int keyNum, bool* subKey) {
	
	bool* C0 = new bool[28];
	bool* D0 = new bool[28];
	for (int i =0; i<28; i++){
		C0[i] = key[PC1_index[i]];
		D0[i] = key[PC1_index[i+28]];
	}
	
	int totalShift = 0;
	for (int i = 0; i < keyNum; i++) {
		totalShift += keyShifts[i];
	}
	
	bool* Ci = new bool[28];
	bool* Di = new bool[28];
	for (int i = totalShift; i < 28; i++) {
		Ci[i-totalShift] = C0[i];
		Di[i-totalShift] = D0[i];
	}
	for (int i = 0; i < totalShift; i++){
		Ci[28-totalShift+i] = C0[i]; 
		Di[28-totalShift+i] = D0[i]; 
	}
	delete[] C0;
	delete[] D0;

	bool* almostThere = new bool[56];

	for (int i = 0; i < 28; i++) {
		almostThere[i] = Ci[i];
		almostThere[i+28] = Di[i];
	}
	delete[] Ci;
	delete[] Di;

	for (int i = 0; i < 48; i++) {
		subKey[i] = almostThere[PC2_index[i]];
	}

		delete [] almostThere;
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
	
	delete [] block;
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
	
	delete [] block;
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
	bool* r_temp = new bool[32];
	for (int i = 0; i < 32; ++i) {
		r_temp[i] = R[i];
	}
	for (int i = 0; i < 32; ++i) {
		R[i] = r_temp[P_index[i]];
	}
	
	delete [] result;
	delete [] r_temp;
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
		for (int i = 16; i >= 1; --i) {
			generateSubkey(key, i, subkeys[16 - i]);
		}
	}
	
	IP(L, R);
	
	bool temp;
	bool* oldR = new bool[32];
	for (int i = 0; i < 16; ++i) {
		for (int j = 0; j < 32; ++j) {
			oldR[j] = R[j];
		}
		F(R, subkeys[i]);
		for (int j = 0; j < 32; ++j) {
			R[j] = L[j] != R[j];
			L[j] = oldR[j];
		}
	}
	for (int i = 0; i < 32; ++i) {
		temp = R[i];
		R[i] = L[i];
		L[i] = temp;
	}
	
	FP(L, R);
	
	for (int i = 0; i < 16; ++i) {
		delete [] subkeys[i];
	}
	delete [] subkeys;
	delete [] oldR;
}
