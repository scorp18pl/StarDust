cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)

project(stardust VERSION 0.1.0 DESCRIPTION "a simple OpenGL based 2D pixel renderer")

include(GNUInstallDirs)

include(3rdParty/CreateTargets.cmake)

include(${CMAKE_CURRENT_SOURCE_DIR}/include_files.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/source_files.cmake)

add_library(stardust STATIC
        ${SOURCE_FILES}
        ${INCLUDE_FILES}
        ${3RD_PARTY_SOURCES}
        )

target_link_libraries(stardust
        PRIVATE OpenGL::GL glfw ${UNIVERSAL})

target_include_directories(stardust
        PUBLIC Include ${IMGUI_INCLUDES}
        PRIVATE Source ${3RD_PARTY_INCLUDES} ${CMAKE_BINARY_DIR}/tools
        )

set_target_properties(stardust PROPERTIES VERSION ${PROJECT_VERSION})

configure_file(tools/stardust.pc.in tools/stardust.pc @ONLY)

add_subdirectory(Tests/StarDust)

if (PROJECT_IS_TOP_LEVEL)
    set(RESOURCES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../Resources")
else ()
    set(RESOURCES_PATH "${CMAKE_INSTALL_DATAROOTDIR}/Resources")
endif ()

install(DIRECTORY ../Resources/ DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/Resources)
configure_file(tools/config.h.in tools/config.h @ONLY)

install(DIRECTORY Include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS stardust
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        )

install(FILES ${CMAKE_BINARY_DIR}/tools/stardust.pc
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
