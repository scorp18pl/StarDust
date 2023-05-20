cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 20)

project(stardust VERSION 0.1.0 DESCRIPTION "a simple OpenGL based 2D pixel renderer")

include(GNUInstallDirs)

add_library(stardust STATIC
        Code/Include/StarDust/Window.h
        Code/Include/StarDust/Buffer/VertexBuffer.h
        Code/Include/StarDust/Buffer/IndexBuffer.h
        Code/Include/StarDust/VertexArray.h
        Code/Include/StarDust/Shader/ShaderProgram.h
        Code/Source/StarDust/Window.cpp
        Code/Source/StarDust/Buffer/VertexBuffer.cpp
        Code/Source/StarDust/Buffer/IndexBuffer.cpp
        Code/Source/StarDust/VertexArray.cpp
        Code/Source/StarDust/Shader/ShaderProgram.cpp
        Code/Source/StarDust/Utils.cpp
        Code/Include/StarDust/Utils.h
        Code/Source/StarDust/Buffer/VertexBufferLayout.cpp
        Code/Include/StarDust/Buffer/VertexBufferLayout.h
        Code/Source/StarDust/Texture.cpp
        Code/Include/StarDust/Texture.h
        Code/Source/StarDust/Buffer/FrameBuffer.cpp
        Code/Include/StarDust/Buffer/FrameBuffer.h
        Code/Source/StarDust/Renderer.cpp
        Code/Include/StarDust/Renderer.h
        Code/Source/StarDust/Shader/ShaderProgramRegistry.cpp
        Code/Include/StarDust/Shader/ShaderProgramRegistry.h
        Code/Source/StarDust/Model/MeshRegistry.cpp
        Code/Include/StarDust/Model/MeshRegistry.h
        Code/Source/StarDust/Model/Mesh.cpp
        Code/Include/StarDust/Model/Mesh.h
        Code/Source/StarDust/Model/Model.cpp
        Code/Include/StarDust/Model/Model.h
        Code/Source/StarDust/Model/ModelInstance.cpp
        Code/Include/StarDust/Model/ModelInstance.h
        Code/Source/StarDust/Buffer/RenderBuffer.cpp
        Code/Include/StarDust/Buffer/RenderBuffer.h
        Code/Tests/StarDust/Tests.cpp
        Code/Include/StarDust/LightSource.h
        Code/Source/StarDust/Model/ModelInstanceSystem.cpp
        Code/Include/StarDust/Model/ModelInstanceSystem.h
        Code/Source/StarDust/LightSource.cpp)

include(Code/3rdParty/CreateTargets.cmake)

target_link_libraries(stardust PUBLIC OpenGL::GL glad glfw ${UNIVERSAL} imgui)

target_include_directories(stardust
        PUBLIC Code/Include
        PRIVATE Code/Source ${3RD_PARTY_INCLUDE_DIRS}
        )

set_target_properties(stardust PROPERTIES VERSION ${PROJECT_VERSION})

configure_file(tools/stardust.pc.in tools/stardust.pc @ONLY)

add_subdirectory(Code/Tests/StarDust)

install(DIRECTORY Code/Include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS stardust
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        )

install(FILES ${CMAKE_BINARY_DIR}/tools/stardust.pc
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
