#include "../internal/core.h"

int main() {
    auto stream = readSource();
    Tokenizer::tokenize(stream);

}
