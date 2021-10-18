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
    lstStream << uppercase << progName << "  " << "START   " << startAddress << endl;
}

/*
 * Handles a Text line of the object code
 */
void Disass::handleText(int line) {
    cout << objCode[line] << endl;
    int textSize = strtol(objCode[line].substr(7, 2).c_str(), nullptr, 16);
    startAddress = strtol(objCode[line].substr(1, 6).c_str(), nullptr, 16);
    if (startAddress != currAddress) { //check to see if there was a jump in address
        cout << "missing RESB handling" << endl;
    }
    currAddress = startAddress;
    for (int i = 9 ; i < 9 + textSize*2 ;) { //Each loops hands one line
        cout << i << endl;
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
        printCol2(toPrint); //prints symbol if found, otherwise blanks

        Opcode::opCodeInfo a = Opcode::translate(strtol(objCode[line].substr(i, 3).c_str(), nullptr, 16));
        printCol3(a.mnemonic, a.format);
        printCol4();
        printObjCol(strtol(objCode[line].substr(i, a.format * 2).c_str(), nullptr, 16), a.format);
        currAddress += a.format;
        i += a.format*2;

        if (a.mnemonic == "LDB") { //handle Base
            lstStream << "     " << setw(8) << setfill(' ') << left << "" << "BASE" << endl;
        }
    }
    for (auto & s : symTab)
        if (s.address == currAddress) {
            printAddress(s.address);
            printCol2(s.symbol);
            printCol2("RESB");
            //print the number
            lstStream << endl;
            currAddress += 3;
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
void Disass::printCol2(const string& toPrint) {
    lstStream << left << setw(8) << setfill(' ') << toPrint;
}
void Disass::printCol3(const string& mnemonic, int format) {
    string s = "+";
    if (format == 4)
        s += mnemonic;
    else
        s = mnemonic;
    printCol2(s);
}
void Disass::printCol4() {
    lstStream << left << setw(14) << setfill(' ') << "BLANK";
}
void Disass::printObjCol(int objCode, int format) {
    lstStream << right << setw(format*2) << setfill('0') << objCode << endl;
}