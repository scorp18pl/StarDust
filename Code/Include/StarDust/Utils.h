#pragma once

namespace Str::Utils
{
    void GlCheckError(const char* file, int line);
    unsigned int GetSizeOfType(unsigned int type);

#ifndef NDEBUG

#define GL_CHECK(x) do { x; Str::Utils::GlCheckError(__FILE__, __LINE__); } while (0)

#else

#define GL_CHECK(expr) (expr)

#endif
} // namespace Str::Utils
