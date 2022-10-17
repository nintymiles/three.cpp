
# 项目工程名
project (Three_cpp_lib)

if(MSVC)
  #设置VC输入代码编码格式为utf-8
  set(CMAKE_C_FLAGS /source-charset:utf-8)
endif()

find_package(Microsoft.GSL CONFIG)

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
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  TARGET_LINK_LIBRARIES(threelibcpp PUBLIC
  # "-framework Cocoa"   # not necessary for system library
  # "-framework CoreGraphics"
  glfw.3.3
  GLESv2
  EGL
  PRIVATE GSL)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  TARGET_LINK_LIBRARIES(threelibcpp PUBLIC
  glfw3
  libGLESv2.dll.lib
  libEGL.dll.lib
  GSL)

else()
  message(STATUS "TARGET_LINK_LIBRARIES Configuring on/for ${CMAKE_SYSTEM_NAME}")
endif()

enable_testing()

find_package(Microsoft.GSL CONFIG)

add_executable(
  threecpp_test
  ${ROOT_DIR}/src/test/frustum_test.cpp;
  #${ROOT_DIR}/src/test/color_test.cpp;
  #${ROOT_DIR}/src/test/vector3_test.cpp;
  #${ROOT_DIR}/src/test/quaternion_test.cpp
)
target_link_libraries(
  threecpp_test
  GSL
  GTest::gtest_main
)
target_sources(threecpp_test PRIVATE ${THREECPP_SOURCE} ${ROOT_DIR}/src/test/constant_test.h)
target_include_directories(threecpp_test PRIVATE 
${ROOT_DIR}/src
${ROOT_DIR}/src/imgui
${ROOT_DIR}/src/imgui/backends
${ROOT_DIR}/src/math
${ROOT_DIR}/src/core
${ROOT_DIR}/src/test)

include(GoogleTest)
gtest_discover_tests(threecpp_test)