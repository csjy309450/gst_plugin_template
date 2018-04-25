cmake_minimum_required(VERSION 3.5)

include(CMake/Dependence.cmake)

#需要用户配置的变量
#PLUGIN_MYFILTER    插件名
#PKG_VERSION        插件版本
#PKG_INSTALL_DIR    插件安装目录
set(OOGST oogst)
set(OOGST_LINK_LIBS ${GST_LIBRARIES})
#set(OOGST_INSTALL_DIR .)

# 查找当前目录下的所有源文件,并将名称保存到 DIR_LIB_SRCS 变量
aux_source_directory(OOGST OOGST_SRCS)
message(">>>>>> src file : ${OOGST_SRCS}")

set(CMAKE_C_COMPILER /usr/bin/g++)
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_C_FLAGS "-Wall -Wimplicit -fpic -ggdb -fpermissive")
    message(">>>>>> compiler is gcc : ${CMAKE_C_FLAGS}")
else(CMAKE_COMPILER_IS_GNUCXX)
    message(">>>>>> ssss")
endif(CMAKE_COMPILER_IS_GNUCXX)

#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OOGST_INSTALL_DIR})
# 指定生成 .so 链接库
add_library ("gst${OOGST}" SHARED ${OOGST_SRCS})
target_link_libraries("gst${OOGST}" ${OOGST_LINK_LIBS})
