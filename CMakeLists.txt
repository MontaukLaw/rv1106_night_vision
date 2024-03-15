cmake_minimum_required(VERSION 3.10)
project(cvv_test)
find_package(OpenCV REQUIRED)

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

# rknn api
set(RKNN_RT_LIB ${CMAKE_SOURCE_DIR}/lib/librknnmrt.so)

include_directories(${CMAKE_SOURCE_DIR}/3rdparty)
include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(app src/app.cc src/cmd_parser.cc src/detect.cc src/postprocess.cc)
target_link_libraries(app ${RKNN_RT_LIB} ${LIBJPEG_LIB})




