#ifndef NOVA_TOKENIZER_H
#define NOVA_TOKENIZER_H

#include <string>
#include <string_view>
#include <stdexcept>
#include <cctype>
#include "tokens.h"
#include "keywords.h"

class Tokenizer {
private:
    [[nodiscard]] static bool has(const char* current, const char* end) noexcept {
        return current < end;
    }

    [[nodiscard]] static char peek(const char* current, const char* end, const size_t offset = 0) noexcept {
        const char* pos = current + offset;
        return pos < end ? *pos : '\0';
    }

    static void consume(Info& info, const char*& current, const size_t offset = 1) noexcept {
        current += offset;
        info += offset;
    }

    static void skipMultiLine(Info& info, const char*& current, const char* end) {
        consume(info, current, 2);

        while (has(current, end)) {
            if (peek(current, end) == '*' && peek(current, end, 1) == '/') {
                consume(info, current, 2);
                return;
            }

            if (peek(current, end) == '\n') {
                info.newLine();
            }

            consume(info, current);
        }

        throw std::runtime_error("Unterminated multi-line comment at line " + std::to_string(info.line));
    }

    static void skipSingleLine(Info& info, const char*& current, const char* end) {
        while (has(current, end) && peek(current, end) != '\n') {
            consume(info, current);
        }
    }

    static void contextExtractor(const char*& lexemeStart, const char* current,
                               std::vector<TokenType>& tokens, Info& info) {
        if (lexemeStart == nullptr) return;

        const size_t length = current - lexemeStart;
        if (length == 0) return;

        std::string_view lexeme(lexemeStart, length);

        if (const auto it = Keywords.find(lexeme); it != Keywords.end()) {
            tokens.emplace_back(it->second, lexeme, info);
        }
        else if (const auto unknow = Unknow.find(lexeme); unknow != Unknow.end()) {
            tokens.emplace_back(unknow->second, lexeme, info);
        }
        else {
            tokens.emplace_back(Token::Identifier, lexeme, info);
        }

        lexemeStart = nullptr;
    }

    static bool tryMatchOperator(const char *current, const char *end,
                                 std::vector<TokenType> &tokens,
                                 Info &info) {
        // Try 2-character operators first
        if (current + 1 < end) {
            std::string_view twoCharOp(current, 2);
            if (const auto it = Operators.find(twoCharOp); it != Operators.end()) {
                tokens.emplace_back(it->second, twoCharOp, info);
                consume(info, current, 2);
                return true;
            }
        }

        // Try 1-character operators
        std::string_view oneCharOp(current, 1);
        if (const auto it = Operators.find(oneCharOp); it != Operators.end()) {
            tokens.emplace_back(it->second, oneCharOp, info);
            consume(info, current);
            return true;
        }

        return false;
    }

public:
    static auto tokenize(const std::string_view src) {
        std::vector<TokenType> tokens;
        tokens.reserve(128);
        Info info{};

        const char* current = src.data();
        const char* end = src.data() + src.size();
        const char* lexemeStart = nullptr;  // Ponteiro para início do lexema atual

        while (current < end) {
            // Handle whitespace
            if (std::isspace(*current)) {
                if (lexemeStart != nullptr) {
                    contextExtractor(lexemeStart, current, tokens, info);
                }

                if (*current == '\n') {
                    info.newLine();
                }
                consume(info, current);
                continue;
            }

            // Handle single-line comments
            if (*current == '/' && peek(current, end, 1) == '/') {
                if (lexemeStart != nullptr) {
                    contextExtractor(lexemeStart, current, tokens, info);
                }
                skipSingleLine(info, current, end);
                continue;
            }

            // Handle multi-line comments
            if (*current == '/' && peek(current, end, 1) == '*') {
                if (lexemeStart != nullptr) {
                    contextExtractor(lexemeStart, current, tokens, info);
                }
                skipMultiLine(info, current, end);
                continue;
            }

            // Handle operators
            if (tryMatchOperator(current, end, tokens, info)) {
                continue;
            }

            // Start new lexeme if needed
            if (lexemeStart == nullptr) {
                lexemeStart = current;
            }

            consume(info, current);
        }

        // Handle last lexeme
        if (lexemeStart != nullptr) {
            contextExtractor(lexemeStart, current, tokens, info);
        }

        return tokens;
    }
};

#endif // NOVA_TOKENIZER_H