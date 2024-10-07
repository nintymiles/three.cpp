cmake_minimum_required(VERSION 3.12)

project(ImGuiExample)

set(CMAKE_CXX_STANDARD 17)
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

file(GLOB TOP_SOURCE ${ROOT_DIR}/src/*.cpp)
file(GLOB TOP_HEADER ${ROOT_DIR}/src/*.h)

file(GLOB UTIL_SOURCE ${ROOT_DIR}/src/utils/*.cpp;${ROOT_DIR}/src/utils/*.c)
file(GLOB UTIL_HEADER ${ROOT_DIR}/src/utils/*.h)

file(GLOB_RECURSE CORE_SOURCE ${ROOT_DIR}/src/core/*.cpp)
# file(GLOB_RECURSE CORE_INLINE ./detail/*.inl)
file(GLOB_RECURSE CORE_HEADER ${ROOT_DIR}/src/core/*.h)

file(GLOB MATH_SOURCE ${ROOT_DIR}/src/math/*.cpp)
file(GLOB MATH_HEADER ${ROOT_DIR}/src/math/*.h)

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

file(GLOB GEOMETRIES_SOURCE ${ROOT_DIR}/src/geometries/*.cpp)
file(GLOB GEOMETRIES_HEADER ${ROOT_DIR}/src/geometries/*.h)

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

file(GLOB EXTRA_SOURCE ${ROOT_DIR}/src/extra/shaders/*.cpp;${ROOT_DIR}/src/extra/postprocessing/*.cpp;${ROOT_DIR}/src/extra/*.cpp)
file(GLOB EXTRA_HEADER ${ROOT_DIR}/src/extra/shaders/*.h;${ROOT_DIR}/src/extra/postprocessing/*.h;${ROOT_DIR}/src/extra/*.h)

file(GLOB BASE_SOURCE ${ROOT_DIR}/src/examples/base/*.cpp)
file(GLOB BASE_HEADER ${ROOT_DIR}/src/examples/base/*.h)

file(GLOB THREEJS_EXAMPLES_SOURCE #${ROOT_DIR}/src/examples/threejs/geometries/*.cpp;
        ${ROOT_DIR}/src/examples/threejs/postprocessing/*.cpp;
        ${ROOT_DIR}/src/examples/threejs/shaders/*.cpp;
        ${ROOT_DIR}/src/examples/threejs/materials/*.cpp;
        ${ROOT_DIR}/src/examples/threejs/lights/*.cpp;
        ${ROOT_DIR}/src/examples/threejs/lines/*.cpp;
        ${ROOT_DIR}/src/examples/loaders/*.cpp)
file(GLOB THREEJS_EXAMPLES_HEADER #${ROOT_DIR}/src/examples/threejs/geometries/*.h;
        ${ROOT_DIR}/src/examples/threejs/postprocessing/*.h;
        ${ROOT_DIR}/src/examples/threejs/shaders/*.h;
        ${ROOT_DIR}/src/examples/threejs/materials/*.h;
        ${ROOT_DIR}/src/examples/threejs/lights/*.h;
        ${ROOT_DIR}/src/examples/threejs/lines/*.h;
        ${ROOT_DIR}/src/examples/loaders/*.h)

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

file(GLOB THREECPP_SOURCE ${TOP_HEADER};${TOP_SOURCE};${MATH_HEADER};${MATH_SOURCE};
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
        ${CONTROLS_SOURCE};${CONTROLS_HEADER};
        ${BASE_SOURCE};${BASE_HEADER};
        ${THREEJS_EXAMPLES_SOURCE};${THREEJS_EXAMPLES_HEADER})

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
        ${ROOT_DIR}/src
        ${ROOT_DIR}/src/imgui
        ${ROOT_DIR}/src/imgui/backends
        ${ROOT_DIR}/src/math
        ${ROOT_DIR}/src/core
        ${ROOT_DIR}/src/textures
        ${ROOT_DIR}/src/materials
        ${ROOT_DIR}/src/cameras
        ${ROOT_DIR}/src/lights
        ${ROOT_DIR}/src/objects
        ${ROOT_DIR}/src/scenes
        ${ROOT_DIR}/src/geometries
        ${ROOT_DIR}/src/curves
        ${ROOT_DIR}/src/helpers
        ${ROOT_DIR}/src/extra
        ${ROOT_DIR}/src/extra/shaders/
        ${ROOT_DIR}/src/extra/postprocessing/
        ${ROOT_DIR}/src/renders
        ${ROOT_DIR}/src/renders/opengl
        ${ROOT_DIR}/src/renders/shaders
        ${ROOT_DIR}/src/control
        ${ROOT_DIR}/src/utils
        ${ROOT_DIR}/src/loaders
        ${ROOT_DIR}/src/examples/base
        ${ROOT_DIR}/src/examples/threejs/lines/
        ${ROOT_DIR}/src/examples/threejs/lights/
        ${ROOT_DIR}/src/examples/threejs/materials/
        ${ROOT_DIR}/src/examples/threejs/shaders/
        #${ROOT_DIR}/src/examples/threejs/postprocessing/
  ${ANDROID_NDK}/sources/android/native_app_glue
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
  android
  EGL
  GLESv3
  log
  GSL
)
