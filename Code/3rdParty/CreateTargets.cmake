set(3RD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Code/3rdParty)

find_package(OpenGL REQUIRED)

find_library(UNIVERSAL universal)

add_library(glad STATIC
        ${3RD_PARTY_DIR}/glad/src/glad.c
        )

target_include_directories(glad PUBLIC
        ${3RD_PARTY_DIR}/glad/include
        )

add_library(imgui STATIC
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

target_include_directories(imgui PUBLIC
        ${3RD_PARTY_DIR}/imgui/include
        )

set(3RD_PARTY_INCLUDE_DIRS
        ${3RD_PARTY_DIR}/glm/
        )