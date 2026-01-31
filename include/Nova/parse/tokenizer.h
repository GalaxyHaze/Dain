// include/Nova/parse/tokenizer.h
#ifndef NOVA_PARSE_TOKENIZER_H
#define NOVA_PARSE_TOKENIZER_H

#include "Nova/utils/slice.h"  // NovaTokenSlice, NovaSlice
#include "Nova/parse/tokens.h" // NovaToken, NovaInfo

#ifdef __cplusplus
extern "C" {
#endif

    struct NovaArena;

    // Tokenizes source into tokens allocated in the arena.
    // On error: prints to stderr and aborts (as per your current behavior).
    // Returns slice of tokens (including final END token).
    NovaTokenSlice nova_tokenize(struct NovaArena* arena, const char* source, size_t source_len);

#ifdef __cplusplus
}
#endif

#endif // NOVA_PARSE_TOKENIZER_H