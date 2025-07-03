cmake_minimum_required(VERSION 3.1)
project(glm)

add_library(glm INTERFACE)


target_include_directories(glm INTERFACE include "${PROJECT_SOURCE_DIR}/")