cmake_minimum_required(VERSION 3.12)

project(ImGuiExample)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(GSL
        GIT_REPOSITORY "https://github.com/microsoft/GSL"
        GIT_TAG "v3.1.0"
        )
FetchContent_MakeAvailable(GSL)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../../../include)

SET(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../..)

file(GLOB TOP_SOURCE ${ROOT_DIR}/src/common_utils.cpp)
file(GLOB TOP_HEADER ${ROOT_DIR}/src/*.h)

file(GLOB_RECURSE CORE_SOURCE ${ROOT_DIR}/src/core/*.cpp)
# file(GLOB_RECURSE CORE_INLINE ./detail/*.inl)
file(GLOB_RECURSE CORE_HEADER ${ROOT_DIR}/src/core/*.h)

file(GLOB MATH_SOURCE ${ROOT_DIR}/src/math/*.cpp)
file(GLOB MATH_HEADER ${ROOT_DIR}/src/math/*.h)

file(GLOB CAMERAS_SOURCE ${ROOT_DIR}/src/cameras/*.cpp)
file(GLOB CAMERAS_HEADER ${ROOT_DIR}/src/cameras/*.h)

source_group("Core" FILES ${CORE_SOURCE})
source_group("Core" FILES ${CORE_HEADER})

source_group("Cameras" FILES ${CAMERAS_SOURCE})
source_group("Cameras" FILES ${CAMERAS_HEADER})

file(GLOB THREECPP_SOURCE ${TOP_HEADER};${TOP_SOURCE};${MATH_HEADER};${MATH_SOURCE};
        ${CAMERAS_HEADER};${CAMERAS_SOURCE};
        ${CORE_SOURCE};${CORE_HEADER})

add_library(${CMAKE_PROJECT_NAME} SHARED
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/teapot.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/teapot.inl
  ${CMAKE_CURRENT_SOURCE_DIR}/../../imgui/imgui.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../../imgui/imgui_demo.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../../imgui/imgui_draw.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../../imgui/imgui_tables.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../../imgui/imgui_widgets.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../../math/matrix4.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../../math/vector3.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../../imgui/backends/android/imgui_impl_android.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../../imgui/backends/imgui_impl_opengl3.cpp
  ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c
)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${THREECPP_SOURCE})

set(CMAKE_SHARED_LINKER_FLAGS
  "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate"
)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
  IMGUI_IMPL_OPENGL_ES3
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../..
  ${CMAKE_CURRENT_SOURCE_DIR}/../../imgui
  ${CMAKE_CURRENT_SOURCE_DIR}/../../imgui/backends
  ${CMAKE_CURRENT_SOURCE_DIR}/../../imgui/backends/android
  ${CMAKE_CURRENT_SOURCE_DIR}/../../math
  ${CMAKE_CURRENT_SOURCE_DIR}/../../core
  ${ANDROID_NDK}/sources/android/native_app_glue
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
  android
  EGL
  GLESv3
  log
  GSL
)
