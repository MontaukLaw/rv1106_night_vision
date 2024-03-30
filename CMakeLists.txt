cmake_minimum_required(VERSION 3.10)
project(cvv_test)
set(CMAKE_CXX_STANDARD 11)

set(TOOLCHAIN_DIR /home/marc/rv1106/toolchain/env/arm-rockchip830-linux-uclibcgnueabihf)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIR}/bin/arm-rockchip830-linux-uclibcgnueabihf-g++)
set(CMAKE_C_COMPILER ${TOOLCHAIN_DIR}/bin/arm-rockchip830-linux-uclibcgnueabihf-gcc)
set(SYSROOT /home/marc/rv1106/sysroot)
set(MEDIA_ROOT /home/marc/rv1106/media/out)

# drm
set(DRM_DIR ${CMAKE_SOURCE_DIR}/3rdparty/drm)
include_directories(${DRM_DIR}/include)
include_directories(${DRM_DIR}/include/libdrm)

#stb CImg
set(STB_DIR ${CMAKE_SOURCE_DIR}/3rdparty/CImg)
include_directories(${STB_DIR})

set(X11_INC /usr/include)
include_directories(${X11_INC})

set(LIBJPEG_DIR ${CMAKE_SOURCE_DIR}/3rdparty/libjpeg/)
include_directories(${LIBJPEG_DIR}/include)
set(LIBJPEG_LIB ${LIBJPEG_DIR}/lib/libjpeg.so)

# opencv
# set(OPENCV_DIR ${CMAKE_SOURCE_DIR}/3rdparty/opencv)
# include_directories(${OPENCV_DIR}/include/opencv4)
# set(OPENCV_LIB ${OPENCV_DIR}/lib/libopencv_core.a
#                ${OPENCV_DIR}/lib/libopencv_features2d.a
#                ${OPENCV_DIR}/lib/libopencv_highgui.a
#                ${OPENCV_DIR}/lib/libopencv_imgproc.a
#                ${OPENCV_DIR}/lib/libopencv_photo.a
#                ${OPENCV_DIR}/lib/libopencv_video.a)

# set(OPENCV_LIB ${OPENCV_DIR}/lib/libopencv_core.a
#                ${OPENCV_DIR}/lib/libopencv_photo.a
#                ${OPENCV_DIR}/lib/libopencv_imgproc.a
#                ${OPENCV_DIR}/lib/libopencv_video.a
#                ${OPENCV_DIR}/lib/libopencv_highgui.a
#                ${OPENCV_DIR}/lib/libopencv_features2d.a)

set(OpenCV_DIR ${CMAKE_SOURCE_DIR}/3rdparty/opencv/lib/cmake/opencv4)
# message(STATUS "OpenCV_DIR: ${OpenCV_DIR}")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# rknn api
set(RKNN_RT_LIB ${CMAKE_SOURCE_DIR}/lib/librknnmrt.so)

include_directories(${CMAKE_SOURCE_DIR}/3rdparty)
include_directories(${CMAKE_SOURCE_DIR}/include)

#add_executable(app src/app.cc src/cmd_parser.cc src/detect.cc src/postprocess.cc)
add_executable(app src/app.cc src/utils.cc src/preprocess.cc src/detect.cc src/postprocess.cc src/udp_file_sender.cpp)
# target_link_libraries(app ${RKNN_RT_LIB} ${OPENCV_LIB})
target_link_libraries(app ${RKNN_RT_LIB} ${OpenCV_LIBS} ${LIBJPEG_LIB})



