# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.12)


# 项目工程名
project (simple_triangle)



# just for visual studio
# source_group("Include" ${THREE_HEADER})

# 构建可执行程序
ADD_EXECUTABLE(simple_triangle ${ROOT_DIR}/src/examples/simple_triangle/simple_triangle.cpp)
# 如果headers作为源码的一部分，那么当然也是作为source来管理的
target_sources(simple_triangle PRIVATE ${IMGUI_SOURCE})

target_include_directories(simple_triangle PRIVATE 
${ROOT_DIR}/src/imgui
${ROOT_DIR}/src/imgui/backends)

TARGET_LINK_LIBRARIES(simple_triangle PUBLIC
  glfw.3.3
  GLESv2
  EGL
)