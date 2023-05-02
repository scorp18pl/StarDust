#pragma once

#include <StarDust/Window.h>

namespace Str
{
    class Drawable
    {
    public:
        friend void Window::Draw(Drawable* drawable);
        friend void Renderer::AddDrawRequest(Drawable* drawable);

    protected:
        [[nodiscard]] virtual const VertexList& GetVertices() = 0;
        [[nodiscard]] virtual const IndexList& GetIndices() = 0;
    };
} // namespace Str
