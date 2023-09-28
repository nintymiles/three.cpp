# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.12)

# 项目工程名
project (remove_quartine)


if(MSVC)
    #设置VC输入代码编码格式为utf-8
    set(CMAKE_C_FLAGS /source-charset:utf-8)
endif()

# just for visual studio
# source_group("Include" ${THREE_HEADER})

# 构建可执行程序
ADD_EXECUTABLE(remove_quartine ${ROOT_DIR}/src/application/remove_quartine/remove_quartine.cpp)
# 如果headers作为源码的一部分，那么当然也是作为source来管理的
target_sources(remove_quartine PRIVATE ${IMGUI_SOURCE})

target_include_directories(remove_quartine PRIVATE 
${ROOT_DIR}/src/imgui
${ROOT_DIR}/src/imgui/backends
${ROOT_DIR}/src/utils)

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    # message(STATUS "${CMAKE_SYSTEM_NAME} lib glfw3.3")
    TARGET_LINK_LIBRARIES(remove_quartine PUBLIC
            # "-framework Cocoa"   # not necessary for system library
            # "-framework CoreGraphics"
            glfw.3.3
            GLESv2
            EGL)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    TARGET_LINK_LIBRARIES(remove_quartine PUBLIC
            glfw3
            libGLESv2.dll.lib
            libEGL.dll.lib)
else()
    message(STATUS "TARGET_LINK_LIBRARIES Configuring on/for ${CMAKE_SYSTEM_NAME}")
endif()