#pragma once

#include "Test.h"
#include <StarDust/Model/ModelInstance3D.h>
#include <Universal/Graphics/Bitmap.h>
#include <Universal/Math/Matrix/Matrix4x4f.h>
#include <Universal/Math/Random/Generator.h>

class GameOfLifeTest : public Test
{
public:
    GameOfLifeTest();
    ~GameOfLifeTest() override;

    void OnUpdate(float deltaTime) override;
    void OnRender(Star::Window& window) override;
    void OnImGuiRender() override;

private:
    enum class CellState
    {
        Dead,
        Alive
    };

    class Board
    {
    public:
        Board(unsigned int width, unsigned int height);
        ~Board() = default;

        // clang-format off
        [[nodiscard]] unsigned int GetWidth() const { return m_width; }
        [[nodiscard]] unsigned int GetHeight() const { return m_height; }
        // clang-format on
        [[nodiscard]] CellState GetCellState(
            unsigned int x, unsigned int y) const;

        void Update();
        void SetCellState(unsigned int x, unsigned int y, CellState state);

    private:
        std::vector<CellState> m_currentCells, m_nextCells;
        unsigned int m_width{ 0U }, m_height{ 0U };
    };

    Uni::Math::Matrix4x4f m_viewMatrix, m_projectionMatrix;
    Star::Texture m_texture;
    Board m_board;
    Uni::Grpx::Bitmap m_bitmap;
    Star::ModelInstance3D m_instance;
    Star::LightSource m_lightSource;
    bool m_isPaused{ false };
    float m_time{ 0.0f };
    Uni::Grpx::Color m_color{Uni::Grpx::Color::White};
    float m_colorTime{ 0.0f };
    Uni::Math::Rand::Generator m_generator;
};
