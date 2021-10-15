//
// Created by socce on 10/14/2021.
//

#ifndef ASSIGNMENT_1_OPCODE_H
#define ASSIGNMENT_1_OPCODE_H

#include <string>

using namespace std;

class Opcode {
public:
    string getChars(const int& opcode);
    string getBin(int hex);
};

#endif //ASSIGNMENT_1_OPCODE_H
