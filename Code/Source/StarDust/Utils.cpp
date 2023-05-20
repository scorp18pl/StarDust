#include <StarDust/Utils.h>
#include <glad/glad.h>
#include <iostream>
#include <string>

namespace Star
{
    void Utils::GlCheckError(const char* file, int line)
    {
        GLenum errorCode = 0x0;
        while ((errorCode = glGetError()) != GL_NO_ERROR)
        {
            std::string error;
            switch (errorCode)
            {
            case GL_INVALID_ENUM:
                error = "INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                error = "INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                error = "INVALID_OPERATION";
                break;
            case GL_STACK_OVERFLOW:
                error = "STACK_OVERFLOW";
                break;
            case GL_STACK_UNDERFLOW:
                error = "STACK_UNDERFLOW";
                break;
            case GL_OUT_OF_MEMORY:
                error = "OUT_OF_MEMORY";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                error = "INVALID_FRAMEBUFFER_OPERATION";
                break;
            }

            throw std::runtime_error(
                "OpenGL Error: " + error + " in " + file + " at line " +
                std::to_string(line));
        }
    }

    unsigned int Utils::GetSizeOfType(unsigned int type)
    {
        switch (type)
        {
        case GL_FLOAT:
            return sizeof(GLfloat);
        case GL_UNSIGNED_INT:
            return sizeof(GLuint);
        case GL_UNSIGNED_BYTE:
            return sizeof(GLubyte);
        }
        return 0;
    }

    std::filesystem::path Utils::GetResourcesPath()
    {
        std::filesystem::path currentFilePath{ __FILE__ };
        std::filesystem::path currentDirectoryPath =
            currentFilePath.parent_path();

        return currentDirectoryPath.parent_path().parent_path().parent_path() /
            "Resources";
    }
} // namespace Star