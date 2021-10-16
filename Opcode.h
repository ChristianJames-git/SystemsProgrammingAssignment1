//
// Created by socce on 10/14/2021.
//

#ifndef ASSIGNMENT_1_OPCODE_H
#define ASSIGNMENT_1_OPCODE_H

#include <string>

using namespace std;

class Opcode {
public:
    struct toReturn {
        string mnemonic;
        int format;
    };
    toReturn translate(int hex);
private:
    string getChars(int hex);
    string getBin(int hex);
    int getHex(string bin);
    int getFormat(string mnemonic, int lastHex);
};

#endif //ASSIGNMENT_1_OPCODE_H
