cmake_minimum_required(VERSION 3.16)

project(demo-firmware
    VERSION 1.0.0
    DESCRIPTION "Demo firmware project for CI/CD demonstrations"
    LANGUAGES C
)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

option(BUILD_TESTING "Build the testing tree." ON)
option(WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)

include(GNUInstallDirs)
include(cmake/CompilerFlags.cmake)

add_subdirectory(src)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)