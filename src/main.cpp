#include "../internal/core.h"

int main() {
    auto stream = nova::file::readSource();
    Tokenizer::tokenize(stream);

}
