cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 20)

project(pixel_dust VERSION 0.1.0 DESCRIPTION "a simple OpenGL based 2D pixel renderer")

include(GNUInstallDirs)

add_library(pixel_dust STATIC
        Code/Source/PixelDust/Window/Window.cpp
        Code/Include/PixelDust/Window/Window.h
)

include(Find3rdPartyLibs.cmake)

target_link_libraries(pixel_dust PUBLIC OpenGL::GL glad glfw ${UNIVERSAL})

target_include_directories(pixel_dust
        PUBLIC Code/Include
        PRIVATE Code/Source)

set_target_properties(pixel_dust PROPERTIES VERSION ${PROJECT_VERSION})

configure_file(tools/pixel_dust.pc.in tools/pixel_dust.pc @ONLY)

install(DIRECTORY Code/Include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS pixel_dust
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        )

install(FILES ${CMAKE_BINARY_DIR}/tools/pixel_dust.pc
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
