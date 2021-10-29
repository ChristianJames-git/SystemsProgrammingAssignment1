//
// Created by socce on 10/15/2021.
//

#include "disass.h"

/*
 * Controller public function
 */
void disass::disassemble() {
    for (int i = 0 ; i < objCode.size() ; i++) {
        switch (objCode[i][0]) { //Ignore 'M' rows. Handle 'H', 'T', and 'E'
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
void disass::handleHeader(int line) {
    progName = objCode[line].substr(1, 6); //progName
    progLength = strtol(objCode[line].substr(13, 6).c_str(), nullptr, 16); //progLength
    startAddress = strtol(objCode[line].substr(7, 6).c_str(), nullptr, 16); //strtol(string to convert, end, base)
    currAddress = startAddress;
    printAddress(currAddress);
    lstStream << uppercase << progName << "  " << "START   " << startAddress << endl; //print first output line
}

/*
 * Handles a Text line of the object code
 */
void disass::handleRESB() {
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
void disass::handleText(int line) {
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
void disass::handleEnd(int line) {
    startAddress = progLength;
    handleRESB(); //add RESB if there is a gap in addresses
    lstStream << setfill(' ') << setw(16) << right << "END" << setw(11) << progName << endl; //print end line
}

/*
 * Implements openFile
 * Opens obj and sym files, exiting if they aren't found
 * Creates and opens output stream to output file
 */
void disass::openFile(char *objFile, char *symFile) { //read in both input files and open output stream
    inStream.open(objFile);
    if (!inStream.is_open()) { //check if .obj file was opened successfully
        cout << ".obj file missing" << endl;
        exit(EXIT_FAILURE);
    }
    readIn(&objCode); //read in .obj file into storage
    inStream.close();
    inStream.open(symFile);
    if (!inStream.is_open()) { //check if .sym file was opened successfully
        cout << ".sym file missing" << endl;
        exit(EXIT_FAILURE);
    }
    readIn(&symStorage); //read in .sym file into storage
    inStream.close();
    lstStream.open("out.lst", ios::out);
    if (!lstStream.is_open()) { //check if output file was created and opened successfully
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
        a.address = strtol(symStorage[i].substr(8, 6).c_str(), nullptr, 16); //stores address
        symTab.push_back(a); //add struct to vector symTab
    }

    for (int j = i+2 ; j < symStorage.size() ; j++) { //loops through rows of lit table
        struct lit a;
        a.length = symStorage[j][20] - '0';
        int temp = 0;
        while (symStorage[j][temp] != ' ') //counts name char length
            temp++;
        if (temp == 0) { //handle literal declarations
            a.name = "*";
            a.litconst = symStorage[j].substr(8, a.length+4);
        } else { //non-declaration literals
            a.name = symStorage[j].substr(0, temp); //stores name
            a.litconst = symStorage[j].substr(8, a.length+3);
        }
        a.address = strtol(symStorage[j].substr(24, 6).c_str(), nullptr, 16);
        litTab.push_back(a); //add struct to vector litTab
    }
}
void disass::closeOutStream() { //closes lstStream
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
void disass::readIn(vector<string> *storage) {
    string nextLine;
    while (inStream.good()) { //read in each line until eof
        getline(inStream, nextLine);
        if (nextLine.empty()) //skip over blank lines
            continue;
        storage->push_back(nextLine); //add each line to given storage vector
    }
};

/*
 * Helper function to print 4 digit addresses in hex format with buffer 0's
 */
void disass::printAddress(int address) { //format 4 digit address printing
    lstStream << setw(4) << setfill('0') << right << hex << address << " ";
}
void disass::printCol2(const string& toPrint) { //format 8 digit Column 2 (and sometimes Column 3) printing
    lstStream << left << setw(8) << setfill(' ') << toPrint;
}
void disass::printCol3(const string& mnemonic, int format) { //add '+' if format 4, then call printCol2
    string s = "+";
    if (format == 4)
        s += mnemonic;
    else
        s = mnemonic;
    printCol2(s);
}
string disass::findCol4(bitset<6> nixbpe, int disp, int format) { //handle Column 4
    string toPrint;
    if (format == 2) //handle CLEAR
        if (disp < Opcode::registerName->length())
            return Opcode::registerName[disp/16]; // div16 as CLEAR only uses the first half byte
    int type;
    int address;
    if (nixbpe[5] & nixbpe[4] || !nixbpe[5] & !nixbpe[4]) {
        type = 0; //simple
    } else if (nixbpe[5] & !nixbpe[4]) {
        toPrint += "@";
        type = 1; //indirect
    } else {
        toPrint += "#";
        type = 2; //immediate
    }
    if (nixbpe[0]) { //direct for format 4
        address = disp;
    } else {
        if (type == 2) { //Immediate Addressing
            toPrint += to_string(disp);
            return toPrint;
        }
        bitset<12> a = disp;
        if (a[11]) { //handle signed disp
            a.flip();
            bool carry = true;
            for (int i = 0 ; ; i++) { //add 1
                if (carry) {
                    a[i] = !a[i];
                    if (a[i])
                        carry = false;
                } else
                    break;
            }
            disp = (int)a.to_ulong() * -1;
        }
        if (nixbpe[1]) //PC Relative
            address = disp + pcAddress;
        if (nixbpe[2]) //Base Relative
            address = disp + baseAddress;
    }
    if (nixbpe[3]) //indexed
        address += xIndex;
    toPrint += findInTab(address); //find which symbol/lit to use
    if (nixbpe[3]) //printout indexed ,X
        toPrint += ",X";
    return toPrint;
}
string disass::findInTab(int address) { //return symbol/name given address
    for (auto & s : symTab) //search symTab
        if (s.address == address)
            return s.symbol;
    for (auto & l : litTab) //search litTab
        if (l.address == address) {
            if (l.name == "*")
                return l.litconst;
            return l.name;
        }
    return "Invalid";
}
void disass::printCol4(const string& toPrint) { //print Column 4, 14 digit
    lstStream << left << setw(14) << setfill(' ') << toPrint;
}
void disass::printObjCol(int obCode, int format) { //print Object Code Column
    lstStream << right << setw(format*2) << setfill('0') << obCode << endl;
}