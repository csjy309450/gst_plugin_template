cmake_minimum_required(VERSION 3.5)

include(CMake/Dependence.cmake)

#PLUGIN_COMPILE_FLAG控制插件编译
set(PLUGIN_COMPILE_FLAG ON)
add_definitions(-DCAMERASRC_TEST)
message(${PLUGIN_COMPILE_FLAG})

if (${PLUGIN_COMPILE_FLAG} STREQUAL "ON")
#    include(myfilter/CMakeLists.txt)
    include(camerasrc/CMakeLists.txt)
#else(${PLUGIN_COMPILE_FLAG} STREQUAL "OFF")

endif ()

include(test/CMakeLists.txt)


