cmake_minimum_required(VERSION 3.22)
project(AutoFrac)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
file(GLOB_RECURSE VENDOR_FILES "vendor/*.cpp")

add_executable(AutoFrac ${SOURCE_FILES} ${VENDOR_FILES})

target_include_directories(AutoFrac PUBLIC include SYSTEM PRIVATE vendor)

set_source_files_properties(${VENDOR_FILES} PROPERTIES COMPILE_OPTIONS "-w")

target_link_libraries(AutoFrac PRIVATE -lglfw3 -lGLEW -lGL -lfmt -lspdlog -larmadillo -llua5.4)

target_compile_options(AutoFrac PRIVATE -Wall -Wextra -Werror)

option(ENABLE_TESTS "Build unit tests" ON)

if (ENABLE_TESTS)
    enable_testing()

    include(FetchContent)
    FetchContent_Declare(googletest
            URL https://github.com/google/googletest/archive/refs/heads/main.zip
            DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
    FetchContent_MakeAvailable(googletest)

    file(GLOB_RECURSE TEST_FILES "tests/*.cpp")
    file(GLOB_RECURSE TESTED_FILES
            "src/app/bcifs/automaton.cpp"
            "src/app/bcifs/bcifs.cpp"
            "src/app/bcifs/bcifsvertex.cpp"
            "src/app/bcifs/booleanmatrix.cpp"
            "src/app/bcifs/coefpool.cpp"
            "src/app/bcifs/constraintsolver.cpp"
            "src/app/bcifs/formalmatrix.cpp"
            "src/app/bcifs/grid.cpp"
            "src/app/bcifs/state.cpp"
            "src/app/bcifs/subdivisionpoint.cpp"
            "src/app/bcifs/transition.cpp"
            "src/app/massspringsystem/*.cpp"
            "src/core/log.cpp"
    )
    add_executable(runTests ${TEST_FILES} ${TESTED_FILES})
    target_include_directories(runTests PRIVATE "include")
    target_link_libraries(runTests PRIVATE GTest::gtest_main -lfmt -lspdlog -larmadillo)
    target_compile_options(runTests PRIVATE -Wall -Wextra -Werror)

    include(GoogleTest)
    gtest_discover_tests(runTests)
endif ()