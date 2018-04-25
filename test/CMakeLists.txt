cmake_minimum_required(VERSION 3.5)

set(TEST_EXE test.out)
include(CMake/Dependence.cmake)

set(TEST_EXE_SRC test/main.cpp test/plugin_test.cpp)
set(TEST_EXE_LINK_LIBS ${GST_LIBRARIES} ${OpenCV_LIBS} ${Boost_LIBRARIES})

add_executable(${TEST_EXE} ${TEST_EXE_SRC})
target_link_libraries(${TEST_EXE} ${TEST_EXE_LINK_LIBS})