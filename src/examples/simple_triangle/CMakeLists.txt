# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.12)

# 项目工程名
project (simple_triangle)


if(MSVC)
    #设置VC输入代码编码格式为utf-8
    set(CMAKE_C_FLAGS /source-charset:utf-8)
endif()

# just for visual studio
# source_group("Include" ${THREE_HEADER})

# 构建可执行程序
ADD_EXECUTABLE(simple_triangle ${ROOT_DIR}/src/examples/simple_triangle/simple_triangle.cpp)
# 如果headers作为源码的一部分，那么当然也是作为source来管理的
target_sources(simple_triangle PRIVATE ${IMGUI_SOURCE})

target_include_directories(simple_triangle PRIVATE 
${ROOT_DIR}/src/imgui
${ROOT_DIR}/src/imgui/backends)

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    TARGET_LINK_LIBRARIES(simple_triangle PUBLIC
            # "-framework Cocoa"   # not necessary for system library
            # "-framework CoreGraphics"
            glfw.3.3
            GLESv2
            EGL)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    TARGET_LINK_LIBRARIES(simple_triangle PUBLIC
            glfw3
            libGLESv2.dll.lib
            libEGL.dll.lib)
else()
    message(STATUS "TARGET_LINK_LIBRARIES Configuring on/for ${CMAKE_SYSTEM_NAME}")
endif()