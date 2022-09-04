# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.12)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 这个策略貌似也没有太大的用处
# if(POLICY CMP0042)
#   cmake_policy(SET CMP0042 NEW)  # CMake 3.0+ (2.8.12): MacOS "@rpath" in target's install name
# endif()

# 项目工程名
project (Three.cpp.lib)
message(STATUS "BINARY dir " ${PROJECT_BINARY_DIR})
message(STATUS "SOURCE dir " ${PROJECT_SOURCE_DIR})

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
else()
  message(STATUS "Configuring on/for ${CMAKE_SYSTEM_NAME}")
endif()


# 指定可执行文件存放目录
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# file(GLOB SHADERS ${ROOT_DIR}/src/002/frustum/shaders/*.fsh;${ROOT_DIR}/src/002/frustum/shaders/*.vsh;${ROOT_DIR}/src/002/frustum/shaders/*.glsl)
# file(GLOB MODEL_FILES ${ROOT_DIR}/src/002/frustum/*.obj)
# file(GLOB THREE_HEADER ${ROOT_DIR}/src/imgui/*.h;${ROOT_DIR}/src/imgui/backends/*.h)
# file(GLOB THREE_SOURCE ${ROOT_DIR}/src/imgui/backends/*.cpp;${ROOT_DIR}/src/imgui/*.cpp;${ROOT_DIR}/src/main.cpp;${ROOT_DIR}/src/002/frustum/*.cpp)
file(GLOB IMGUI_SOURCE ${ROOT_DIR}/src/imgui/backends/*.h;${ROOT_DIR}/src/imgui/backends/*.cpp;${ROOT_DIR}/src/imgui/*.h;${ROOT_DIR}/src/imgui/*.cpp)

file(GLOB THREECPP_SOURCE ${ROOT_DIR}/src/*.h ${ROOT_DIR}/src/*.cpp;
${ROOT_DIR}/src/core/*.h;${ROOT_DIR}/src/core/*.cpp;
${ROOT_DIR}/src/math/*.h;${ROOT_DIR}/src/math/*.cpp)

# 构建可执行程序
ADD_EXECUTABLE(threelibcpp ${ROOT_DIR}/src/main.cpp)
# 如果headers作为源码的一部分，那么当然也是作为source来管理的
target_sources(threelibcpp PRIVATE  ${IMGUI_SOURCE} ${THREECPP_SOURCE})
target_include_directories(threelibcpp PRIVATE 
${ROOT_DIR}/src
${ROOT_DIR}/src/imgui
${ROOT_DIR}/src/imgui/backends
${ROOT_DIR}/src/math
${ROOT_DIR}/src/core)

TARGET_LINK_LIBRARIES(threelibcpp PUBLIC
  # "-framework Cocoa"   # not necessary for system library
  # "-framework CoreGraphics"
  glfw.3.3
  GLESv2
  EGL
)

# file(COPY ${SHADERS} ${MODEL_FILES}
#      DESTINATION ${PROJECT_BINARY_DIR}/bin)

# TARGET_LINK_LIBRARIES(threecpp GLESv2;EGL)