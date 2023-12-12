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

add_executable(test_forward_list test/forward_list.cpp)
target_link_libraries(test_forward_list PRIVATE gtest_main gmock_main)
add_test(NAME test_forward_list COMMAND test_forward_list)

add_executable(test_array test/array.cpp)
target_link_libraries(test_array PRIVATE gtest_main gmock_main)
add_test(NAME test_array COMMAND test_array)

add_executable(test_deque test/deque.cpp)
target_link_libraries(test_deque PRIVATE gtest_main gmock_main)
add_test(NAME test_deque COMMAND test_deque)

add_executable(test_heap test/heap.cpp)
target_link_libraries(test_heap PRIVATE gtest_main gmock_main)
add_test(NAME test_heap COMMAND test_heap)

add_executable(test_stack test/stack.cpp)
target_link_libraries(test_stack PRIVATE gtest_main gmock_main)
add_test(NAME test_stack COMMAND test_stack)

add_executable(test_queue test/queue.cpp)
target_link_libraries(test_queue PRIVATE gtest_main gmock_main)
add_test(NAME test_queue COMMAND test_queue)

add_executable(test_priority_queue test/priority_queue.cpp)
target_link_libraries(test_priority_queue PRIVATE gtest_main gmock_main)
add_test(NAME test_priority_queue COMMAND test_priority_queue)

add_executable(test_set test/set.cpp)
target_link_libraries(test_set PRIVATE gtest_main gmock_main)
add_test(NAME test_set COMMAND test_set)

add_executable(test_map test/map.cpp)
target_link_libraries(test_map PRIVATE gtest_main gmock_main)
add_test(NAME test_map COMMAND test_map)

add_executable(test_multiset test/multiset.cpp)
target_link_libraries(test_multiset PRIVATE gtest_main gmock_main)
add_test(NAME test_multiset COMMAND test_multiset)

add_executable(test_multimap test/multimap.cpp)
target_link_libraries(test_multimap PRIVATE gtest_main gmock_main)
add_test(NAME test_multimap COMMAND test_multimap)

add_executable(test_unordered_set test/unordered_set.cpp)
target_link_libraries(test_unordered_set PRIVATE gtest_main gmock_main)
add_test(NAME test_unordered_set COMMAND test_unordered_set)

add_executable(test_unordered_map test/unordered_map.cpp)
target_link_libraries(test_unordered_map PRIVATE gtest_main gmock_main)
add_test(NAME test_unordered_map COMMAND test_unordered_map)

add_executable(test_unordered_multiset test/unordered_multiset.cpp)
target_link_libraries(test_unordered_multiset PRIVATE gtest_main gmock_main)
add_test(NAME test_unordered_multiset COMMAND test_unordered_multiset)

add_executable(test_unordered_multimap test/unordered_multimap.cpp)
target_link_libraries(test_unordered_multimap PRIVATE gtest_main gmock_main)
add_test(NAME test_unordered_multimap COMMAND test_unordered_multimap)