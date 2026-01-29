cmake_minimum_required(VERSION 3.15)
project(Nova LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Output dirs
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Dependencies
include(FetchContent)

FetchContent_Declare(cli11
        GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
        GIT_TAG v2.4.1
)
FetchContent_MakeAvailable(cli11)

FetchContent_Declare(unordered_dense
        GIT_REPOSITORY https://github.com/martinus/unordered_dense.git
        GIT_TAG v4.0.0
)
FetchContent_MakeAvailable(unordered_dense)

# LLVM
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")

# Optional: use LLD
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=lld")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fuse-ld=lld")


# Version
add_definitions(-DNOVA_VERSION="v0.0-alpha")

# Include
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Sources
set(CORE_C_SOURCES
        impl/memory/arena_c_functions.c
        impl/memory/file.c
        impl/utils/vector.c
)

set(PARSER_CPP_SOURCES
        impl/parser/tokenizer.cpp
        impl/interface/CLI.cpp
)

# Parser lib
add_library(NovaParse SHARED ${PARSER_CPP_SOURCES})
target_include_directories(NovaParse
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE ${LLVM_INCLUDE_DIRS}
)
target_link_libraries(NovaParse
        PRIVATE
        LLVM
        CLI11::CLI11
        unordered_dense::unordered_dense
)

# Executable
add_executable(Nova main.cpp ${CORE_C_SOURCES})
target_include_directories(Nova PRIVATE ${LLVM_INCLUDE_DIRS})
target_link_libraries(Nova
        PRIVATE
        NovaParse
        CLI11::CLI11
        unordered_dense::unordered_dense
)