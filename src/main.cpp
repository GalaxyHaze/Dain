#include "../internal/core.h"

int main() {
    const auto src = readSource();
    auto Tokens = Tokenizer::tokenize(src);
    for (const auto i : Tokens)
    {
        std::cout << 1;
    }

}
