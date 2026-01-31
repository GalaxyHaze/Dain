// include/Nova/utils/debug.h
#ifndef NOVA_UTILS_DEBUG_H
#define NOVA_UTILS_DEBUG_H

#include "Nova/parse/tokens.h"  // NovaToken, NovaTokenType
#include "Nova/utils/slice.h"   // NovaTokenSlice
#include <iostream>
#include <string_view>

namespace nova::debug {

// Helper: convert token type to string (for debugging)
inline const char* token_type_name(NovaTokenType t) {
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

// Print tokens from a C-style slice (used by your tokenizer)
inline void printTokens(NovaTokenSlice tokens) {
    std::cout << "=== Token Stream ===\n";
    for (size_t i = 0; i < tokens.len; ++i) {
        const NovaToken& tok = tokens.data[i];
        std::string_view lexeme(tok.value, tok.value_len);

        std::cout << "[" << tok.info.line << ":" << tok.info.index << "] "
                  << token_type_name(tok.token)
                  << " = \"" << lexeme << "\"\n";

        if (tok.token == NOVA_TOKEN_END) break;
    }
    std::cout << "====================\n";
}

// Optional: overload for std::vector (for legacy/testing)
inline void printTokens(const std::vector<NovaToken>& tokens) {
    NovaTokenSlice slice{tokens.data(), tokens.size()};
    printTokens(slice);
}

} // namespace nova::debug

#endif // NOVA_UTILS_DEBUG_H