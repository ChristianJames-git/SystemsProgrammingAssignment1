#include <iostream>

#include "Opcode.h"
#include "disass.h"
int main(int argc, char **argv) {
    if (argc != 3) {
        cout << "Please input correct number of files" << endl;
        exit(EXIT_FAILURE);
    }
    auto *disass = new Disass;
    disass->openFile(argv[1], argv[2]);
    //disass->disassemble();
    disass->closeOutStream();
    return 0;
}
