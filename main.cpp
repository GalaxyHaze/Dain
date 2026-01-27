#include "include/Nova/core.h"
#include "include/Nova/memory/bumpAllocator.h"
#include "include/Nova/utils/i_o_utils.h"

int main(const int argc, const char** argv) {
    //Nova::CoreInterfaceCommand::parse(argc, argv);
    const nova::Arena arena;
    auto [fst, snd] = nova::file::readSource(arena);
    std::string_view source(fst, snd);

}
