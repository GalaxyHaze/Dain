#ifndef NOVA_TOKENIZER_ULTRA_H
#define NOVA_TOKENIZER_ULTRA_H

#include <string_view>
#include <vector>
#include <immintrin.h> // SSE4.2, AVX2

#include "maps.h"
#include "tokens.h"

class UltraTokenizer {
private:
    // Precomputed lookup tables for fast character classification
    alignas(64) static constexpr bool IS_WHITESPACE_TABLE[256] = {
        [' '] = true, ['\t'] = true, ['\n'] = true, ['\r'] = true, ['\v'] = true, ['\f'] = true
    };

    alignas(64) static constexpr bool IS_ALPHA_TABLE[256] = {
        ['a'] = true, ['b'] = true, ['c'] = true, ['d'] = true, ['e'] = true, ['f'] = true,
        ['g'] = true, ['h'] = true, ['i'] = true, ['j'] = true, ['k'] = true, ['l'] = true,
        ['m'] = true, ['n'] = true, ['o'] = true, ['p'] = true, ['q'] = true, ['r'] = true,
        ['s'] = true, ['t'] = true, ['u'] = true, ['v'] = true, ['w'] = true, ['x'] = true,
        ['y'] = true, ['z'] = true,
        ['A'] = true, ['B'] = true, ['C'] = true, ['D'] = true, ['E'] = true, ['F'] = true,
        ['G'] = true, ['H'] = true, ['I'] = true, ['J'] = true, ['K'] = true, ['L'] = true,
        ['M'] = true, ['N'] = true, ['O'] = true, ['P'] = true, ['Q'] = true, ['R'] = true,
        ['S'] = true, ['T'] = true, ['U'] = true, ['V'] = true, ['W'] = true, ['X'] = true,
        ['Y'] = true, ['Z'] = true,
        ['_'] = true
    };

    alignas(64) static constexpr bool IS_DIGIT_TABLE[256] = {
        ['0'] = true, ['1'] = true, ['2'] = true, ['3'] = true, ['4'] = true, ['5'] = true,
        ['6'] = true, ['7'] = true, ['8'] = true, ['9'] = true
    };

    // SIMD-optimized character search
    static size_t find_char_simd(const char* start, const char* end, char target) noexcept {
        const size_t len = end - start;
        if (len < 16) {
            // Small string fallback
            for (size_t i = 0; i < len; ++i) {
                if (start[i] == target) return i;
            }
            return len;
        }

        #ifdef __AVX2__
        const __m256i target_vec = _mm256_set1_epi8(target);
        const char* ptr = start;
        const char* simd_end = start + (len & ~31);

        while (ptr < simd_end) {
            __m256i chunk = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(ptr));
            __m256i cmp = _mm256_cmpeq_epi8(chunk, target_vec);
            int mask = _mm256_movemask_epi8(cmp);

            if (mask != 0) {
                return ptr - start + __builtin_ctz(mask);
            }
            ptr += 32;
        }
        #elif defined(__SSE2__)
        const __m128i target_vec = _mm_set1_epi8(target);
        const char* ptr = start;
        const char* simd_end = start + (len & ~15);

        while (ptr < simd_end) {
            __m128i chunk = _mm_loadu_si128(reinterpret_cast<const __m128i*>(ptr));
            __m128i cmp = _mm_cmpeq_epi8(chunk, target_vec);
            int mask = _mm_movemask_epi8(cmp);

            if (mask != 0) {
                return ptr - start + __builtin_ctz(mask);
            }
            ptr += 16;
        }
        #endif

        // Handle remaining bytes
        for (const char* p = ptr; p < end; ++p) {
            if (*p == target) return p - start;
        }

        return len;
    }

    // Fast whitespace skipping with SIMD
    static const char* skip_whitespace_simd(const char* current, const char* end, Info& info) noexcept {
        #ifdef __SSE4_2__
        const __m128i whitespace_mask = _mm_set1_epi8(' ');
        const __m128i newline_mask = _mm_set1_epi8('\n');
        const __m128i tab_mask = _mm_set1_epi8('\t');

        while (current + 16 <= end) {
            __m128i chunk = _mm_loadu_si128(reinterpret_cast<const __m128i*>(current));

            // Check for whitespace: space, tab, newline, carriage return
            __m128i is_space = _mm_cmpeq_epi8(chunk, whitespace_mask);
            __m128i is_tab = _mm_cmpeq_epi8(chunk, tab_mask);
            __m128i is_newline = _mm_cmpeq_epi8(chunk, newline_mask);
            __m128i is_cr = _mm_cmpeq_epi8(chunk, _mm_set1_epi8('\r'));

            __m128i is_ws = _mm_or_si128(_mm_or_si128(is_space, is_tab),
                                        _mm_or_si128(is_newline, is_cr));

            int mask = _mm_movemask_epi8(is_ws);

            if (mask != 0xFFFF) { // Not all characters are whitespace
                break;
            }

            // Count newlines in this chunk
            int newline_mask = _mm_movemask_epi8(is_newline);
            info.newLine(__builtin_popcount(newline_mask));

            current += 16;
            info += 16;
        }
        #endif

        // Scalar fallback for remaining characters
        while (current < end && IS_WHITESPACE_TABLE[static_cast<unsigned char>(*current)]) {
            if (*current == '\n') info.newLine();
            ++current;
            info++;
        }

        return current;
    }

    // Ultra-fast number parsing
    static void processNumber(const char*& current, const char* end,
                            std::vector<TokenType>& tokens, Info& info) noexcept {
        const char* start = current;
        bool has_dot = false;
        bool has_exponent = false;

        // Parse integer part
        while (current < end && IS_DIGIT_TABLE[static_cast<unsigned char>(*current)]) {
            ++current;
        }

        // Parse fractional part
        if (current < end && *current == '.' &&
            current + 1 < end && IS_DIGIT_TABLE[static_cast<unsigned char>(current[1])]) {
            has_dot = true;
            ++current;

            while (current < end && IS_DIGIT_TABLE[static_cast<unsigned char>(*current)]) {
                ++current;
            }
        }

        // Parse exponent
        if (current < end && (*current == 'e' || *current == 'E')) {
            const char* exp_start = current;
            ++current;

            // Optional sign
            if (current < end && (*current == '+' || *current == '-')) {
                ++current;
            }

            // Must have at least one digit after exponent
            if (current < end && IS_DIGIT_TABLE[static_cast<unsigned char>(*current)]) {
                has_exponent = true;
                while (current < end && IS_DIGIT_TABLE[static_cast<unsigned char>(*current)]) {
                    ++current;
                }
            } else {
                // Not a valid exponent, roll back
                current = exp_start;
            }
        }

        const std::string_view lexeme(start, static_cast<size_t>(current - start));
        tokens.emplace_back(Token::Number, lexeme, info);
        info += (current - start);
    }

    // Optimized identifier/keyword processing
    static void processIdentifierFast(const char*& current, const char* end,
                                    std::vector<TokenType>& tokens, Info& info) noexcept {
        const char* start = current;

        // Fast path: process multiple characters at once when possible
        while (current < end) {
            unsigned char c = *current;
            if (!(IS_ALPHA_TABLE[c] || IS_DIGIT_TABLE[c])) {
                break;
            }
            ++current;
        }

        const std::string_view lexeme(start, static_cast<size_t>(current - start));

        // Fast keyword lookup with perfect hashing potential
        if (const auto keyword = lookupKeyword(lexeme); keyword != Token::Unknown) {
            tokens.emplace_back(keyword, lexeme, info);
        } else {
            tokens.emplace_back(Token::Identifier, lexeme, info);
        }

        info += (current - start);
    }

    // Optimized string processing
    static void processStringFast(const char*& current, const char* end,
                                std::vector<TokenType>& tokens, Info& info) noexcept {
        const char* start = current;
        ++current; // Skip opening quote
        info++;

        // Use SIMD to find closing quote faster
        const size_t closing_quote_pos = find_char_simd(current, end, '"');
        const char* string_end = current + closing_quote_pos;

        if (string_end >= end) {
            //lexError(info, std::string_view{"Unterminated string at line "});
            return;
        }

        // Count newlines in the string
        size_t newline_count = 0;
        const char* nl_search = current;
        while (nl_search < string_end) {
            size_t next_nl = find_char_simd(nl_search, string_end, '\n');
            if (next_nl == (string_end - nl_search)) break;

            ++newline_count;
            nl_search += next_nl + 1;
        }

        info.newLine(newline_count);

        const std::string_view lexeme(start, static_cast<size_t>(string_end + 1 - start));
        tokens.emplace_back(Token::String, lexeme, info);

        current = string_end + 1;
        info += (string_end - current + 2); // +2 for the two quotes
    }

public:
    [[nodiscard]] static auto tokenize(const std::string_view src) noexcept {
        std::vector<TokenType> tokens;
        // Better capacity estimation
        tokens.reserve(src.size() / 8);
        Info info{};

        const char* current = src.data();
        const char* end = src.data() + src.size();

        while (current < end) {
            // Fast whitespace skipping with SIMD
            current = skip_whitespace_simd(current, end, info);
            if (current >= end) break;

            const unsigned char c = *current;

            // Single-character branch prediction hints
            if (IS_ALPHA_TABLE[c] || c == '_') [[likely]] {
                processIdentifierFast(current, end, tokens, info);
                continue;
            }

            if (IS_DIGIT_TABLE[c]) [[likely]] {
                processNumber(current, end, tokens, info);
                continue;
            }

            // Two-character lookahead for comments and operators
            if (current + 1 < end) [[likely]] {
                const std::string_view twoChars(current, 2);

                // Comments (handle before operators since '/' is also an operator)
                if (twoChars == "//") {
                    // Fast single-line comment skipping
                    const char* comment_end = current + 2;
                    const size_t newline_pos = find_char_simd(comment_end, end, '\n');
                    size_t advance = newline_pos;
                    if (comment_end + newline_pos >= end) {
                        advance = end - comment_end;
                    }

                    // Count newlines in comment
                    size_t nl_count = 0;
                    const char* nl_ptr = comment_end;
                    const char* comment_real_end = comment_end + advance;
                    while (nl_ptr < comment_real_end) {
                        if (*nl_ptr == '\n') ++nl_count;
                        ++nl_ptr;
                    }

                    info.newLine(nl_count);
                    current = comment_end + advance;
                    info += (advance + 2);
                    continue;
                }

                if (twoChars == "/*") {
                    // Multi-line comment with SIMD search for "*/"
                    const char* comment_start = current;
                    current += 2;
                    info += 2;

                    while (current + 1 < end) {
                        const size_t star_pos = find_char_simd(current, end, '*');
                        if (current + star_pos + 1 >= end) break;

                        if (current[star_pos + 1] == '/') {
                            // Count newlines in this segment
                            size_t nl_count = 0;
                            for (const char* p = comment_start + 2; p < current + star_pos; ++p) {
                                if (*p == '\n') ++nl_count;
                            }

                            info.newLine(nl_count);
                            current += star_pos + 2;
                            info += (star_pos + 2);
                            goto found_comment_end;
                        }

                        current += star_pos + 1;
                        info += star_pos + 1;
                    }

                    //lexError(info, "Unterminated multi-line comment at line ");
                    found_comment_end:
                    continue;
                }

                // Two-character operators
                if (const auto token = lookupOperator(twoChars); token != Token::Unknown) {
                    tokens.emplace_back(token, twoChars, info);
                    current += 2;
                    info += 2;
                    continue;
                }
            }

            // Single-character operators
            const std::string_view oneChar(current, 1);
            if (const auto token = lookupOperator(oneChar); token != Token::Unknown) {
                tokens.emplace_back(token, oneChar, info);
                ++current;
                info++;
                continue;
            }

            // Strings
            if (c == '"') [[likely]] {
                processStringFast(current, end, tokens, info);
                continue;
            }

            // Unknown token
            tokens.emplace_back(Token::Unknown, oneChar, info);
            ++current;
            info++;
        }

        return tokens;
    }
};

#endif // NOVA_TOKENIZER_ULTRA_H