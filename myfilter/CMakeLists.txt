cmake_minimum_required(VERSION 3.5)

include(CMake/Dependence.cmake)

#需要用户配置的变量
#PLUGIN_MYFILTER    插件名
#PKG_VERSION        插件版本
#PKG_INSTALL_DIR    插件安装目录
set(PLUGIN_MYFILTER myfilter)
set(PLUGIN_MYFILTER_VERSION '"0.0.1"')
set(PLUGIN_MYFILTER_INSTALL_DIR /usr/lib/x86_64-linux-gnu/gstreamer-1.0)

# 查找当前目录下的所有源文件,并将名称保存到 DIR_LIB_SRCS 变量
aux_source_directory(${PLUGIN_MYFILTER} PLUGIN_MYFILTER_SRCS)
message(">>>>>> src file : ${PLUGIN_MYFILTER_SRCS}")

set(CMAKE_C_COMPILER /usr/bin/g++)
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_C_FLAGS "-Wall -DPACKAGE='\"GStreamer\"' -DVERSION=${PLUGIN_MYFILTER_VERSION} -Wimplicit -fpic -ggdb -fpermissive")
    message(">>>>>> compiler is gcc : ${CMAKE_C_FLAGS}")
else(CMAKE_COMPILER_IS_GNUCXX)
    message(">>>>>> ssss")
endif(CMAKE_COMPILER_IS_GNUCXX)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PLUGIN_MYFILTER_INSTALL_DIR})
# 指定生成 .so 链接库
add_library ("gst${PLUGIN_MYFILTER}" SHARED ${PLUGIN_MYFILTER_SRCS})
target_link_libraries("gst${PLUGIN_MYFILTER}" ${GST_LIBRARIES})
