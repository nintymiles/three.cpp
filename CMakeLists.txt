# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

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

#include(FetchContent)
#
## In this example we are picking a specific tag.
## You can also pick a specific commit, if you need to.
#FetchContent_Declare(GSL
#    GIT_REPOSITORY "https://github.com/microsoft/GSL"
#    GIT_TAG "v3.1.0"
#)
#FetchContent_MakeAvailable(GSL)
#
#FetchContent_Declare(
#  googletest
#  GIT_REPOSITORY https://github.com/google/googletest.git
#  GIT_TAG release-1.12.1
#)
## For Windows: Prevent overriding the parent project's compiler/linker settings
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#FetchContent_MakeAvailable(googletest)

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


# ImGui Source files set，it can be used by sub project.
# file(GLOB THREE_HEADER ${ROOT_DIR}/src/imgui/*.h;${ROOT_DIR}/src/imgui/backends/*.h)
# file(GLOB THREE_SOURCE ${ROOT_DIR}/src/imgui/backends/*.cpp;${ROOT_DIR}/src/imgui/*.cpp;${ROOT_DIR}/src/main.cpp;${ROOT_DIR}/src/002/frustum/*.cpp)
file(GLOB IMGUI_SOURCE ${ROOT_DIR}/src/imgui/backends/*.h;${ROOT_DIR}/src/imgui/backends/*.cpp;${ROOT_DIR}/src/imgui/*.h;${ROOT_DIR}/src/imgui/*.cpp)

file(GLOB TOP_SOURCE ${ROOT_DIR}/src/*.cpp)
file(GLOB TOP_HEADER ${ROOT_DIR}/src/*.h)

file(GLOB UTIL_SOURCE ${ROOT_DIR}/src/utils/*.cpp;${ROOT_DIR}/src/utils/*.c)
file(GLOB UTIL_HEADER ${ROOT_DIR}/src/utils/*.h)

file(GLOB_RECURSE CORE_SOURCE ${ROOT_DIR}/src/core/*.cpp)
# file(GLOB_RECURSE CORE_INLINE ./detail/*.inl)
file(GLOB_RECURSE CORE_HEADER ${ROOT_DIR}/src/core/*.h)

file(GLOB MATH_SOURCE ${ROOT_DIR}/src/math/*.cpp;${ROOT_DIR}/src/math/interpolants/*.cpp)
file(GLOB MATH_HEADER ${ROOT_DIR}/src/math/*.h;${ROOT_DIR}/src/math/interpolants/*.h)

file(GLOB CAMERAS_SOURCE ${ROOT_DIR}/src/cameras/*.cpp)
file(GLOB CAMERAS_HEADER ${ROOT_DIR}/src/cameras/*.h)

file(GLOB LIGHTS_SOURCE ${ROOT_DIR}/src/lights/*.cpp)
file(GLOB LIGHTS_HEADER ${ROOT_DIR}/src/lights/*.h)

file(GLOB TEXTURES_SOURCE ${ROOT_DIR}/src/textures/*.cpp)
file(GLOB TEXTURES_HEADER ${ROOT_DIR}/src/textures/*.h)

file(GLOB MATERIALS_SOURCE ${ROOT_DIR}/src/materials/*.cpp)
file(GLOB MATERIALS_HEADER ${ROOT_DIR}/src/materials/*.h)

file(GLOB OBJECTS_SOURCE ${ROOT_DIR}/src/objects/*.cpp)
file(GLOB OBJECTS_HEADER ${ROOT_DIR}/src/objects/*.h)

file(GLOB SCENES_SOURCE ${ROOT_DIR}/src/scenes/*.cpp)
file(GLOB SCENES_HEADER ${ROOT_DIR}/src/scenes/*.h)

file(GLOB GEOMETRIES_SOURCE ${ROOT_DIR}/src/geometries/*.cpp;${ROOT_DIR}/src/geometries/extra/*.cpp)
file(GLOB GEOMETRIES_HEADER ${ROOT_DIR}/src/geometries/*.h;${ROOT_DIR}/src/geometries/extra/*.h)

file(GLOB CURVES_SOURCE ${ROOT_DIR}/src/curves/*.cpp)
file(GLOB CURVES_HEADER ${ROOT_DIR}/src/curves/*.h)

file(GLOB HELPERS_SOURCE ${ROOT_DIR}/src/helpers/*.cpp)
file(GLOB HELPERS_HEADER ${ROOT_DIR}/src/helpers/*.h)

file(GLOB LOADERS_SOURCE ${ROOT_DIR}/src/loaders/*.cpp)
file(GLOB LOADERS_HEADER ${ROOT_DIR}/src/loaders/*.h)

file(GLOB RENDERS_SOURCE ${ROOT_DIR}/src/renders/*.cpp;${ROOT_DIR}/src/renders/opengl/*.cpp;${ROOT_DIR}/src/renders/shaders/*.cpp)
file(GLOB RENDERS_HEADER ${ROOT_DIR}/src/renders/*.h;${ROOT_DIR}/src/renders/opengl/*.h;${ROOT_DIR}/src/renders/shaders/*.h;${ROOT_DIR}/src/renders/shaders/shader_chunk/*.h;${ROOT_DIR}/src/renders/shaders/shader_lib/*.h)

file(GLOB CONTROLS_SOURCE ${ROOT_DIR}/src/control/*.cpp)
file(GLOB CONTROLS_HEADER ${ROOT_DIR}/src/control/*.h)

file(GLOB EXTRA_SOURCE ${ROOT_DIR}/src/extra/shaders/*.cpp;${ROOT_DIR}/src/extra/postprocessing/*.cpp;${ROOT_DIR}/src/extra/math/*.cpp;${ROOT_DIR}/src/extra/*.cpp)
file(GLOB EXTRA_HEADER ${ROOT_DIR}/src/extra/shaders/*.h;${ROOT_DIR}/src/extra/postprocessing/*.h;${ROOT_DIR}/src/extra/math/*.h;${ROOT_DIR}/src/extra/*.h)

source_group("Core" FILES ${CORE_SOURCE})
source_group("Core" FILES ${CORE_HEADER})

source_group("Math" FILES ${MATH_SOURCE})
source_group("Math" FILES ${MATH_HEADER})

source_group("Cameras" FILES ${CAMERAS_SOURCE})
source_group("Cameras" FILES ${CAMERAS_HEADER})

source_group("Renders" FILES ${RENDERS_SOURCE})
source_group("Renders" FILES ${RENDERS_HEADER})

source_group("Utils" FILES ${UTIL_SOURCE})
source_group("Utils" FILES ${UTIL_HEADER})

source_group("Lights" FILES ${LIGHTS_SOURCE})
source_group("Lights" FILES ${LIGHTS_HEADER})

source_group("Objects" FILES ${OBJECTS_SOURCE})
source_group("Objects" FILES ${OBJECTS_HEADER})

source_group("Geometries" FILES ${GEOMETRIES_SOURCE})
source_group("Geometries" FILES ${GEOMETRIES_HEADER})

source_group("Curves" FILES ${CURVES_SOURCE})
source_group("Curves" FILES ${CURVES_HEADER})

source_group("Helpers" FILES ${HELPERS_SOURCE})
source_group("Helpers" FILES ${HELPERS_HEADER})

source_group("Extra" FILES ${EXTRA_SOURCE})
source_group("Extra" FILES ${EXTRA_HEADER})

source_group("Textures" FILES ${TEXTURES_SOURCE})
source_group("Textures" FILES ${TEXTURES_HEADER})

source_group("Loaders" FILES ${LOADERS_SOURCE})
source_group("Loaders" FILES ${LOADERS_HEADER})

source_group("Scenes" FILES ${SCENES_SOURCE})
source_group("Scenes" FILES ${SCENES_HEADER})

source_group("Materials" FILES ${MATERIALS_SOURCE})
source_group("Materials" FILES ${MATERIALS_HEADER})

source_group("Controls" FILES ${CONTROLS_SOURCE})
source_group("Controls" FILES ${CONTROLS_HEADER})

file(GLOB THREECPP_LIB_SOURCE ${TOP_HEADER};${TOP_SOURCE};${MATH_HEADER};${MATH_SOURCE};
        ${CAMERAS_HEADER};${CAMERAS_SOURCE};
        ${CORE_SOURCE};${CORE_HEADER};
        ${UTIL_SOURCE};${UTIL_HEADER};
        ${LIGHTS_SOURCE};${LIGHTS_HEADER};
        ${OBJECTS_SOURCE};${OBJECTS_HEADER};
        ${TEXTURES_SOURCE};${TEXTURES_HEADER};
        ${SCENES_SOURCE};${SCENES_HEADER};
        ${GEOMETRIES_SOURCE};${GEOMETRIES_HEADER};
        ${CURVES_SOURCE};${CURVES_HEADER};
        ${HELPERS_SOURCE};${HELPERS_HEADER};
        ${EXTRA_SOURCE};${EXTRA_HEADER};
        ${LOADERS_SOURCE};${LOADERS_HEADER};
        ${MATERIALS_SOURCE};${MATERIALS_HEADER};
        ${RENDERS_SOURCE};${RENDERS_HEADER};
        ${CONTROLS_SOURCE};${CONTROLS_HEADER})


# 文件或者目录名有空格时，使用引号"${PROJECT_SOURCE_DIR}/Math Functions"
list(APPEND THREECPP_LIB_INCLUDES ${ROOT_DIR}/src
        ${ROOT_DIR}/src/utils
        ${ROOT_DIR}/src/math
        ${ROOT_DIR}/src/math/interpolants
        ${ROOT_DIR}/src/core
        ${ROOT_DIR}/src/textures
        ${ROOT_DIR}/src/materials
        ${ROOT_DIR}/src/cameras
        ${ROOT_DIR}/src/lights
        ${ROOT_DIR}/src/objects
        ${ROOT_DIR}/src/scenes
        ${ROOT_DIR}/src/geometries
        ${ROOT_DIR}/src/geometries/extra
        ${ROOT_DIR}/src/curves
        ${ROOT_DIR}/src/helpers
        ${ROOT_DIR}/src/extra
        ${ROOT_DIR}/src/extra/shaders/
        ${ROOT_DIR}/src/extra/postprocessing/
        ${ROOT_DIR}/src/extra/math/
        ${ROOT_DIR}/src/renders
        ${ROOT_DIR}/src/renders/opengl
        ${ROOT_DIR}/src/renders/shaders
        ${ROOT_DIR}/src/control
        ${ROOT_DIR}/src/loaders)

list(APPEND IMGUI_INCLUDES ${ROOT_DIR}/src/imgui
        ${ROOT_DIR}/src/imgui/backends
        ${ROOT_DIR}/src/imgui/ImGuiFileDialog
        ${ROOT_DIR}/src/imgui/ImGuiFileDialog/dirent)

# 这些方法都不起作用，他们起作用的前提是针对系统内置库，如果使用独立的angle则不行
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
  # glfw.3.3
  # libglfw.3.3.dylib
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

#### add sub projects
add_subdirectory(src)
add_subdirectory(src/examples/app_gl)

# add_subdirectory(src/examples/simple_triangle)
# add_subdirectory(src/examples/teapot_desktop)
add_subdirectory(src/application/remove_quartine)
add_subdirectory(src/application/View3D++)

