cmake_minimum_required(VERSION 3.15)
project(stardust_demo)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} DemoApplication.cpp)

target_include_directories(stardust_demo PRIVATE ../../../3rdParty/glm)

target_link_libraries(${PROJECT_NAME} PRIVATE stardust)
