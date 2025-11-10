#include "../internal/core.h"

int main() {
    const auto stream = nova::file::readSource();
    //const auto tokens = Tokenizer::tokenize(stream);
    auto poha ="'ewwf";
    int i = static_cast<int>(*poha);
    std::cout << i << std::endl;
    //printTokens(tokens);

}
