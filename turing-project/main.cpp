#include <iostream>
#include <string>
#include "tm.h"
using namespace std;

static void print_help() {
    cerr << "usage: turing [-v|--verbose] [-h|--help] <tm> <input>" << endl;
}

int main(int argc, char* argv[]){
    bool is_verbose = false;
    // parse args
    if(argc == 1){
        print_help();
        return 1;
    }
    string tm_filename = "", input_str = "";
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "-v" || arg == "--verbose") {
            is_verbose = true;
        }
        else if (arg == "-h" || arg == "--help") {
            print_help();
        }
        else {
            if (tm_filename == "") tm_filename = arg;
            else if (input_str == "") input_str = arg;
            else {
                cerr << "error: too many arguments";
                return 1;
            }
        }
    }
    // parse tm
    if (tm_filename != "") {
        TuringMachine tm{tm_filename, is_verbose};
        tm.input(input_str);
        tm.execute();
    }
    return 0;
}