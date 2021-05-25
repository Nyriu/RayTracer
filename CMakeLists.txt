cmake_minimum_required(VERSION 3.2)
project(Simple_SphereTracer LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)

find_package(glfw3 REQUIRED)
set(OpenGL_GL_PREFERENCE "LEGACY")
find_package(OpenGL REQUIRED)

#set(CMAKE_CXX_FLAGS "-g")
add_compile_options(-g) # -Wall) # -Wextra -pedantic) # -Werror)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../")

add_executable(main ${SOURCES})
target_link_libraries(main glfw GL)
