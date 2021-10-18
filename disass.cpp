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
    for (int i = 0 ; i < symTab.size() ; i++) { //Handles RESB cases
        if (symTab[i].address >= currAddress && symTab[i].address < startAddress) {
            printAddress(symTab[i].address);
            printCol2(symTab[i].symbol);
            printCol2("RESB");
            if (i+1 < symTab.size() && symTab[i+1].address < startAddress)
                printCol2(to_string(symTab[i+1].address - symTab[i].address));
            else
                printCol2(to_string(startAddress - symTab[i].address));
            lstStream << endl;
            currAddress = symTab[i].address + 1;
        }
    }
    currAddress = startAddress;
    for (int i = 9 ; i < 9 + textSize*2 ;) { //Each loops hands one line
        printAddress(currAddress); //prints address column

        string toPrint;
        for (auto & s : symTab) //searches for address in symTab
            if (s.address == currAddress) {
                toPrint = s.symbol;
                break;
            }
        bool litfound = false;
        for (auto & l : litTab)
            if (l.address == currAddress) {
                litfound = true;
                printCol2(l.name);
                printCol2("BYTE");
                printCol2(l.litconst);
                lstStream << "      " << l.litconst.substr(2, l.length) << endl;
                currAddress += l.length/2;
                i += l.length;
                break;
            }
        if (litfound) //Skips to next loops if value found in litTab
            continue;
        printCol2(toPrint); //prints symbol if found, otherwise blanks

        Opcode::opCodeInfo a = Opcode::translate(strtol(objCode[line].substr(i, 3).c_str(), nullptr, 16));
        printCol3(a.mnemonic, a.format);
        printCol4(a.nixbpe);
        printObjCol(strtol(objCode[line].substr(i, a.format * 2).c_str(), nullptr, 16), a.format);
        currAddress += a.format;
        i += a.format*2;

        if (a.mnemonic == "LDB") { //handle Base
            lstStream << "     " << setw(8) << setfill(' ') << left << "" << "BASE" << endl;
        }
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
void Disass::printCol4(bitset<6> nixbpe) {
    string toPrint;
    if (nixbpe[5] & nixbpe[4] || !nixbpe[5] & !nixbpe[4]) {
        //Simple Addressing
    } else if (nixbpe[5] & !nixbpe[4]) {
        toPrint += '@';
    } else {
        toPrint += '#';
    }
    lstStream << left << setw(14) << setfill(' ') << toPrint;
}
void Disass::printObjCol(int obCode, int format) {
    lstStream << right << setw(format*2) << setfill('0') << obCode << endl;
}