// include/Nova/parse/keywords.h
#ifndef NOVA_PARSE_KEYWORDS_H
#define NOVA_PARSE_KEYWORDS_H

#include <cstring>
#include "Nova/parse/tokens.h"  // for NovaTokenType

#ifdef __cplusplus
extern "C" {
#endif

    // Lookup a keyword by string (null-terminated or not)
    // Returns NOVA_TOKEN_IDENTIFIER if not found
    NovaTokenType nova_lookup_keyword(const char* str, size_t len);

    // Convenience wrapper for null-terminated strings
    static NovaTokenType nova_lookup_keyword_cstr(const char* str) {
        return str ? nova_lookup_keyword(str, std::strlen(str)) : NOVA_TOKEN_IDENTIFIER;
    }

#ifdef __cplusplus
}
#endif

#endif // NOVA_PARSE_KEYWORDS_H