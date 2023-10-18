
# 项目工程名
project (Three_cpp_lib)

if(MSVC)
  #设置VC输入代码编码格式为utf-8
  set(CMAKE_C_FLAGS /source-charset:utf-8)
endif()

find_package(Microsoft.GSL CONFIG)

file(GLOB TOP_SOURCE ${ROOT_DIR}/src/common_utils.cpp)
file(GLOB TOP_HEADER ${ROOT_DIR}/src/*.h)

file(GLOB UTIL_SOURCE ${ROOT_DIR}/src/utils/*.cpp)
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

file(GLOB RENDERS_SOURCE ${ROOT_DIR}/src/renders/*.cpp;${ROOT_DIR}/src/renders/opengl/*.cpp;${ROOT_DIR}/src/renders/shaders/*.cpp)
file(GLOB RENDERS_HEADER ${ROOT_DIR}/src/renders/*.h;${ROOT_DIR}/src/renders/opengl/*.h;${ROOT_DIR}/src/renders/shaders/*.h;${ROOT_DIR}/src/renders/shaders/shader_chunk/*.h)

source_group("Core" FILES ${CORE_SOURCE})
source_group("Core" FILES ${CORE_HEADER})

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

source_group("Textures" FILES ${TEXTURES_SOURCE})
source_group("Textures" FILES ${TEXTURES_HEADER})

source_group("Scenes" FILES ${SCENES_SOURCE})
source_group("Scenes" FILES ${SCENES_HEADER})

source_group("Materials" FILES ${TEXTURES_SOURCE})
source_group("Materials" FILES ${TEXTURES_HEADER})

file(GLOB THREECPP_SOURCE ${TOP_HEADER};${TOP_SOURCE};${MATH_HEADER};${MATH_SOURCE};
        ${CAMERAS_HEADER};${CAMERAS_SOURCE};
        ${CORE_SOURCE};${CORE_HEADER};
        ${UTIL_SOURCE};${UTIL_HEADER};
        ${LIGHTS_SOURCE};${LIGHTS_HEADER};
        ${OBJECTS_SOURCE};${OBJECTS_HEADER};
        ${TEXTURES_SOURCE};${TEXTURES_HEADER};
        ${SCENES_SOURCE};${SCENES_HEADER};
        ${GEOMETRIES_SOURCE};${GEOMETRIES_HEADER};
        ${MATERIALS_SOURCE};${MATERIALS_SOURCE};
        ${RENDERS_SOURCE};${RENDERS_HEADER})

# 构建可执行程序
ADD_EXECUTABLE(threelibcpp ${ROOT_DIR}/src/main.cpp)
# 如果headers作为源码的一部分，那么当然也是作为source来管理的
target_sources(threelibcpp PRIVATE  ${IMGUI_SOURCE} ${THREECPP_SOURCE})
target_include_directories(threelibcpp PRIVATE 
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
${ROOT_DIR}/src/renders/
${ROOT_DIR}/src/renders/opengl
${ROOT_DIR}/src/renders/shaders
${ROOT_DIR}/src/utils)

#MSVC编译时需要添加一些额外的参数，否则会导致一些奇怪的错误
if(MSVC)
  target_link_options(threelibcpp PUBLIC /DEBUG)
  # 和上一个选项应该是等效的
  # set_property(TARGET threelibcpp APPEND PROPERTY LINK_FLAGS "/NODEFAULTLIB:MSVCRT")
endif()

# TARGET_LINK_LIBRARIES(threelibcpp PUBLIC
#   glfw.3.3
#   GLESv2
#   EGL
#   PRIVATE GSL
# )
# 如果以FetchContent方式引用GSL，则需要添加'PRIVATE GSL'到traget_link_libraries
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  TARGET_LINK_LIBRARIES(threelibcpp PUBLIC
  # "-framework Cocoa"   # not necessary for system library
  # "-framework CoreGraphics"
  glfw.3.3
  GLESv2
  EGL
  PRIVATE GSL)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows") # Windows则需要添加'GSL'到traget_link_libraries
  TARGET_LINK_LIBRARIES(threelibcpp PUBLIC
  glfw3
  libGLESv2.dll.lib
  libEGL.dll.lib
  GSL)

else()
  message(STATUS "TARGET_LINK_LIBRARIES Configuring on/for ${CMAKE_SYSTEM_NAME}")
endif()

#enable_testing()
#
#find_package(Microsoft.GSL CONFIG)
#
#add_executable(
#  threecpp_test
#  ${ROOT_DIR}/src/test/frustum_test.cpp;
#  #${ROOT_DIR}/src/test/color_test.cpp;
#  #${ROOT_DIR}/src/test/vector3_test.cpp;
#  #${ROOT_DIR}/src/test/quaternion_test.cpp
#        renders/opengl/gl_info.h utils/string_utils.h utils/string_utils.cpp)
#target_link_libraries(
#  threecpp_test
#  GSL
#  GTest::gtest_main
#)
#target_sources(threecpp_test PRIVATE ${THREECPP_SOURCE} ${ROOT_DIR}/src/test/constant_test.h)
#target_include_directories(threecpp_test PRIVATE
#${ROOT_DIR}/src
#${ROOT_DIR}/src/imgui
#${ROOT_DIR}/src/imgui/backends
#${ROOT_DIR}/src/math
#${ROOT_DIR}/src/core
#${ROOT_DIR}/src/test)
#
#include(GoogleTest)
#gtest_discover_tests(threecpp_test)