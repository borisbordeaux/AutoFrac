cmake_minimum_required(VERSION 3.25)
project(AutoFrac)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE FILES "src/*.cpp" "vendor/imgui/*.cpp")

add_executable(AutoFrac ${FILES})

target_include_directories(AutoFrac PUBLIC include vendor)

target_link_libraries(AutoFrac PRIVATE -lglfw3 -lGLEW -lGL -larmadillo -lfmt -lspdlog)

target_compile_options(AutoFrac PRIVATE -Wall -Wextra -Werror)

option(ENABLE_TESTS "Build unit tests" ON)

if (ENABLE_TESTS)
    enable_testing()

    # ----------------------------------------------------------------------
    # 3. Téléchargement automatique de GoogleTest (méthode recommandée CLion)
    # ----------------------------------------------------------------------
    include(FetchContent)
    FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/refs/heads/main.zip)
    FetchContent_MakeAvailable(googletest)

    # ----------------------------------------------------------------------
    # 4. Créer l’exécutable de tests
    # ----------------------------------------------------------------------
    file(GLOB_RECURSE TEST_FILES "tests/*.cpp")
    file(GLOB_RECURSE TESTED_FILES "src/bcifs/*.cpp" "src/automaton/*.cpp")
    add_executable(runTests ${TEST_FILES} ${TESTED_FILES})
    target_include_directories(runTests PUBLIC "include")
    target_link_libraries(runTests PRIVATE GTest::gtest_main)
    target_compile_options(runTests PRIVATE -Wall -Wextra -Werror)

    include(GoogleTest)
    gtest_discover_tests(runTests)
endif ()