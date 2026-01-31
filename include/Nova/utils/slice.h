// include/Nova/utils/slice.h
#ifndef NOVA_UTILS_SLICE_H
#define NOVA_UTILS_SLICE_H

#include "Nova/parse/tokens.h"

#ifdef __cplusplus
extern "C" {
#endif

    // Only expose what's needed
    typedef struct {
        const NovaToken* data;
        size_t len;
    } NovaTokenSlice;


    struct NovaSlice{
        void* data;
        size_t len;
    };


#ifdef __cplusplus
}
#endif

#endif // NOVA_UTILS_SLICE_H