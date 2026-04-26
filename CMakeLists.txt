cmake_minimum_required(VERSION 3.10)
project(pendulum_opengl)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(libs/glfw/include)
include_directories(libs/glad/include)
include_directories(imgui)
include_directories(imgui/backends)


link_directories(libs/glfw/lib-mingw-w64)

add_executable(pendulum_opengl
    src/main.cpp
    src/Shader.cpp
    
    libs/glad/src/glad.c

    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)

target_link_libraries(pendulum_opengl
    glfw3
    opengl32
)
