//
// Created by socce on 10/15/2021.
//

#ifndef ASSIGNMENT_1_DISASS_H
#define ASSIGNMENT_1_DISASS_H

#include "Opcode.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

class Disass {
public:
    void openFile (char *objFile, char *symFile);
    void closeOutStream();
private:
    void readInObj();
    void readInSym();

    ifstream inStream;
    ofstream lstStream;
    vector<string> objCode;
    int startAddress;
    int baseAddress;
    int currAddress;
    int progLength;
};

#endif //ASSIGNMENT_1_DISASS_H
