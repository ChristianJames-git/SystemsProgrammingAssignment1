#ifndef ASSIGNMENT_1_DISASS_H
#define ASSIGNMENT_1_DISASS_H

#include "Opcode.h"

class disass {
public:
    void openFile (char *objFile, char *symFile);
    void closeOutStream();
    void disassemble();
private:
    void readIn(vector<string> *storage);
    void handleHeader(int);
    void handleText(int);
    void handleEnd(int);
    void printAddress(int);
    void printCol2(const string&);
    void printCol3(const string&, int);
    string findCol4(bitset<6>, int, int);
    string findInTab(int);
    void printCol4(const string&);
    void printObjCol(int, int);
    void handleRESB();

    ifstream inStream;
    ofstream lstStream;
    vector<string> objCode;
    vector<string> symStorage;
    struct sym {
        string symbol;
        int address;
        int decimal;
    };
    struct lit {
        string name;
        string litconst;
        int length;
        int address;
    };
    vector<sym> symTab;
    vector<lit> litTab;
    string progName;
    int startAddress;
    int baseAddress;
    int pcAddress;
    int xIndex;
    int currAddress;
    int progStart;
    int progLength;
};

#endif //ASSIGNMENT_1_DISASS_H
