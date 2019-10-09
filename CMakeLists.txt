cmake_minimum_required(VERSION 2.8.11)
project(Hedit)

# General sources.
set(general_sources
    "src/buffer.c"
    "src/file/fileWriter.c"
)

# Platform specific sources.
if(WIN32)
    set(platform_specific_sources
        "src/file/windows/fileMapping.c"
    )
elseif(UNIX)
    set(platform_specific_sources
        "src/file/linux/fileMapping.c"
    )
else()
    set(platform_specific_sources
        "src/file/other/fileMapping.c"
    )
endif()

# Test sources.
set(test_sources
    "test/src/test.c"
    "test/src/test_buffer.c"
)

# Compiler specific.
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_definitions(HEDIT_UNREACHABLE=__builtin_unreachable\(\))
    message("TODO")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_definitions(HEDIT_UNREACHABLE=__builtin_unreachable\(\))
    set(CMAKE_C_FLAGS 
        "-std=c11 -Wall -Wextra -Wpedantic -Wwrite-strings -no-pie -fno-pie"
    )
    set(CMAKE_C_FLAGS_RELEASE
        "-O3 -s -DNDEBUG"
    )
    set(CMAKE_C_FLAGS_MINSIZEREL
        "-Os -s -DNDEBUG"
    )
    set(CMAKE_C_FLAGS_DEBUG
        "-Wno-unused-parameter -g"
    )
else()
    message(FATAL_ERROR "Unsupported compiler!")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")

add_executable(hedit "src/main.c" ${general_sources} ${platform_specific_sources})
target_include_directories(hedit PRIVATE "include")

add_executable(test EXCLUDE_FROM_ALL ${general_sources} ${test_sources})
target_include_directories(test PRIVATE "include" "test/include")