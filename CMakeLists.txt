cmake_minimum_required(VERSION 3.0)

project(Simple_SphereTracer)

set(CMAKE_CXX_FLAGS "-g")

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../")

add_executable(main ${SOURCES})

#target_link_libraries(main
#  -lglfw
#  #-lvulkan
#  #-ldl
#  #-lpthread
#  #-lX11
#  #-lXxf86vm
#  #-lXrandr
#  #-lXi
#  )
