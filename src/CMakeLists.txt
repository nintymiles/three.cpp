
# 项目工程名
project (three_cpp_gles)

if(MSVC)
  #设置VC输入代码编码格式为utf-8
  #set(CMAKE_C_FLAGS /source-charset:utf-8)
  add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
  add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
  #增加macro定义NOMINMAX，去除windows.h中min/max宏影响
  add_definitions(-DNOMINMAX)

  #set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
  #set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
  #set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  #message("debugflag: ${CMAKE_CXX_FLAGS_DEBUG}")
  #string(REPLACE "/MDd" "/MTd" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
  message("debugflag: ${CMAKE_CXX_FLAGS_DEBUG}")
endif()

add_definitions(-D_DEBUG)
#add_definitions(-latomic;-lc++)
#add_definitions(-stdlib=libc++;-D_LIBCPP_DEBUG=1)
#add_definitions(-stdlib=libc++)

#find_package(Microsoft.GSL CONFIG)

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

file(GLOB THREEJS_EXAMPLES_SOURCE ${ROOT_DIR}/src/examples/threejs/geometries/*.cpp;
                                  ${ROOT_DIR}/src/examples/threejs/postprocessing/*.cpp;
                                  ${ROOT_DIR}/src/examples/threejs/shaders/*.cpp;
                                  ${ROOT_DIR}/src/examples/threejs/materials/*.cpp;
                                  ${ROOT_DIR}/src/examples/threejs/lights/*.cpp;
                                  ${ROOT_DIR}/src/examples/threejs/lines/*.cpp;
                                  ${ROOT_DIR}/src/examples/loaders/*.cpp)
file(GLOB THREEJS_EXAMPLES_HEADER ${ROOT_DIR}/src/examples/threejs/geometries/*.h;
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

#add_link_options("-stdlib=libc++")
# 构建可执行程序
ADD_EXECUTABLE(threecppgles ${ROOT_DIR}/src/examples/app_es/app_es.cpp)
#add_library(threecppgles)
# 如果headers作为源码的一部分，那么当然也是作为source来管理的
target_sources(threecppgles PRIVATE  ${IMGUI_SOURCE} ${THREECPP_SOURCE})
target_compile_definitions(threecppgles PRIVATE IMGUI_IMPL_OPENGL_ES3)
target_include_directories(threecppgles PRIVATE
${ROOT_DIR}/src
${ROOT_DIR}/src/imgui
${ROOT_DIR}/src/imgui/backends
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
${ROOT_DIR}/src/examples/threejs/postprocessing/
${ROOT_DIR}/src/examples/threejs/geometries/
)

#MSVC编译时需要添加一些额外的参数，否则会导致一些奇怪的错误
if(MSVC)
  target_link_options(threecppgles PUBLIC /DEBUG)
  target_link_options(threecppgles PUBLIC /NODEFAULTLIB:library)
  # 和上一个选项应该是等效的
  set_property(TARGET threecppgles APPEND PROPERTY LINK_FLAGS "/NODEFAULTLIB:MSVCRT")

endif()

# TARGET_LINK_LIBRARIES(threecppgles PUBLIC
#   glfw.3.3
#   GLESv2
#   EGL
#   PRIVATE GSL
# )
# 如果以FetchContent方式引用GSL，则需要添加'PRIVATE GSL'到traget_link_libraries
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  TARGET_LINK_LIBRARIES(threecppgles PUBLIC
  # "-framework Cocoa"   # not necessary for system library
  # "-framework CoreGraphics"
  glfw.3.3
  GLESv2
  EGL
  #pcrecpp
  #pcre
#  PRIVATE GSL
  )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows") # Windows则需要添加'GSL'到traget_link_libraries
  LINK_DIRECTORIES(${ROOT_DIR}/lib/win64) #添加动态连接库的路径
  TARGET_LINK_LIBRARIES(threecppgles PUBLIC
  glfw3
  #glfw3d.lib for static debug build lib
  #angel相关dll要不拷贝到系统路径，要不拷贝到执行目录，lib用于编译引用，dll用于链接执行
  libGLESv2.dll.lib
  libEGL.dll.lib
  #GLADd.lib
  #pcrecppd.lib
  #pcred.lib
  #GSL
  )

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