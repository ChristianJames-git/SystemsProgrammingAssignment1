#ifndef ASSIGNMENT_1_OPCODE_H
#define ASSIGNMENT_1_OPCODE_H

#include <string>
#include <bitset>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <iomanip>

using namespace std;

class Opcode {
public:
    struct opCodeInfo {
        string mnemonic;
        int format;
        bitset<6> nixbpe;
    };

    static opCodeInfo translate(int hex);

    const static string registerName[];
private:
    static string getChars(int hex);

    static bitset<6> getNIXBPE(int hex);

    static bitset<4> getBin(int hex);

    static int getFormat(const string &, int);
};
#endif //ASSIGNMENT_1_OPCODE_H
