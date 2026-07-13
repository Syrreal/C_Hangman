cmake_minimum_required(VERSION 3.10.0)
project(C_Hangman VERSION 0.0.0 LANGUAGES C CXX)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(C_Hangman src/main.cpp dependencies/GLAD/src/glad.c)

target_include_directories(C_Hangman PRIVATE dependencies)

target_link_libraries(C_Hangman glfw OpenGL::GL)