// main.cpp
#include <iostream>
#include "Nova/core.h"        // C API: nova_load_file_to_arena



int main(const int argc, char* argv[]) {
    nova::Arena buffer;
    auto result = nova::file::readSource(buffer);
    auto config =nova::file::readFileToArena(buffer, "config.txt");
    std::cout << config.first << std::endl;
    return 0;
}