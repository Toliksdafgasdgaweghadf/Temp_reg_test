# CMakeList.txt : CMake project for CMakeProject1, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
include(GoogleTest)

set(TARGET_NAME temp_test)

file(GLOB_RECURSE MODULE_SOURCE_CPP ${CMAKE_CURRENT_LIST_DIR}/module/cpp/*.cpp)
file(GLOB_RECURSE TESTING_SOURCE_CPP ${CMAKE_CURRENT_LIST_DIR}/test/MOC/cpp/*.cpp)

# Add source to this project's executable.
add_executable (${TARGET_NAME} "main.cpp" 
								${MODULE_SOURCE_CPP} 
								${TESTING_SOURCE_CPP}
)

target_sources(${TARGET_NAME}
  PRIVATE ${MODULE_SOURCE_CPP}
  PRIVATE ${TESTING_SOURCE_CPP}
)

target_link_libraries(
  ${TARGET_NAME}
  GTest::gtest_main
)

target_include_directories(${TARGET_NAME}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/module/header/
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/test/MOC/header/
)

gtest_discover_tests(${TARGET_NAME})