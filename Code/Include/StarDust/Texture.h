#pragma once

#include <Universal/Graphics/Bitmap.h>
#include <string>
#include <glad/glad.h>

namespace Star
{
    class Texture
    {
    public:
        explicit Texture(const void* data = nullptr, unsigned int width = 0U, unsigned int height = 0U);
        Texture(const Texture&);
        Texture(Texture&&) = delete;
        ~Texture();

        [[nodiscard]] unsigned int GetID() const;

        void Bind(unsigned int textureSlot = 0U) const;
        static void Unbind();

        void SetData(const void* data, unsigned int width, unsigned int height);

        Texture& operator=(const Texture&);

    private:
        unsigned int m_id;
        const void* m_data;
        unsigned int m_width, m_height;
    };
} // namespace Star
