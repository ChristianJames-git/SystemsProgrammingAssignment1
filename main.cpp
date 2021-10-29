#include <iostream>

#include "disass.h"
int main(int argc, char **argv) {
    if (argc != 3) {
        cout << "Please input correct number of files" << endl;
        exit(EXIT_FAILURE);
    }
    auto *dis = new disass;
    dis->openFile(argv[1], argv[2]);
    dis->disassemble();
    dis->closeOutStream();
    return 0;
}
