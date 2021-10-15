//
// Created by socce on 10/14/2021.
//

#include "Opcode.h"

const static int ops[] = {0x18, 0x58, 0x90, 0x40, 0xB4, 0x28, 0x88, 0xA0, 0x24, 0x64, 0x9C, 0xC4, 0xC0, 0xF4, 0x3C,
                             0x30, 0x34, 0x38, 0x48, 0x00, 0x68, 0x50, 0x70, 0x08, 0x6C, 0x74, 0x04, 0xD0, 0x20, 0x60,
                             0x98, 0x44, 0xD8, 0xAC, 0x4C, 0xA4, 0xA8, 0xEC, 0x0C, 0x78, 0x54, 0x80, 0xD4,
                             0x14, 0x7C, 0xE8, 0x84, 0x10, 0x1C, 0x5C, 0x94, 0xB0, 0xE0, 0x2C, 0xB8, 0xDC};
const static string mnemonics[] = {"ADD", "ADDF", "ADDR", "AND", "CLEAR", "COMP", "COMPF", "COMPR", "DIV", "DIVF",
                                   "DIVR", "FIX","FLOAT", "HIO", "J", "JEQ", "JGT", "JLT", "JSUB", "LDA", "LDB", "LDCH",
                                   "LDF", "LDL", "LDS", "LDT", "LDX", "LPS", "MUL", "MULF","MULR", "OR", "RD",
                                   "RMO", "RSUB","SHIFTL", "SHIFTR", "SSK", "STA", "STB","STCH", "STF", "STI",
                                   "STL","STS", "STSW","STT", "STX", "SUB", "SUBF", "SUBR", "SVC","TD", "TIX",
                                   "TIXR", "WD"};

string Opcode::getChars(const int& opcode) {
    for (int i = 0; i < sizeof(ops)/sizeof(int); ++i)
        if (ops[i] == opcode)
            return mnemonics[i];
}

string Opcode::getBin(int hex) {
    string binary;
    while (binary.length() < 4) {
        binary += char(hex % 2);
    }
    return binary;
}
