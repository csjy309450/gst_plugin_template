cmake_minimum_required(VERSION 3.5)

include(CMake/Dependence.cmake)

#需要用户配置的变量
#PLUGIN_MYFILTER    插件名
#PKG_VERSION        插件版本
#PKG_INSTALL_DIR    插件安装目录
set(PLUGIN_CAMERASRC camerasrc)
set(PLUGIN_CAMERASRC_VERSION '"0.0.1"')
set(PLUGIN_CAMERASRC_INSTALL_DIR /usr/lib/x86_64-linux-gnu/gstreamer-1.0)
set(PLUGIN_CAMERASRC_LINK_LIBS ${GST_LIBRARIES} ${OpenCV_LIBS} ${Boost_LIBRARIES})
set(PLUGIN_CAMERASRC_DEBUG OFF)

# 查找当前目录下的所有源文件,并将名称保存到 DIR_LIB_SRCS 变量
aux_source_directory(${PLUGIN_CAMERASRC} PLUGIN_CAMERASRC_SRCS)
message(">>>>>> src file : ${PLUGIN_CAMERASRC_SRCS}")

set(CMAKE_C_COMPILER /usr/bin/g++)
if(CMAKE_COMPILER_IS_GNUCXX)
    if(PLUGIN_CAMERASRC_DEBUG STREQUAL "ON")
        set(CMAKE_C_FLAGS "-Wall -DPACKAGE='\"GStreamer\"' -DVERSION=${PLUGIN_CAMERASRC_VERSION} -DPLUGIN_CAMERASRC_DEBUG \
         -Wimplicit -fpic -ggdb -fpermissive")
    elseif(PLUGIN_CAMERASRC_DEBUG STREQUAL "OFF")
        set(CMAKE_C_FLAGS "-Wall -DPACKAGE='\"GStreamer\"' -DVERSION=${PLUGIN_CAMERASRC_VERSION} -Wimplicit -fpic -ggdb \
        -fpermissive")
    endif()
    message(">>>>>> compiler is gcc : ${CMAKE_C_FLAGS}")
else(CMAKE_COMPILER_IS_GNUCXX)
    message(">>>>>> ssss")
endif(CMAKE_COMPILER_IS_GNUCXX)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PLUGIN_CAMERASRC_INSTALL_DIR})
# 指定生成 .so 链接库
add_library ("gst${PLUGIN_CAMERASRC}" SHARED ${PLUGIN_CAMERASRC_SRCS})
target_link_libraries("gst${PLUGIN_CAMERASRC}" ${PLUGIN_CAMERASRC_LINK_LIBS})
