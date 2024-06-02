
# 项目工程名
project (three_cpp_gl)

if(MSVC)
  #设置VC输入代码编码格式为utf-8
  set(CMAKE_C_FLAGS /source-charset:utf-8)
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

file(GLOB BASE_SOURCE ${ROOT_DIR}/src/examples/base/*.cpp)
file(GLOB BASE_HEADER ${ROOT_DIR}/src/examples/base/*.h)

file(GLOB THREEJS_EXAMPLES_SOURCE ${ROOT_DIR}/src/examples/threejs/geometries/*.cpp;
        ${ROOT_DIR}/src/examples/threejs/postprocessing/*.cpp;
        ${ROOT_DIR}/src/examples/threejs/shaders/*.cpp;
        ${ROOT_DIR}/src/examples/threejs/materials/*.cpp;
        ${ROOT_DIR}/src/examples/threejs/lights/*.cpp;
        ${ROOT_DIR}/src/examples/threejs/lines/*.cpp;
        ${ROOT_DIR}/src/examples/threejs/features/*.cpp)
file(GLOB THREEJS_EXAMPLES_HEADER ${ROOT_DIR}/src/examples/threejs/geometries/*.h;
        ${ROOT_DIR}/src/examples/threejs/postprocessing/*.h;
        ${ROOT_DIR}/src/examples/threejs/shaders/*.h;
        ${ROOT_DIR}/src/examples/threejs/materials/*.h;
        ${ROOT_DIR}/src/examples/threejs/lights/*.h;
        ${ROOT_DIR}/src/examples/threejs/lines/*.h;
        ${ROOT_DIR}/src/examples/threejs/features/*.h)



file(GLOB THREEJS_EXAMPLES ${BASE_SOURCE};${BASE_HEADER};
        ${THREEJS_EXAMPLES_SOURCE};${THREEJS_EXAMPLES_HEADER})

list(APPEND THREEJS_EXAMPLES_INCLUDES
        ${ROOT_DIR}/src/examples/base
        ${ROOT_DIR}/src/examples/threejs/lines/
        ${ROOT_DIR}/src/examples/threejs/lights/
        ${ROOT_DIR}/src/examples/threejs/materials/
        ${ROOT_DIR}/src/examples/threejs/shaders/
        ${ROOT_DIR}/src/examples/threejs/postprocessing/
        ${ROOT_DIR}/src/examples/threejs/geometries/
        ${ROOT_DIR}/src/examples/threejs/features/)

#add_link_options("-stdlib=libc++")
# 构建可执行程序
ADD_EXECUTABLE(threelibgl ${ROOT_DIR}/src/examples/app_gl/app_gl.cpp)
#add_library(threelibgl)

target_sources(threelibgl PRIVATE  ${IMGUI_SOURCE} ${THREECPP_LIB_SOURCE} ${THREEJS_EXAMPLES})
target_include_directories(threelibgl PRIVATE
      ${THREECPP_LIB_INCLUDES}
      ${IMGUI_INCLUDES}
      ${THREEJS_EXAMPLES_INCLUDES}
      )

#MSVC编译时需要添加一些额外的参数，否则会导致一些奇怪的错误
if(MSVC)
  target_link_options(threelibgl PUBLIC /DEBUG)
  target_link_options(threelibgl PUBLIC /NODEFAULTLIB:library)
  # 和上一个选项应该是等效的
  set_property(TARGET threelibgl APPEND PROPERTY LINK_FLAGS "/NODEFAULTLIB:MSVCRT")

endif()

# TARGET_LINK_LIBRARIES(threelibgl PUBLIC
#   glfw.3.3
#   GLESv2
#   EGL
#   PRIVATE GSL
# )
# 如果以FetchContent方式引用GSL，则需要添加'PRIVATE GSL'到traget_link_libraries
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  TARGET_LINK_LIBRARIES(threelibgl PUBLIC
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
  TARGET_LINK_LIBRARIES(threelibgl PUBLIC
  glfw3
  #glfw3d.lib for static debug build lib
  #angel相关dll要不拷贝到系统路径，要不拷贝到执行目录，lib用于编译引用，dll用于链接执行
#  libGLESv2.dll.lib
#  libEGL.dll.lib
  GLADd.lib
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