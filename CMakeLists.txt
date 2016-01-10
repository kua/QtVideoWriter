project(QtVideoWriter)
cmake_minimum_required(VERSION 2.8)

SET(SOURCES src/FFmpegLauncher.cpp
            src/ImageWriter.cpp)

SET(HEADERS FFmpegLauncher.h
            ImageWriter.h
            VideoWriter.h)

FIND_PACKAGE(Qt4 REQUIRED)
SET(QT_USE_QTNETWORK true)
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_NO_KEYWORDS)

QT4_ADD_RESOURCES(RESOURCES_MOC "sample/resources/sample.qrc")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(video_writer ${SOURCES} ${HEADERS})
set_target_properties(video_writer PROPERTIES AUTOMOC true)
target_link_libraries(video_writer)

add_executable(video_writer-main sample/main.cpp ${RESOURCES_MOC})
target_link_libraries(video_writer-main video_writer ${QT_LIBRARIES})
set_target_properties(video_writer-main PROPERTIES AUTOMOC true)
