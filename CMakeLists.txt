cmake_minimum_required(VERSION 3.16)

project(ppm_viewer VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

add_executable(ppm_viewer src/main.cc)

add_subdirectory(thirdparty/glfw-3.4)
add_subdirectory(thirdparty/glad)
add_subdirectory(thirdparty/imgui-docking)

target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC GLFW_INCLUDE_NONE=1) 

target_include_directories(ppm_viewer PUBLIC "${PROJECT_SOURCE_DIR}/include")

target_link_libraries(ppm_viewer PRIVATE glfw glad imgui)