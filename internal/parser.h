#pragma once
#include <memory>

class ASTnode;
//NOTE: for now I will use std::unique_ptr, but later I will replace to another thing + an Arena Allcoator
typedef std::unique_ptr<ASTnode>(Evaluator)();

/*class ASTnode {
    ASTnode *left = nullptr, *right = nullptr;

    inline static auto head = ASTnode();
    size_t i = 0;
    const get_instance() {
        ASTnode j;
        return j;
    }
    Evaluator expr;
    ASTnode() = default;
};*/