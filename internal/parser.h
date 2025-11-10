#pragma once
#include <memory>

class ASTnode;
//NOTE: for now I will use std::unique_ptr, but later I will replace to another thing + an Arena Allcoator
typedef std::unique_ptr<ASTnode>(ASTnode::*ASTnodePtr)();

class ASTnode {
    ASTnode *left = nullptr, *right = nullptr;

};