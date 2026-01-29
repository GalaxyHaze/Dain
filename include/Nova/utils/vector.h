// include/Nova/utils/vector.h
#ifndef NOVA_UTILS_VECTOR_H
#define NOVA_UTILS_VECTOR_H

#include <stddef.h>
#include "Nova/memory/arena_c_functions.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct {
        void* data;
        size_t len;
        size_t cap;
        size_t elem_size;
        struct NovaArena* arena;  // ← use 'struct' para evitar dependência circular
    } NovaVector;

    void nova_vector_init(NovaVector* vec, size_t elem_size, struct NovaArena* arena);
    void nova_vector_push(NovaVector* vec, const void* elem);
    void* nova_vector_at(const NovaVector* vec, size_t index);

#ifdef __cplusplus
}
#endif

#endif // NOVA_UTILS_VECTOR_H