//
// Created by socce on 10/15/2021.
//

#include "disass.h"

/*
 * Master function
 */
void Disass::disassemble() {
    for (int i = 0 ; i < objCode.size() ; i++) {
        switch (objCode[i][0]) {
            case 'H':
                handleHeader(i);
                break;
            case 'T':
                handleText(i);
                break;
            case 'E':
                handleEnd(i);
                break;
            default:
                break;
        }
    }
}

void Disass::handleHeader(int line) {
    progName = objCode[line].substr(1, 6);
    startAddress = strtol(objCode[line].substr(7, 6).c_str(), nullptr, 16); //strtol(string to convert, end, base)
    currAddress = startAddress;
    progLength = strtol(objCode[line].substr(13, 6).c_str(), nullptr, 16);
    printAddress(currAddress);
    lstStream << progName << "  " << "START   " << startAddress << endl;
}

void Disass::handleText(int line) {
    int textSize = strtol(objCode[line].substr(7, 2).c_str(), nullptr, 16);
}

void Disass::handleEnd(int line) {
    lstStream << setfill(' ') << setw(16) << "END" << setw(11) << progName << endl;
}

/*
 * Implements openFile
 * Opens obj and sym files, exiting if they aren't found
 * Creates and opens output stream to output file
 */
void Disass::openFile(char *objFile, char *symFile) {
    inStream.open("test.obj");
    if (!inStream.is_open()) {
        cout << ".obj file missing" << endl;
        exit(EXIT_FAILURE);
    }
    readIn(&objCode);
    inStream.close();
    inStream.open(symFile);
    if (!inStream.is_open()) {
        cout << ".sym file missing" << endl;
        exit(EXIT_FAILURE);
    }
    readIn(&symStorage);
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
 * Reads in the current open in-file into the vector pointed at
 */
void Disass::readIn(vector<string> *storage) {
    string nextLine;
    while (inStream.good()) {
        getline(inStream, nextLine);
        storage->push_back(nextLine);
    }
};

void Disass::printAddress(int address) {
    lstStream << hex << setw(4) << setfill('0') << address << " ";
}