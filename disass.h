//
// Created by socce on 10/15/2021.
//

#ifndef ASSIGNMENT_1_DISASS_H
#define ASSIGNMENT_1_DISASS_H

#include "Opcode.h"
#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <iomanip>

class Disass {
public:
    void openFile (char *objFile, char *symFile);
    void closeOutStream();
    void disassemble();
private:
    void readIn(vector<string> *storage);
    void handleHeader(int);
    void handleText(int);
    void handleEnd(int);
    void printAddress (int);

    ifstream inStream;
    ofstream lstStream;
    vector<string> objCode;
    vector<string> symStorage;
    string progName;
    int startAddress;
    int baseAddress;
    int xIndex;
    int currAddress;
    int progLength;
};

#endif //ASSIGNMENT_1_DISASS_H
