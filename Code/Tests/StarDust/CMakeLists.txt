cmake_minimum_required(VERSION 3.15)
project(stardust_tests)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} Tests.cpp
        Test.cpp
        Test.h
        AxesTest.cpp
        AxesTest.h
        InstancingTest.cpp
        InstancingTest.h
        MeshTest.cpp
        MeshTest.h
        TestScene.cpp
        TestScene.h
        TerrainGenerationTest.cpp TerrainGenerationTest.h CubeGameTest.cpp CubeGameTest.h)

target_include_directories(${PROJECT_NAME} PRIVATE ../../3rdParty/glm)
target_include_directories(${PROJECT_NAME} PRIVATE .)

target_link_libraries(${PROJECT_NAME} PRIVATE stardust)
