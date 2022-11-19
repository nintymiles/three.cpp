# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.12)


# 项目工程名
project (teapot_desktop)



# just for visual studio
# source_group("Include" ${THREE_HEADER})

file(GLOB TOP_SOURCE ${ROOT_DIR}/src/common_utils.cpp)
file(GLOB TOP_HEADER ${ROOT_DIR}/src/*.h)

file(GLOB_RECURSE CORE_SOURCE ${ROOT_DIR}/src/core/*.cpp)
# file(GLOB_RECURSE CORE_INLINE ./detail/*.inl)
file(GLOB_RECURSE CORE_HEADER ${ROOT_DIR}/src/core/*.h)

file(GLOB MATH_SOURCE ${ROOT_DIR}/src/math/*.cpp)
file(GLOB MATH_HEADER ${ROOT_DIR}/src/math/*.h)

file(GLOB CAMERAS_SOURCE ${ROOT_DIR}/src/cameras/*.cpp)
file(GLOB CAMERAS_HEADER ${ROOT_DIR}/src/cameras/*.h)

file(GLOB TEAPOT_SOURCE ${ROOT_DIR}/src/examples/teapot/teapot.cpp)
file(GLOB TEAPOT_HEADER ${ROOT_DIR}/src/examples/teapot/*.h)

source_group("Core" FILES ${CORE_SOURCE})
source_group("Core" FILES ${CORE_HEADER})

source_group("Cameras" FILES ${CAMERAS_SOURCE})
source_group("Cameras" FILES ${CAMERAS_HEADER})

file(GLOB THREECPP_SOURCE ${TOP_HEADER};${TOP_SOURCE};${MATH_HEADER};${MATH_SOURCE};
        ${CAMERAS_HEADER};${CAMERAS_SOURCE};
        ${CORE_SOURCE};${CORE_HEADER};
        ${TEAPOT_SOURCE};${TEAPOT_HEADER})

# 构建可执行程序
ADD_EXECUTABLE(teapot_desktop ${ROOT_DIR}/src/examples/teapot_desktop/teapot_desktop.cpp)
# 如果headers作为源码的一部分，那么当然也是作为source来管理的
target_sources(teapot_desktop PRIVATE ${IMGUI_SOURCE} ${THREECPP_SOURCE})

target_include_directories(teapot_desktop PRIVATE
${ROOT_DIR}/src
${ROOT_DIR}/src/math
${ROOT_DIR}/src/core
${ROOT_DIR}/src/imgui
${ROOT_DIR}/src/imgui/backends
${ROOT_DIR}/src/examples/teapot)

TARGET_LINK_LIBRARIES(teapot_desktop PUBLIC
  glfw.3.3
  GLESv2
  EGL
  GSL
)