# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.12)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 这个策略貌似也没有太大的用处
# if(POLICY CMP0042)
#   cmake_policy(SET CMP0042 NEW)  # CMake 3.0+ (2.8.12): MacOS "@rpath" in target's install name
# endif()

if(MSVC)
  #设置VC输入代码编码格式为utf-8
  set(CMAKE_C_FLAGS /source-charset:utf-8)
endif()


# 项目工程名
project (Three.cpp)
message(STATUS "BINARY dir " ${PROJECT_BINARY_DIR})
message(STATUS "SOURCE dir " ${PROJECT_SOURCE_DIR})

include(FetchContent)

# In this example we are picking a specific tag.
# You can also pick a specific commit, if you need to.
FetchContent_Declare(GSL
    GIT_REPOSITORY "https://github.com/microsoft/GSL"
    GIT_TAG "v3.1.0"
)
FetchContent_MakeAvailable(GSL)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# 定义工程根目录; CMAKE_SOURCE_DIR为内建变量，表示工程根目录的CMakeLists.txt文件路径
SET(ROOT_DIR ${CMAKE_SOURCE_DIR})

# 头文件，不管是项目自有，还是第三方库所有，都需要在include_directories和target_include_directories中指定才生效
# INCLUDE_DIRECTORIES(${ROOT_DIR}/src/imgui)
# INCLUDE_DIRECTORIES(${ROOT_DIR}/src/imgui/backends)
# INCLUDE_DIRECTORIES(${ROOT_DIR}/src/002)
# INCLUDE_DIRECTORIES(${ROOT_DIR}/src/002/base)


# 指定头文件搜索路径
INCLUDE_DIRECTORIES(${ROOT_DIR}/include)

# 指定引用的外部库的搜索路径
# message(STATUS "Configuring on/for ${CMAKE_SYSTEM_NAME}")
message(STATUS "CMAKE_HOST_SYSTEM_PROCESSOR matches ${CMAKE_HOST_SYSTEM_PROCESSOR}")
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  message(STATUS "Configuring on/for Linux")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  message(STATUS "Configuring on lib for macOS")
  LINK_DIRECTORIES(${ROOT_DIR}/lib/macOS)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  message(STATUS "Configuring on/for Windows")
  LINK_DIRECTORIES(${ROOT_DIR}/lib/win64)
else()
  message(STATUS "Configuring on/for ${CMAKE_SYSTEM_NAME}")
endif()



# 这些方法都不起作用，他们的作用前提是针对系统内置库，如果使用独立的angle则不行
# find_package(OpenGL REQUIRED COMPONENTS OpenGL EGL)
# Search for EGL; nvidia drivers ship the library but not headers, so we redistribute those
# find_path(EGL_INCLUDE_DIR NAMES EGL/egl.h PATHS ${ROOT_DIR}/include REQUIRED)
# find_library(EGL_LIBRARIES NAMES egl EGL REQUIRED)

# from dan ginsburg的opengl es3，but it doesnt work too
# find_library( OPENGLES3_LIBRARY GLESv2 "OpenGL ES v3.0 library")
# find_library( EGL_LIBRARY EGL "EGL 1.4 library" )

# find_library(libGLESv2 GLESv2)
# find_library(libEGL EGL)

# 指定可执行文件存放目录
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

file(GLOB SHADERS ${ROOT_DIR}/src/002/frustum/shaders/*.fsh;${ROOT_DIR}/src/002/frustum/shaders/*.vsh;${ROOT_DIR}/src/002/frustum/shaders/*.glsl)
file(GLOB MODEL_FILES ${ROOT_DIR}/src/002/frustum/*.obj)
# file(GLOB THREE_HEADER ${ROOT_DIR}/src/imgui/*.h;${ROOT_DIR}/src/imgui/backends/*.h)
# file(GLOB THREE_SOURCE ${ROOT_DIR}/src/imgui/backends/*.cpp;${ROOT_DIR}/src/imgui/*.cpp;${ROOT_DIR}/src/main.cpp;${ROOT_DIR}/src/002/frustum/*.cpp)
file(GLOB IMGUI_SOURCE ${ROOT_DIR}/src/imgui/backends/*.h;${ROOT_DIR}/src/imgui/backends/*.cpp;${ROOT_DIR}/src/imgui/*.h;${ROOT_DIR}/src/imgui/*.cpp)

file(GLOB Camera_Frustum ${ROOT_DIR}/src/002/frustum/base/perfMonitor/*.h ${ROOT_DIR}/src/002/frustum/base/perfMonitor/*.cpp;
${ROOT_DIR}/src/002/frustum/base/quat/*.h;${ROOT_DIR}/src/002/frustum/base/quat/*.cpp;
${ROOT_DIR}/src/002/frustum/base/tinyobj/*.h;${ROOT_DIR}/src/002/frustum/base/tinyobj/*.cpp;
${ROOT_DIR}/src/002/frustum/base/*.h;${ROOT_DIR}/src/002/frustum/base/*.cpp;
${ROOT_DIR}/src/002/frustum/*.h;${ROOT_DIR}/src/002/frustum/*.cpp)

file(GLOB THREECPP_SOURCE ${ROOT_DIR}/src/math/*.h;${ROOT_DIR}/src/math/*.cpp;
${ROOT_DIR}/src/core/*.h;${ROOT_DIR}/src/core/*.cpp)

# file(GLOB Camera_Frustum ${ROOT_DIR}/src/002/frustum/base/perfMonitor/*.cpp;
# ${ROOT_DIR}/src/002/frustum/base/quat/*.cpp;
# ${ROOT_DIR}/src/002/frustum/base/tinyobj/*.cpp;
# ${ROOT_DIR}/src/002/frustum/base/*.cpp;
# ${ROOT_DIR}/src/002/frustum/*.cpp)

# just for visual studio
# source_group("Include" ${THREE_HEADER})

# 构建可执行程序
ADD_EXECUTABLE(threecpp ${ROOT_DIR}/src/002/main_002.cpp)
# 如果headers作为源码的一部分，那么当然也是作为source来管理的
target_sources(threecpp PRIVATE  ${IMGUI_SOURCE} ${Camera_Frustum})
target_include_directories(threecpp PRIVATE 
${ROOT_DIR}/src/imgui
${ROOT_DIR}/src/imgui/backends
#${ROOT_DIR}/src/math
#${ROOT_DIR}/src/core
${ROOT_DIR}/src/002/frustum 
${ROOT_DIR}/src/002/frustum/base 
${ROOT_DIR}/src/002/frustum/base/quat 
${ROOT_DIR}/src/002/frustum/base/tinyobj
${ROOT_DIR}/src/002/frustum/base/perfMonitor)
# target_include_directories(threecpp PUBLIC 
# ${ROOT_DIR}/src/imgui/ PUBLIC
# ${ROOT_DIR}/src/imgui/backends/ PUBLIC
# )
# TARGET_LINK_LIBRARIES(threecpp PUBLIC
#   # "-framework Cocoa"   # not necessary for system library
#   # "-framework CoreGraphics"
#   glfw.3.3
#   GLESv2
#   EGL
# )
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  TARGET_LINK_LIBRARIES(threecpp PUBLIC
  # "-framework Cocoa"   # not necessary for system library
  # "-framework CoreGraphics"
  glfw.3.3
  GLESv2
  EGL)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  TARGET_LINK_LIBRARIES(threecpp PUBLIC
  glfw3
  libGLESv2.dll.lib
  libEGL.dll.lib)
else()
  message(STATUS "TARGET_LINK_LIBRARIES Configuring on/for ${CMAKE_SYSTEM_NAME}")
endif()

file(COPY ${SHADERS} ${MODEL_FILES}
     DESTINATION ${PROJECT_BINARY_DIR}/bin)

# TARGET_LINK_LIBRARIES(threecpp GLESv2;EGL)

add_subdirectory(src)

add_subdirectory(src/examples/simple_triangle)
