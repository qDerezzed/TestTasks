cmake_minimum_required(VERSION 3.8)

include("tools/HunterGate.cmake")
HunterGate(
        URL "https://github.com/cpp-pm/hunter/archive/v0.23.312.tar.gz"
        SHA1 "42311324f6b859983d833cffc2151d0b86b8bea4")

project(AVSoft_test)

hunter_add_package(tinyxml2)
find_package(tinyxml2 CONFIG REQUIRED)

hunter_add_package(GTest)
find_package(GTest CONFIG REQUIRED)

set(CMAKE_THREAD_PREFER_PTHREAD ON)
find_package(Threads REQUIRED)

add_executable(AVSoft_task_1 ${CMAKE_CURRENT_SOURCE_DIR}/Task_1/sources/main.cpp)

add_executable(AVSoft_task_2
        ${CMAKE_CURRENT_SOURCE_DIR}/Task_2/sources/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Task_2/sources/departments.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Task_2/sources/invoker.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Task_2/sources/parser.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Task_2/sources/menu.cpp)

add_executable(tests_task_2
        ${CMAKE_CURRENT_SOURCE_DIR}/Task_2/tests/test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Task_2/sources/departments.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Task_2/sources/invoker.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Task_2/sources/parser.cpp)

target_include_directories(AVSoft_task_2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Task_2/include)
target_include_directories(tests_task_2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Task_2/include)

target_link_libraries(AVSoft_task_1 PUBLIC Threads::Threads)

target_link_libraries(AVSoft_task_2 PUBLIC tinyxml2::tinyxml2)
target_link_libraries(tests_task_2 PUBLIC GTest::gtest_main tinyxml2::tinyxml2)

target_compile_options(AVSoft_task_1 PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
        -pedantic -Werror -Wall -Wextra
        -Wno-unused-command-line-argument -Wshadow -Wnon-virtual-dtor>
        $<$<CXX_COMPILER_ID:MSVC>:
        /WX /W4>)
target_compile_features(AVSoft_task_1 PUBLIC cxx_std_20)

target_compile_options(AVSoft_task_2 PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
        -pedantic -Werror -Wall -Wextra
        -Wno-unused-command-line-argument -Wshadow -Wnon-virtual-dtor>
        $<$<CXX_COMPILER_ID:MSVC>:
        /WX /W4>)
target_compile_features(AVSoft_task_2 PUBLIC cxx_std_20)

target_compile_features(tests_task_2 PUBLIC cxx_std_20)

enable_testing()
add_test(NAME unit_tests COMMAND tests)
