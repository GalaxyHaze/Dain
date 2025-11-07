//
// Created by dioguabo-rei-delas on 11/5/25.
//

#ifndef NOVA_TOKENS_H
#define NOVA_TOKENS_H

#include <cstdint>

// --- Token Types ---
enum class Token : uint8_t {
    String,
    Number,
    Type,

    Assignment,
    Equal,
    NotEqual,
    Plus,
    Minus,
    Multiply,
    Divide,

    GreaterThan,
    LessThan,
    GreaterThanOrEqual,
    LessThanOrEqual,

    And,
    Or,

    LParen,
    RParen,
    LBrace,
    RBrace,
    LBracket,
    RBracket,

    Comma,
    Colon,
    Semicolon,

    Unknown // always nice to have a fallback
};

// --- Virtual Machine / IR Instructions ---
enum class Instruction : uint8_t {
    Add,
    Sub,
    Mul,
    Div,
    PointTo,
    AccessIn,
    Emplace,
    Store,
    // Future: Load, Jump, Call, Return, etc.
};

// --- Type Definitions ---
enum class Type : uint8_t {
    u8, u16, u32, u64, u128, u256,
    i8, i16, i32, i64, i128, i256,
    f8, f16, f32, f64, f128, f256,
    Boolean,
    Character,
    Byte,
    Null,
    Pointer
};

// --- Forward Declarations ---
class Symbol;

#endif // NOVA_TOKENS_H
