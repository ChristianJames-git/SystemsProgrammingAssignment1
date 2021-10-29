//
// Created by socce on 10/14/2021.
//

#include "Opcode.h"

const static int ops[] =          {0x18,   0x58,   0x90,   0x40,     0xB4,     0x28,   0x88,    0xA0,
                                   0x24,   0x64,   0x9C,   0xC4,     0xC0,     0xF4,   0x3C,    0x30,
                                   0x34,   0x38,   0x48,   0x00,     0x68,     0x50,   0x70,    0x08,
                                   0x6C,   0x74,   0x04,   0xD0,     0x20,     0x60,   0x98,    0x44,
                                   0xD8,   0xAC,   0x4C,   0xA4,     0xA8,     0xEC,   0x0C,    0x78,
                                   0x54,   0x80,   0xD4,   0x14,     0x7C,     0xE8,   0x84,    0x10,
                                   0x1C,   0x5C,   0x94,   0xB0,     0xE0,     0x2C,   0xB8,    0xDC}; //opcodes
const static string mnemonics[] = {"ADD",  "ADDF", "ADDR", "AND",    "CLEAR",  "COMP", "COMPF", "COMPR",
                                   "DIV",  "DIVF", "DIVR", "FIX",    "FLOAT",  "HIO",  "J",     "JEQ",
                                   "JGT",  "JLT",  "JSUB", "LDA",    "LDB",    "LDCH", "LDF",   "LDL",
                                   "LDS",  "LDT",  "LDX",  "LPS",    "MUL",    "MULF", "MULR",  "OR",
                                   "RD",   "RMO",  "RSUB", "SHIFTL", "SHIFTR", "SSK",  "STA",   "STB",
                                   "STCH", "STF",  "STI",  "STL",    "STS",    "STSW", "STT",   "STX",
                                   "SUB",  "SUBF", "SUBR", "SVC",    "TD",     "TIX",  "TIXR",  "WD"}; //corresponding mnemonic
const static string format2[] = {"CLEAR", "COMPR", "DIVR", "MULR", "RMO", "SHIFTL", "SHIFTR", "SUBR", "SVC", "TIXR"}; //list of format2s
const string Opcode::registerName[] = {"A", "X", "L", "B", "S", "T", "F", "PC", "SW"}; //register name to index
Opcode::opCodeInfo Opcode::translate(int hex) { //input hex, output mnemonic, nixbpe, and format
    opCodeInfo info;
    info.mnemonic = getChars(hex/16);
    info.nixbpe = getNIXBPE(hex);
    info.format = getFormat(info.mnemonic, info.nixbpe[0]);
    return info;
}

string Opcode::getChars(int hexIn) { //input first 2 hex digits
    bitset<4> binary2 = getBin(hexIn%16);
    binary2[0] = binary2[1] = false; //set rightmost 2 bits to 0 to make opcode
    int opcode = (hexIn - hexIn%16) + int(binary2.to_ulong());
    for (int i = 0; i < sizeof(ops)/sizeof(int); i++) //find corresponding mnemonic to opcode
        if (ops[i] == opcode)
            return mnemonics[i];
    cout << "OpCode Not Found" << endl;
    exit(EXIT_FAILURE);
}

bitset<4> Opcode::getBin(int hex) { //convert hex digit to 4-bit binary
    bitset<4> binary;
    binary = hex;
    return binary;
}

bitset<6> Opcode::getNIXBPE(int hexA) {
    bitset<4> binary1 = getBin(hexA % 16); //3rd hex digit
    bitset<4> binary2 = getBin((hexA/16) % 16); //2nd hex digit
    bitset<6> toReturn;
    toReturn[5] = binary2[1]; toReturn[4] = binary2[0]; //set n and i bits
    for (int i = 3 ; i >= 0 ; i--) //set xbpe bits
        toReturn[i] = binary1[i];
    return toReturn;
}
int Opcode::getFormat(const string& mnemonic, int e) { //get format
    for (int i = 0; i < format2->size() ; ++i) //check if mnemonic is in format2 array
        if (mnemonic == format2[i])
            return 2;
    if (e == 1) //check e for format 4
        return 4;
    return 3;
}
