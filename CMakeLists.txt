cmake_minimum_required(VERSION 3.15)
project(Nova)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch CLI11
include(FetchContent)
FetchContent_Declare(
        cli11
        GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
        GIT_TAG        v2.4.1  # ← Use latest stable version; check GitHub for updates
)
FetchContent_MakeAvailable(cli11)

# Your source files
add_executable(Nova
        main.cpp
        internal/utils/file.h
        internal/parse/tokenizer.h
        internal/core.h
        internal/utils/helpers.h
        internal/parse/tokens.h
        internal/dense_map/unordered_dense_map.h
        internal/dense_map/stl.h
        internal/parse/tokens_map.h
        internal/utils/debug.h
        internal/parse/parser.h
        internal/interface/CLI.h
)

# Link CLI11 (it's an interface library)
target_link_libraries(Nova PRIVATE CLI11::CLI11)