set(3RD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty)

find_package(OpenGL REQUIRED)
find_library(UNIVERSAL universal)

set(GLAD_SOURCES ${3RD_PARTY_DIR}/glad/src/glad.c)
set(GLAD_INCLUDES ${3RD_PARTY_DIR}/glad/include)

set(IMGUI_SOURCES
        ${3RD_PARTY_DIR}/imgui/src/imgui/imgui.cpp
        ${3RD_PARTY_DIR}/imgui/src/imgui/imgui_demo.cpp
        ${3RD_PARTY_DIR}/imgui/src/imgui/imgui_draw.cpp
        ${3RD_PARTY_DIR}/imgui/src/imgui/imgui_impl_glfw.cpp
        ${3RD_PARTY_DIR}/imgui/src/imgui/imgui_impl_opengl3.cpp
        ${3RD_PARTY_DIR}/imgui/src/imgui/imgui_tables.cpp
        ${3RD_PARTY_DIR}/imgui/src/imgui/imgui_widgets.cpp
        ${3RD_PARTY_DIR}/imgui/include/imgui/imconfig.h
        ${3RD_PARTY_DIR}/imgui/include/imgui/imgui.h
        ${3RD_PARTY_DIR}/imgui/include/imgui/imgui_impl_glfw.h
        ${3RD_PARTY_DIR}/imgui/include/imgui/imgui_impl_opengl3.h
        ${3RD_PARTY_DIR}/imgui/include/imgui/imgui_impl_opengl3_loader.h
        ${3RD_PARTY_DIR}/imgui/include/imgui/imgui_internal.h
        ${3RD_PARTY_DIR}/imgui/include/imgui/imstb_rectpack.h
        ${3RD_PARTY_DIR}/imgui/include/imgui/imstb_textedit.h
        ${3RD_PARTY_DIR}/imgui/include/imgui/imstb_truetype.h
        )
set(IMGUI_INCLUDES ${3RD_PARTY_DIR}/imgui/include)

set(3RD_PARTY_SOURCES ${GLAD_SOURCES} ${IMGUI_SOURCES})
set(3RD_PARTY_INCLUDES ${GLAD_INCLUDES} ${IMGUI_INCLUDES})
