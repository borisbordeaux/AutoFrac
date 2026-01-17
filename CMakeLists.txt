cmake_minimum_required(VERSION 3.25)
project(AutoFrac)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
file(GLOB_RECURSE VENDOR_FILES "vendor/*.cpp")

add_executable(AutoFrac ${SOURCE_FILES} ${VENDOR_FILES})

target_include_directories(AutoFrac PUBLIC include SYSTEM PRIVATE vendor)

set_source_files_properties(${VENDOR_FILES} PROPERTIES COMPILE_OPTIONS "-w")

target_link_libraries(AutoFrac PRIVATE -lglfw3 -lGLEW -lGL -lfmt -lspdlog -larmadillo -llua5.4)

target_compile_options(AutoFrac PRIVATE -Wall -Wextra -Werror)

option(ENABLE_TESTS "Build unit tests" OFF)

if (ENABLE_TESTS)
    enable_testing()

    include(FetchContent)
    FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/refs/heads/main.zip)
    FetchContent_MakeAvailable(googletest)

    file(GLOB_RECURSE TEST_FILES "tests/*.cpp")
    file(GLOB_RECURSE TESTED_FILES "src/app/bcifs/*.cpp" "src/app/automaton/*.cpp")
    add_executable(runTests ${TEST_FILES} ${TESTED_FILES})
    target_include_directories(runTests PUBLIC "include")
    target_link_libraries(runTests PRIVATE GTest::gtest_main)
    target_compile_options(runTests PRIVATE -Wall -Wextra -Werror)

    include(GoogleTest)
    gtest_discover_tests(runTests)
endif ()