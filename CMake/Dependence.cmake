## 寻找 gst pkg
find_package(PkgConfig REQUIRED)
pkg_check_modules(GST QUIET gstreamer-1.0)
find_library(GST_LIBRARIES_0 gstcontroller-1.0 /usr/lib/x86_64-linux-gnu)
find_library(GST_LIBRARIES_1 gstaudio-1.0 /usr/lib/x86_64-linux-gnu)
find_library(GST_LIBRARIES_2 gstbase-1.0 /usr/lib/x86_64-linux-gnu)
list(APPEND GST_LIBRARIES ${GST_LIBRARIES_0} ${GST_LIBRARIES_1} ${GST_LIBRARIES_2})
message(STATUS "GST library status:")
message(STATUS "    version: ${GST_VERSION}")
include_directories(${GST_INCLUDE_DIRS})
#message("<<${GST_INCLUDE_DIRS}")
#message("<<${GST_LIBRARIES}")

## 寻找 boost pkg
find_package(Boost 1.54.0 REQUIRED thread system log log_setup program_options) # REQUIRED表示至少1.54.0版本以上, 之后的为要启用的模块名
find_library(boost_libs boost* /usr/lib/x86_64-linux-gnu)
include_directories(${Boost_INCLUDE_DIRS})

## 寻找 OpenCV pkg
find_package(OpenCV 3.1.0 REQUIRED)
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
include_directories(${OpenCV_INCLUDE_DIRS})

