//
// Created by socce on 10/15/2021.
//

#include "disass.h"

/*
 * Controller public function
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

/*
 * Handles the Header line of the object code
 */
void Disass::handleHeader(int line) {
    progName = objCode[line].substr(1, 6);
    startAddress = strtol(objCode[line].substr(7, 6).c_str(), nullptr, 16); //strtol(string to convert, end, base)
    currAddress = startAddress;
    progLength = strtol(objCode[line].substr(13, 6).c_str(), nullptr, 16);
    printAddress(currAddress);
    lstStream << progName << "  " << "START   " << startAddress << endl;
}

/*
 * Handles a Text line of the object code
 */
void Disass::handleText(int line) {
    cout << objCode[line] << endl;
    int textSize = strtol(objCode[line].substr(7, 2).c_str(), nullptr, 16);
    startAddress = strtol(objCode[line].substr(1, 6).c_str(), nullptr, 16);
    if (startAddress != currAddress) //check to see if there was a jump in address
        cout << "missing RESB handling" << endl;
    currAddress = startAddress;
    for (int i = 9 ; i < 9 + textSize ;) {
        printAddress(currAddress); //prints address column

        string toPrint;
        for (auto & s : symTab) //searches for address in symTab
            if (s.address == currAddress) {
                toPrint = s.symbol;
                break;
            }
        for (auto & l : litTab)
            if (l.address == currAddress) {
                toPrint = l.name;
                break;
            }
        lstStream << left << setw(8) << setfill(' ') << toPrint; //prints symbol if found, otherwise blanks

        Opcode::opCodeInfo a = Opcode::translate(strtol(objCode[line].substr(i, 3).c_str(), nullptr, 16));
        if (a.format == 4)
            lstStream << "+";
        lstStream << a.mnemonic << endl;
        currAddress += a.format;
        i += a.format;
    }
    for (auto & s : symTab)
        if (s.address == currAddress) {
            printAddress(s.address);
            lstStream << left << setw(8) << setfill(' ') << s.symbol << "RESB" << endl;
        }
}

/*
 * Handles the End line of the object code
 */
void Disass::handleEnd(int line) {
    lstStream << setfill(' ') << setw(16) << right << "END" << setw(11) << progName << endl;
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
    int i = 2;
    for ( ; symStorage[i].substr(0, 4) != "Name" ; i++) { //loops through rows of sym table
        struct sym a;
        int temp = 0;
        while (symStorage[i][temp] != ' ') //counts symbol char length
            temp++;
        a.symbol = symStorage[i].substr(0, temp); //stores symbol
        a.address = strtol(symStorage[i].substr(8, 6).c_str(), nullptr, 16);
        symTab.push_back(a);
    }
    for (int j = i+2 ; j < symStorage.size() ; j++) {
        struct lit a;
        a.length = (unsigned char)symStorage[j][20];
        int temp = 0;
        while (symStorage[j][temp] != ' ') //counts name char length
            temp++;
        if (temp == 0) {
            a.litconst = symStorage[j].substr(8, a.length+4);
        } else {
            a.name = symStorage[j].substr(0, temp); //stores name
            a.litconst = symStorage[j].substr(8, a.length+3);
        }
        a.address = strtol(symStorage[j].substr(24, 6).c_str(), nullptr, 16);
        litTab.push_back(a);
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
        if (nextLine.empty())
            continue;
        storage->push_back(nextLine);
    }
};

/*
 * Helper function to print 4 digit addresses in hex format with buffer 0's
 */
void Disass::printAddress(int address) {
    lstStream << setw(4) << setfill('0') << right << hex << address << " ";
}