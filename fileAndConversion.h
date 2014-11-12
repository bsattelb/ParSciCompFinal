#include <iostream>
#include <fstream>
#pragma once

using namespace std;

void generateLR(bool* L, bool* R, char* text);
void generateText(bool* L, bool* R, char* text);
void readIn(char* text, ifstream* file);
void writeOut(char* text, ofstream* outFile);
