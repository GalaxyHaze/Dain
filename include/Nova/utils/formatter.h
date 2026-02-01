// include/Nova/utils/mini_format.h
#ifndef NOVA_UTILS_MINI_FORMAT_H
#define NOVA_UTILS_MINI_FORMAT_H

#include <cstdint>

#include "Nova/memory/arena_c_functions.h"

namespace nova::fmt {

// Result: string view into a buffer (caller owns the buffer!)
struct FormatResult {
    const char* data;
    size_t size;
};

// Simple integer-to-string (base 10, unsigned)
static size_t uint_to_str(char* buf, size_t buf_size, uint64_t value) {
    if (buf_size == 0) return 0;
    if (value == 0) {
        buf[0] = '0';
        return 1;
    }
    char temp[21]; // max digits for uint64_t + null
    size_t len = 0;
    while (value > 0) {
        temp[len++] = '0' + (value % 10);
        value /= 10;
    }
    if (len > buf_size) return 0;
    for (size_t i = 0; i < len; ++i) {
        buf[i] = temp[len - 1 - i];
    }
    return len;
}

// Integer to hex (lowercase)
static size_t uint_to_hex(char* buf, size_t buf_size, uint64_t value) {
    if (buf_size == 0) return 0;
    if (value == 0) {
        buf[0] = '0';
        return 1;
    }
    char temp[17]; // 16 hex digits + null
    size_t len = 0;
    while (value > 0) {
        const int digit = value & 0xF;
        temp[len++] = (digit < 10) ? ('0' + digit) : ('a' + digit - 10);
        value >>= 4;
    }
    if (len > buf_size) return 0;
    for (size_t i = 0; i < len; ++i) {
        buf[i] = temp[len - 1 - i];
    }
    return len;
}

// Minimal snprintf-like formatter (supports %s, %d, %u, %x, %c, %%)
template<size_t N>
constexpr FormatResult format(char (&out)[N], const char* fmt, ...) {
    // We'll use a simple va_list approach — but for safety, we’ll do it manually with overloads
    // Instead, let's provide type-safe overloads (no varargs!)
    // → Better: use function overloading or format_args
    static_assert(N > 0, "Buffer must be non-empty");
    return FormatResult{out, 0}; // placeholder
}

// Type-safe formatting (no varargs!)
// Usage: fmt::format_to(buffer, "Hello {}", name_view);
template<size_t N>
size_t format_to(char (&out)[N], const char* fmt, const char* arg) {
    size_t pos = 0;
    size_t fmt_len = 0;
    while (fmt[fmt_len]) ++fmt_len;

    for (size_t i = 0; i < fmt_len && pos < N; ++i) {
        if (fmt[i] == '{' && i + 1 < fmt_len && fmt[i + 1] == '}') {
            // Insert arg
            size_t arg_len = 0;
            while (arg[arg_len]) ++arg_len;
            if (pos + arg_len > N) break;
            for (size_t j = 0; j < arg_len; ++j) {
                out[pos++] = arg[j];
            }
            i++; // skip '}'
        } else {
            out[pos++] = fmt[i];
        }
    }
    if (pos < N) out[pos] = '\0';
    return pos;
}

template<size_t N>
size_t format_to(char (&out)[N], const char* fmt, int64_t arg) {
    char num_buf[24];
    size_t num_len = 0;
    if (arg < 0) {
        out[0] = '-';
        // Avoid overflow on INT64_MIN
        uint64_t val = (arg == INT64_MIN) ? static_cast<uint64_t>(INT64_MAX) + 1 : static_cast<uint64_t>(-arg);
        num_len = uint_to_str(num_buf, sizeof(num_buf), val);
        if (1 + num_len >= N) return 0;
        for (size_t i = 0; i < num_len; ++i) out[1 + i] = num_buf[i];
        return 1 + num_len;
    } else {
        num_len = uint_to_str(num_buf, sizeof(num_buf), static_cast<uint64_t>(arg));
        if (num_len >= N) return 0;
        for (size_t i = 0; i < num_len; ++i) out[i] = num_buf[i];
        return num_len;
    }
}

template<size_t N>
size_t format_to(char (&out)[N], const char* fmt, uint64_t arg) {
    char num_buf[24];
    size_t num_len = uint_to_str(num_buf, sizeof(num_buf), arg);
    size_t pos = 0;
    size_t fmt_len = 0;
    while (fmt[fmt_len]) ++fmt_len;

    for (size_t i = 0; i < fmt_len && pos < N; ++i) {
        if (fmt[i] == '{' && i + 1 < fmt_len && fmt[i + 1] == '}') {
            if (pos + num_len > N) break;
            for (size_t j = 0; j < num_len; ++j) out[pos++] = num_buf[j];
            i++;
        } else {
            out[pos++] = fmt[i];
        }
    }
    if (pos < N) out[pos] = '\0';
    return pos;
}

template<size_t N>
size_t format_to(char (&out)[N], const char* fmt, char arg) {
    size_t pos = 0;
    size_t fmt_len = 0;
    while (fmt[fmt_len]) ++fmt_len;

    for (size_t i = 0; i < fmt_len && pos < N; ++i) {
        if (fmt[i] == '{' && i + 1 < fmt_len && fmt[i + 1] == '}') {
            out[pos++] = arg;
            i++;
        } else {
            out[pos++] = fmt[i];
        }
    }
    if (pos < N) out[pos] = '\0';
    return pos;
}

// Helper: format to Arena-allocated buffer (if you want dynamic output)
// Requires: NovaArena* and size estimate
char* format_to_arena(NovaArena* arena, const char* fmt, const char* arg, size_t* out_len);

} // namespace nova::fmt

#endif // NOVA_UTILS_MINI_FORMAT_H