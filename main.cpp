#include "disass.h"

int main(int argc, char **argv) {
    if (argc != 3) { //make sure correct args amount
        cout << "Please input correct number of files" << endl;
        exit(EXIT_FAILURE);
    }
    auto *dis = new disass;
    dis->openFile(argv[1], argv[2]); //open and read in files
    dis->disassemble(); //call main program
    dis->closeOutStream(); //close out stream
    return 0;
}
