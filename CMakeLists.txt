cmake_minimum_required(VERSION 3.12)
project(ffmpeg_cpp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")

find_package(PkgConfig REQUIRED)
pkg_check_modules(AVCODEC REQUIRED libavcodec)
pkg_check_modules(AVFORMAT REQUIRED libavformat)
pkg_check_modules(AVUTIL REQUIRED libavutil)
pkg_check_modules(SWSCALE REQUIRED libswscale)

add_library(ffmpeg src/Codec.h src/Codec.cpp src/Packet.cpp src/Packet.h src/av.h src/Frame.cpp src/Frame.h src/Parser.h src/Format.h src/SwsContext.h src/Format.cpp src/av.cpp)
target_include_directories(ffmpeg PRIVATE ${AVUTIL_INCLUDE_DIRS} ${AVCODEC_INCLUDE_DIRS} ${AVFORMAT_INCLUDE_DIRS} ${SWSCALE_INCLUDE_DIRS})
target_link_libraries(ffmpeg ${AVUTIL_LIBRARIES} ${AVCODEC_LIBRARIES} ${AVFORMAT_LIBRARIES} ${SWSCALE_LIBRARIES})

add_executable(ffmpeg_test test/Codec_test.cpp)
target_link_libraries(ffmpeg_test ffmpeg)