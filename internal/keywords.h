//
// Created by dioguabo-rei-delas on 11/9/25.
//

#ifndef NOVA_KEYWORDS_H
#define NOVA_KEYWORDS_H
#include "tokens.h"
#include "dense_map/unordered_dense_map.h"

static auto KeywordsInitializer() {
    ankerl::unordered_dense::map<std::string_view, Token> keywords;
    keywords["i32"] = Token::Type;
    keywords["public"] = Token::Modifier;
    keywords["private"] = Token::Type;
    return keywords;
}

static auto OperatorsInitializer() {
    using Map = ankerl::unordered_dense::map<std::string_view, Token>;
    Map ops;

    // Aritméticos
    ops["+"]  = Token::Plus;
    ops["-"]  = Token::Minus;
    ops["*"]  = Token::Multiply;
    ops["/"]  = Token::Divide;
    ops["="]  = Token::Assignment;

    // Comparações
    ops["=="] = Token::Equal;
    ops["!="] = Token::NotEqual;
    ops[">"]  = Token::GreaterThan;
    ops["<"]  = Token::LessThan;
    ops[">="] = Token::GreaterThanOrEqual;
    ops["<="] = Token::LessThanOrEqual;

    // Lógicos
    ops["&&"] = Token::And;
    ops["||"] = Token::Or;

    // Agrupadores
    ops["("] = Token::LParen;
    ops[")"] = Token::RParen;
    ops["{"] = Token::LBrace;
    ops["}"] = Token::RBrace;
    ops["["] = Token::LBracket;
    ops["]"] = Token::RBracket;

    // Separadores
    ops[","]  = Token::Comma;
    ops[":"]  = Token::Colon;
    ops[";"]  = Token::Semicolon;

    return ops;
}

static auto UnknowInitializer() {
    using Map = ankerl::unordered_dense::map<std::string_view, Token>;
    return Map{};
}

const auto Operators = OperatorsInitializer();

const inline auto Keywords = KeywordsInitializer();

const inline auto Unknow = UnknowInitializer();

#endif //NOVA_KEYWORDS_H