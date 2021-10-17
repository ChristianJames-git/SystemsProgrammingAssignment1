//
// Created by socce on 10/14/2021.
//

#ifndef ASSIGNMENT_1_OPCODE_H
#define ASSIGNMENT_1_OPCODE_H

#include <string>
#include <bitset>
#include <iostream>

using namespace std;

class Opcode {
public:
    struct opCodeInfo {
        string mnemonic;
        int format;
    };
    static opCodeInfo translate(int hex);
private:
    static string getChars(int hex);
    static bitset<4> getBin(int hex);
    static int getFormat(const string& mnemonic, int lastHex);
};

#endif //ASSIGNMENT_1_OPCODE_H
