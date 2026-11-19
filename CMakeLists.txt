cmake_minimum_required(VERSION 3.20)
project(0xb100d)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)
FetchContent_MakeAvailable(googletest)
enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_LINK_LIBRARIES_ONLY_TARGETS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(server src/main.cpp)
add_executable(tests tests/main.cpp)

target_link_libraries(tests GTest::gtest_main)

foreach(t server tests)
  target_compile_options(${t} PRIVATE $<$<CONFIG:Debug>:-fsanitize=address,undefined -fno-omit-frame-pointer>)
  target_link_options(${t}    PRIVATE $<$<CONFIG:Debug>:-fsanitize=address,undefined>)
endforeach()

include(GoogleTest)
gtest_discover_tests(tests)
