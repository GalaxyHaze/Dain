// include/Nova/parse/tokenizer.h
#ifndef NOVA_PARSE_TOKENIZER_H
#define NOVA_PARSE_TOKENIZER_H

#include "Nova/utils/slice.h"    // defines NovaTokenSlice
#include "Nova/memory/arena_c_functions.h" //declare some arena functions
#include "Nova/utils/vector.h"

#ifdef __cplusplus
namespace nova::detail {
#endif

    struct LexError {
        const char* message;
        NovaInfo info;
    };

    struct NovaArena;

    // Main C++ tokenizer entry point (returns tokens + errors)
    NovaVector tokenize(
        utils::NovaSlice src,
        ::NovaArena* arena,
        NovaVector& errors
    );

    // namespace nova::detail

#ifdef __cplusplus
    extern "C" {
#endif

        struct NovaArena;

        nova::utils::NovaTokenSlice nova_tokenize(struct NovaArena* arena, const char* source, size_t source_len);

#ifdef __cplusplus
    }
}
#endif

#endif // NOVA_PARSE_TOKENIZER_H