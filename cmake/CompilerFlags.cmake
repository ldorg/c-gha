if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    set(COMMON_FLAGS
        -Wall
        -Wextra
        -Wpedantic
        -Wformat=2
        -Wno-unused-parameter
        -Wshadow
        -Wwrite-strings
        -Wstrict-prototypes
        -Wold-style-definition
        -Wredundant-decls
        -Wnested-externs
        -Wmissing-include-dirs
    )

    if(WARNINGS_AS_ERRORS)
        list(APPEND COMMON_FLAGS -Werror)
    endif()

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        list(APPEND COMMON_FLAGS -O0 -g3 -ggdb)
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        list(APPEND COMMON_FLAGS -O2 -DNDEBUG)
    elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
        list(APPEND COMMON_FLAGS -Os -DNDEBUG)
    endif()

    add_compile_options(${COMMON_FLAGS})
endif()

if(ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(-fsanitize=address -fsanitize=undefined)
        add_link_options(-fsanitize=address -fsanitize=undefined)
    endif()
endif()