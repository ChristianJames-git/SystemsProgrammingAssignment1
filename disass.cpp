//
// Created by socce on 10/15/2021.
//

#include "disass.h"

/*
 * Master function
 */
void Disass::disassemble() {
    for (auto & i : objCode) {
        switch (i[0]) {
            case 'H':
                //Deal with Header
                break;
            case 'T':
                //Deal with Text
                break;
            case 'E':
                //Deal with End
                break;
            default:
                break;
        }
    }
}

/*
 * Implements openFile
 * Opens obj and sym files, exiting if they aren't found
 * Creates and opens output stream to output file
 */
void Disass::openFile(char *objFile, char *symFile) {
    inStream.open(objFile);
    if (!inStream.is_open()) {
        cout << ".obj file missing" << endl;
        exit(EXIT_FAILURE);
    }
    readInObj();
    inStream.close();
    inStream.open(symFile);
    if (!inStream.is_open()) {
        cout << ".sym file missing" << endl;
        exit(EXIT_FAILURE);
    }
    readInSym();
    inStream.close();
    lstStream.open("out.lst", ios::out);
    if (!lstStream.is_open()) {
        cout << "Error in creating file" << endl;
        exit(EXIT_FAILURE);
    }
}
void Disass::closeOutStream() {
    if (lstStream.is_open())
        lstStream.close();
    else {
        cout << "Output not open" << endl;
        exit(EXIT_FAILURE);
    }
}

/*
 * Reads in Object Code into vector
 */
void Disass::readInObj() {
    string nextLine;
    while (inStream.good()) {
        getline(inStream, nextLine);
        objCode.push_back(nextLine);
    }
};

/*
 * Reads in SymTab and LitTab
 */
void Disass::readInSym() {

}