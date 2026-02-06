// include/Nova/ast/ast.h
#ifndef NOVA_AST_AST_H
#define NOVA_AST_AST_H

#include "Nova/utils/slice.h"

#ifdef __cplusplus
extern "C" {
#endif

    struct NovaArena;

    typedef enum {
        NOVA_NODE_INVALID = 0x00,
        NOVA_NODE_EXPRESSION = 0x01,
        NOVA_NODE_STATEMENT = 0x02,
        NOVA_NODE_DECLARATION = 0x03,
        NOVA_NODE_TYPE = 0x04,
        NOVA_NODE_UNKNOWN = 0xFF
    } NovaNodeType;

    typedef struct ASTNode ASTNode;
    typedef ASTNode* NovaNode;

    NovaNode nova_parse(NovaTokenSlice tok, NovaArena* arena);
    NovaNodeType nova_node_get_type(NovaNode node);

#ifdef __cplusplus
}
#endif

#endif // NOVA_AST_AST_H
