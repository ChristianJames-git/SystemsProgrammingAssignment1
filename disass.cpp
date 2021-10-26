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
    progLength = strtol(objCode[line].substr(13, 6).c_str(), nullptr, 16);
    startAddress = strtol(objCode[line].substr(7, 6).c_str(), nullptr, 16); //strtol(string to convert, end, base)
    currAddress = startAddress;
    printAddress(currAddress);
    lstStream << uppercase << progName << "  " << "START   " << startAddress << endl;
}

/*
 * Handles a Text line of the object code
 */
void Disass::handleRESB() {
    for (int i = 0 ; i < symTab.size() ; i++) { //Handles RESB cases
        if (symTab[i].address >= currAddress && symTab[i].address < startAddress) {
            printAddress(symTab[i].address);
            printCol2(symTab[i].symbol);
            printCol2("RESB");
            int difference;
            if (i+1 < symTab.size() && symTab[i+1].address < startAddress)
                difference = symTab[i+1].address - symTab[i].address;
            else
                difference = startAddress - symTab[i].address;
            printCol2(to_string(difference));
            symTab[i].decimal = difference;
            lstStream << endl;
            currAddress = symTab[i].address + 1;
        }
    }
}
void Disass::handleText(int line) {
    int textSize = strtol(objCode[line].substr(7, 2).c_str(), nullptr, 16);
    startAddress = strtol(objCode[line].substr(1, 6).c_str(), nullptr, 16);
    handleRESB();
    currAddress = startAddress;
    for (int i = 9 ; i < 9 + textSize*2 ;) { //Each loops hands one line
        for (auto & l : litTab) //Handle LTORG
            if (l.address == currAddress)
                if (l.name == "*") {
                    lstStream << "     ";
                    printCol2("");
                    printCol2("LTORG");
                    lstStream << endl;
                }
        printAddress(currAddress); //prints address column

        string toPrint;
        for (auto & s : symTab) //searches for address in symTab
            if (s.address == currAddress) {
                toPrint = s.symbol;
                break;
            }
        bool litfound = false;
        for (auto & l : litTab) //handle litTab
            if (l.address == currAddress) {
                litfound = true;
                string litConst;
                if (l.name == "*") {
                    printCol2("");
                    printCol2(l.name);
                    litConst = l.litconst.substr(3, l.length);
                } else {
                    printCol2(l.name);
                    printCol2("BYTE");
                    litConst = l.litconst.substr(2, l.length);
                }
                lstStream << setw(14) << setfill(' ') << left << l.litconst;
                lstStream << litConst << endl;
                currAddress += l.length/2;
                i += l.length;
                break;
            }
        if (litfound) //Skips to next loops if value found in litTab
            continue;
        printCol2(toPrint); //prints symbol if found, otherwise blanks

        Opcode::opCodeInfo a = Opcode::translate(strtol(objCode[line].substr(i, 3).c_str(), nullptr, 16));
        printCol3(a.mnemonic, a.format);
        int disp;
        if (a.format == 2)
            disp = strtol(objCode[line].substr(i+2, 2).c_str(), nullptr, 16);
        else
            disp = strtol(objCode[line].substr(i+3, a.format*2 - 3).c_str(), nullptr, 16);
        pcAddress = currAddress + a.format;
        string col4 = findCol4(a.nixbpe, disp, a.format);
        printCol4(col4);
        printObjCol(strtol(objCode[line].substr(i, a.format*2).c_str(), nullptr, 16), a.format);
        if (a.mnemonic == "LDX") //load x
            xIndex = strtol(objCode[line].substr(i+3, 3).c_str(), nullptr, 16);
        currAddress += a.format;
        i += a.format*2;

        if (a.mnemonic == "LDB") { //handle Base
            lstStream << "     " << setw(8) << setfill(' ') << left << "" << setw(8) << "BASE";
            if (col4[0] == '@' || col4[0] == '#')
                col4 = col4.substr(1, col4.length()-1);
            printCol4(col4);
            lstStream << endl;
            for (auto & s : symTab)
                if (s.symbol == col4) {
                    baseAddress = s.address;
                    break;
                }
        }
    }
}

/*
 * Handles the End line of the object code
 */
void Disass::handleEnd(int line) {
    startAddress = progLength;
    handleRESB();
    lstStream << setfill(' ') << setw(16) << right << "END" << setw(11) << progName << endl;
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
    for (int j = 0 ; j < symTab.size() ; j++) {
        if (symTab[j].symbol == "BADR")
            symTab[j].decimal = symTab[j + 1].address - symTab[j].address;
    }
    for (int j = i+2 ; j < symStorage.size() ; j++) {
        struct lit a;
        a.length = symStorage[j][20] - '0';
        int temp = 0;
        while (symStorage[j][temp] != ' ') //counts name char length
            temp++;
        if (temp == 0) {
            a.name = "*";
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
string Disass::findCol4(bitset<6> nixbpe, int disp, int format) {
    string toPrint;
    if (format == 2)
        if (disp < Opcode::registerName->length())
            return Opcode::registerName[disp];
    int type;
    int address;
    if (nixbpe[5] & nixbpe[4] || !nixbpe[5] & !nixbpe[4]) {
        type = 0;
    } else if (nixbpe[5] & !nixbpe[4]) {
        toPrint += "@";
        type = 1;
    } else {
        toPrint += "#";
        type = 2;
    }
    if (nixbpe[0]) {
        address = disp;
    } else {
        if (type == 2) { //Immediate Addressing
            toPrint += to_string(disp);
            return toPrint;
        }
        bitset<12> a = disp;
        if (a[11]) {
            a.flip();
            bool carry = true;
            for (int i = 0 ; ; i++) {
                if (carry) {
                    a[i] = !a[i];
                    if (a[i])
                        carry = false;
                } else
                    break;
            }
            disp = (int)a.to_ulong() * -1;
        }
        if (nixbpe[1])
            address = disp + pcAddress;
        if (nixbpe[2])
            address = disp + baseAddress;
        if (type == 1) {
            for (auto & s : symTab)
                if (s.address == address) {
                    //address += s.decimal;
                    break;
                }
        }
    }
    if (nixbpe[3])
        address += xIndex;
    toPrint += findInTab(address);
    if (nixbpe[3])
        toPrint += ",X";
    return toPrint;
}
string Disass::findInTab(int address) {
    for (auto & s : symTab)
        if (s.address == address)
            return s.symbol;
    for (auto & l : litTab)
        if (l.address == address) {
            if (l.name == "*")
                return l.litconst;
            return l.name;
        }
    return "Invalid";
}
void Disass::printCol4(const string& toPrint) {
    lstStream << left << setw(14) << setfill(' ') << toPrint;
}
void Disass::printObjCol(int obCode, int format) {
    lstStream << right << setw(format*2) << setfill('0') << obCode << endl;
}