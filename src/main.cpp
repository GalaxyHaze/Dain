#include "../internal/core.h"

int main() {
    auto i = 0/*11414141232*/;
    auto stream = nova::file::readSource();
    Tokenizer::tokenize(stream);

}
