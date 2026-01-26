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

FetchContent_Declare(
        unordered_dense
        GIT_REPOSITORY https://github.com/martinus/unordered_dense.git
        GIT_TAG v4.0.0  # ou versão estável
)
FetchContent_MakeAvailable(unordered_dense)


# Your source files
add_executable(Nova
        main.cpp
        include/Nova/utils/file.h
        include/Nova/parse/tokenizer.h
        include/Nova/core.h
        include/Nova/utils/helpers.h
        include/Nova/parse/tokens.h
        ankler/dense_map/unordered_dense_map.h
        ankler/dense_map/stl.h
        include/Nova/parse/tokens_map.h
        include/Nova/utils/debug.h
        include/Nova/parse/parser.h
        include/Nova/interface/CLI.h
)

# Link CLI11 (it's an interface library)
target_link_libraries(Nova PRIVATE CLI11::CLI11)
target_link_libraries(Nova PRIVATE unordered_dense::unordered_dense)