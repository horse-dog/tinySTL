cmake_minimum_required(VERSION 3.20)
project(tinySTL)
set(CMAKE_CXX_STANDARD 20)

# link GoogleTest.
include(FetchContent)
FetchContent_Declare(
    googletest
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/googletest
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# include tinySTL.
include_directories(${PROJECT_SOURCE_DIR}/src)

enable_testing()

add_executable(test_vector test/vector.cpp)
target_link_libraries(test_vector PRIVATE gtest_main gmock_main)
add_test(NAME test_vector COMMAND test_vector)

add_executable(test_list test/list.cpp)
target_link_libraries(test_list PRIVATE gtest_main gmock_main)
add_test(NAME test_list COMMAND test_list)

add_executable(test_slist test/forward_list.cpp)
target_link_libraries(test_slist PRIVATE gtest_main gmock_main)
add_test(NAME test_slist COMMAND test_slist)