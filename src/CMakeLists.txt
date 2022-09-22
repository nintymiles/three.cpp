
# 项目工程名
project (Three_cpp_lib)

file(GLOB THREECPP_SOURCE ${ROOT_DIR}/src/math/clock.h;${ROOT_DIR}/src/math/clock.cpp;
${ROOT_DIR}/src/math/vector3.h;${ROOT_DIR}/src/math/vector3.cpp;
${ROOT_DIR}/src/math/matrix4.h;${ROOT_DIR}/src/math/matrix4.cpp;
${ROOT_DIR}/src/math/quaternion.h;${ROOT_DIR}/src/math/quaternion.cpp)

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
  glfw.3.3
  GLESv2
  EGL
  PRIVATE GSL
)

enable_testing()

add_executable(
  threecpp_test
  ${ROOT_DIR}/src/test/vector3_test.cpp
)
target_link_libraries(
  threecpp_test
  GTest::gtest_main
)
target_include_directories(threecpp_test PRIVATE 
${ROOT_DIR}/src
${ROOT_DIR}/src/imgui
${ROOT_DIR}/src/imgui/backends
${ROOT_DIR}/src/math
${ROOT_DIR}/src/core
${ROOT_DIR}/src/test)

include(GoogleTest)
gtest_discover_tests(threecpp_test)