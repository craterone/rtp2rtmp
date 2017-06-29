cmake_minimum_required(VERSION 3.7)
project(webrtc2rtmp)

set(CMAKE_C_STANDARD 99)

include(FindPkgConfig)


set(PACKAGE_LIST
        gstreamer-1.0
        )

set(PACKAGE_MIN_LIST
        1.10.2
        )

list(LENGTH PACKAGE_LIST len1)
math(EXPR len2 "${len1} - 1")

foreach(val RANGE ${len2})
    list(GET PACKAGE_LIST ${val} package)
    list(GET PACKAGE_MIN_LIST ${val} min_version)

    set(MIN_${package}_VERSION ${min_version})

    string(TOLOWER ${package} package_lib_name)
    pkg_check_modules (${package} ${package_lib_name} REQUIRED)
    if(${package}_VERSION VERSION_LESS MIN_${package}_VERSION )
        message(FATAL_ERROR "${package} version is ${${package}_VERSION} , but required > ${MIN_${package}_VERSION}" )
    endif()

    include_directories(${${package}_INCLUDE_DIRS})
    link_directories(${${package}_LIBRARY_DIRS})
    link_libraries(${${package}_LIBRARIES})

endforeach()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)

set(SOURCE_FILES main.c rtp2rtmp.c)
add_executable(webrtc2rtmp ${SOURCE_FILES})