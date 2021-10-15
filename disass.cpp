//
// Created by socce on 10/15/2021.
//

#include "disass.h"

ifstream objStream;
ifstream symStream;
ofstream lstStream("out.lst");

/*
 * Implements openFile
 * Opens obj and sym files, exiting if they aren't found
 * Creates and opens output stream to output file
 */
void Disass::openFile(char *objFile, char *symFile) {
    objStream.open(objFile);
    if (!objStream.is_open()) {
        cout << ".obj file missing" << endl;
        exit(EXIT_FAILURE);
    }
    symStream.open(symFile);
    if (!symStream.is_open()) {
        cout << ".sym file missing" << endl;
        exit(EXIT_FAILURE);
    }
    readIn();
    objStream.close();
    symStream.close();
}

/*
 *
 */
void Disass::readIn() {
    
}