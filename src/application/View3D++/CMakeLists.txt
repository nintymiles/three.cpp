
# 项目工程名
project (View3D++)

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

file(GLOB THREECPP_SOURCE ${ROOT_DIR}/src/*.h;${ROOT_DIR}/src/*.cpp;)
file(GLOB IMGUI_SOURCE ${ROOT_DIR}/src/imgui/backends/*.h;${ROOT_DIR}/src/imgui/backends/*.cpp;${ROOT_DIR}/src/imgui/*.h;${ROOT_DIR}/src/imgui/*.cpp)
file(GLOB IMGUI_FILEDIALOG_SOURCE ${ROOT_DIR}/src/imgui/ImGuiFileDialog/*.h;${ROOT_DIR}/src/imgui/ImGuiFileDialog/*.cpp;${ROOT_DIR}/src/ImGuiFileDialog/dirent/*.h;)

# 构建可执行程序
ADD_EXECUTABLE(View3D++ ${ROOT_DIR}/src/application/View3D++/view3d_main.cpp)

# 如果headers作为源码的一部分，那么当然也是作为source来管理的
target_sources(View3D++ PRIVATE  ${IMGUI_SOURCE} ${IMGUI_FILEDIALOG_SOURCE} ${THREECPP_SOURCE})
target_compile_definitions(View3D++ PRIVATE IMGUI_IMPL_OPENGL_ES3)
target_include_directories(View3D++ PRIVATE
${ROOT_DIR}/src
${ROOT_DIR}/src/imgui
${ROOT_DIR}/src/imgui/backends
${ROOT_DIR}/src/imgui/ImGuiFileDialog
${ROOT_DIR}/src/imgui/ImGuiFileDialog/dirent
${ROOT_DIR}/src/utils
)

#MSVC编译时需要添加一些额外的参数，否则会导致一些奇怪的错误
if(MSVC)
  target_link_options(View3D++ PUBLIC /DEBUG)
  target_link_options(View3D++ PUBLIC /NODEFAULTLIB:library)
  # 和上一个选项应该是等效的
  set_property(TARGET View3D++ APPEND PROPERTY LINK_FLAGS "/NODEFAULTLIB:MSVCRT")

endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  TARGET_LINK_LIBRARIES(View3D++ PUBLIC
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
  TARGET_LINK_LIBRARIES(View3D++ PUBLIC
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
