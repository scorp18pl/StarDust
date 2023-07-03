#pragma once

#include <filesystem>

namespace Star::Utils
{
    void GlCheckError(const char* file, int line);
    unsigned int GetSizeOfType(unsigned int type);

    std::filesystem::path GetResourcesPath();

#ifndef NDEBUG

#define GL_CHECK(x) { x; Star::Utils::GlCheckError(__FILE__, __LINE__); }

#else

#define GL_CHECK(expr) (expr)

#endif
} // namespace Star::Utils
