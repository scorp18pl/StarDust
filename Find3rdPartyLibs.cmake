find_package(OpenGL REQUIRED)

find_library(UNIVERSAL universal)

add_library(glad STATIC
        3rdParty/glad/src/glad.c
        )

target_include_directories(glad PUBLIC
        3rdParty/glad/include
        )