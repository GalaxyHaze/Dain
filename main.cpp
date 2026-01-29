// main.cpp
#include <iostream>
#include "Nova/memory/arena_c_functions.h"
#include "Nova/utils/i_o_utils.h"        // C API: nova_load_file_to_arena
#include "Nova/parse/tokenizer.h"   // C API: nova_tokenize
#include "Nova/parse/tokens.h"      // NovaToken, NovaTokenType

// Helper: print token type as string (for debugging)
static const char* token_type_name(NovaTokenType t) {
    switch (t) {
#define CASE(x) case NOVA_TOKEN_##x: return #x;
        CASE(STRING)
        CASE(NUMBER)
        CASE(TYPE)
        CASE(IDENTIFIER)
        CASE(MODIFIER)
        CASE(ASSIGNMENT)
        CASE(EQUAL)
        CASE(NOT_EQUAL)
        CASE(PLUS)
        CASE(MINUS)
        CASE(MULTIPLY)
        CASE(DIVIDE)
        CASE(CONST)
        CASE(LET)
        CASE(AUTO)
        CASE(MUTABLE)
        CASE(GREATER_THAN)
        CASE(LESS_THAN)
        CASE(GREATER_THAN_OR_EQUAL)
        CASE(LESS_THAN_OR_EQUAL)
        CASE(AND)
        CASE(OR)
        CASE(LPAREN)
        CASE(RPAREN)
        CASE(LBRACE)
        CASE(RBRACE)
        CASE(LBRACKET)
        CASE(RBRACKET)
        CASE(COMMA)
        CASE(COLON)
        CASE(SEMICOLON)
        CASE(UNKNOWN)
        CASE(RETURN)
        CASE(END)
        CASE(IF)
        CASE(ELSE)
        CASE(WHILE)
        CASE(FOR)
        CASE(IN)
        CASE(ARROW)
        CASE(PLUS_EQUAL)
        CASE(MINUS_EQUAL)
        CASE(MULTIPLY_EQUAL)
        CASE(DIVIDE_EQUAL)
        CASE(DOT)
        CASE(DOTS)
        CASE(SWITCH)
        CASE(STRUCT)
        CASE(ENUM)
        CASE(UNION)
        CASE(FAMILY)
        CASE(BREAK)
        CASE(CONTINUE)
        CASE(MOD)
        CASE(ENTITY)
        CASE(FLOAT)
        CASE(NOT)
        CASE(HEXADECIMAL)
        CASE(OCTONAL)
        CASE(BINARY)
#undef CASE
        default: return "???";
    }
}

int main(const int argc, char* argv[]) {
    nova::Arena buffer;
    auto result = nova::file::readSource(buffer);
    auto config =nova::file::readFileToArena(buffer, "config.txt");
    std::cout << config.first << std::endl;
    return 0;
}