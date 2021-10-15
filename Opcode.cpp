//
// Created by socce on 10/14/2021.
//

#include "Opcode.h"

const static string ops[] = {"18", "58", "90", "40", "B4", "28","88", "A0", "24", "64", "9C", "C4","C0", "F4", "3C",
                             "30", "34", "38","48", "00", "68", "50", "70", "08","6C", "74", "04", "D0", "20", "60",
                             "98", "44", "D8", "AC", "4C","A4", "A8", "EC", "0C", "78","54", "80", "D4",
                             "14", "7C", "E8","84", "10", "1C", "5C", "94", "B0","E0", "2C", "B8", "DC"};
const static string mnemonics[] = {"ADD", "ADDF", "ADDR", "AND", "CLEAR", "COMP", "COMPF", "COMPR", "DIV", "DIVF",
                                   "DIVR", "FIX","FLOAT", "HIO", "J", "JEQ", "JGT", "JLT", "JSUB", "LDA", "LDB", "LDCH",
                                   "LDF", "LDL", "LDS", "LDT", "LDX", "LPS", "MUL", "MULF","MULR", "OR", "RD",
                                   "RMO", "RSUB","SHIFTL", "SHIFTR", "SSK", "STA", "STB","STCH", "STF", "STI",
                                   "STL","STS", "STSW","STT", "STX", "SUB", "SUBF", "SUBR", "SVC","TD", "TIX",
                                   "TIXR", "WD"};

string Opcode::getChars(string opcode) {
    for (int i = 0; i < ops->size(); ++i)
        if (ops[i] == opcode)
            return mnemonics[i];
}
