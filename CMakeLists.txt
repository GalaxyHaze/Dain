cmake_minimum_required(VERSION 4.0)
project(Nova)

set(CMAKE_CXX_STANDARD 20)

add_executable(Nova src/main.cpp
        internal/file.h
        internal/tokens.h
        internal/tokenizer.h
        internal/helpers.h
        internal/core.h)
