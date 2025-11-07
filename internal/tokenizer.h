//
// Created by dioguabo-rei-delas on 11/6/25.
//

#ifndef NOVA_TOKENIZER_H
#define NOVA_TOKENIZER_H
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>
#include "tokens.h"
#include "helpers.h"

class Tokenizer{
    //inline static std::string_view src;

    struct Info
    {
        long index;
        long line;
        long column;
    };

    static bool has(long& index, std::string_view src){
        return ( index++ < src.length() );
    }

    static void skipComment(long& index, const std::string_view src){
        while (has(index, src))
        {
            if (isNewLine(src[index]))
            {
                return;
            }
        }

    }

    static void skipCommentMultiLine(long& index, const std::string_view src){
        while (has(index, src))
        {
            if (peak(index, src) == '*' && peak(index, src, 1) == '/')
            {
                consume(index, src,2);
                return;
            }
        }

        throw std::runtime_error("Error:");

    }

    static char peak(const long index, const std::string_view src, const long offset = 0)
    {
        if (index + offset < src.length())
        {
            return src[index + offset];
        }
        return '\0';
    }

    static bool consume(long& index, const std::string_view src, const long offset = 1)
    {
        if (index + offset < src.length())
        {
            index += offset;
            return true;
        }
        return false;
    }



public:

    static auto tokenize(const std::string& src){
        if (src.empty())
            return std::vector<Token>{};
        long index = 0;
        std::vector<Token> tokens;
        tokens.reserve(96);

        do
        {
            if (peak(index, src) == '/' && peak(index, src, 1) == '/')
            {
                consume(index, src);
                skipComment(index, src);
                continue;
            }

            if  (peak(index, src) == '/' && peak(index, src, 1) == '*')
            {
                skipCommentMultiLine(index, src);
                continue;
            }
        } while (has(index, src));
        std::cout << "End of loop\n";
        return tokens;
    }

};

#endif //NOVA_TOKENIZER_H