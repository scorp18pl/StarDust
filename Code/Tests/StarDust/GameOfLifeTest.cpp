#include "GameOfLifeTest.h"
#include <StarDust/Model/MeshRegistry.h>
#include <StarDust/Renderer.h>
#include <StarDust/Shader/ShaderProgram.h>
#include <StarDust/Utilities/Math.h>
#include <Universal/Math/Random/Generator.h>
#include <imgui.h>

GameOfLifeTest::Board::Board(unsigned int width, unsigned int height)
    : m_width(width)
    , m_height(height)
    , m_currentCells(width * height, CellState::Alive)
    , m_nextCells(width * height, CellState::Alive)
{
    Uni::Math::Rand::Generator generator;
    for (size_t i = 0; i < m_width * m_height; ++i)
    {
        m_currentCells[i] =
            generator.GenerateInRange(0, 1) == 0 ? CellState::Dead : CellState::Alive;
    }
}

void GameOfLifeTest::Board::Update()
{
    for (size_t i = 0; i < m_width; ++i)
    {
        for (size_t j = 0; j < m_height; ++j)
        {
            unsigned int aliveNeighbours = 0;
            for (int x = -1; x <= 1; ++x)
            {
                for (int y = -1; y <= 1; ++y)
                {
                    if (x == 0 && y == 0)
                    {
                        continue;
                    }

                    if (GetCellState(i + x, j + y) == CellState::Alive)
                    {
                        ++aliveNeighbours;
                    }
                }
            }

            if (GetCellState(i, j) == CellState::Alive)
            {
                if (aliveNeighbours < 2 || aliveNeighbours > 3)
                {
                    SetCellState(i, j, CellState::Dead);
                }
            }
            else
            {
                if (aliveNeighbours == 3)
                {
                    SetCellState(i, j, CellState::Alive);
                }
            }
        }
    }

    m_currentCells = m_nextCells;
}

GameOfLifeTest::CellState GameOfLifeTest::Board::GetCellState(unsigned int x, unsigned int y) const
{
    // not valid for values less than -dimension.
    x = (x + m_width) % m_width;
    y = (y + m_height) % m_height;
    return m_currentCells[x + y * m_width];
}

void GameOfLifeTest::Board::SetCellState(
    unsigned int x, unsigned int y, GameOfLifeTest::CellState state)
{
    // not valid for values less than -dimension.
    x = (x + m_width) % m_width;
    y = (y + m_height) % m_height;
    m_nextCells[x + y * m_width] = state;
}

GameOfLifeTest::GameOfLifeTest()
    : Test(Test::TestType::GameOfLife)
    , m_projectionMatrix{ Star::Utils::CreateOrthographicProjectionMatrix(
          -0.5f, 0.5f, -0.5f, 0.5f, -2.0f, 2.0f) }
    , m_viewMatrix{ Star::Utils::CreateLookAtMatrix(
          Uni::Math::Vector3f::CreateAxisX(),
          Uni::Math::Vector3f::CreateZero(),
          Uni::Math::Vector3f::CreateAxisZ()) }
    , m_instance{ Star::MeshRegistry::Get().GetMeshId("Rectangle"),
                  Uni::Math::Transform(),
                  Uni::Grpx::Color::White, }
    , m_lightSource{ Star::LightSourceType::Directional,
                     Star::LightData{
                         -Uni::Math::Vector3f::CreateAxisX(),
                         Uni::Grpx::Color::White,
                         1.0f,
                     }, }
    , m_board(128U, 128U)
    , m_bitmap(m_board.GetWidth(), m_board.GetHeight(), Uni::Grpx::Channel::Alpha)
{
    m_texture = Star::Texture(m_bitmap.GetData(), m_bitmap.GetWidth(), m_bitmap.GetHeight());

    m_texture.Bind(1);

    Star::ShaderProgram& shader = Star::Renderer::Get().GetUsedShaderProgram();
    shader.Bind();

    shader.SetUniformMat4x4f("u_view", m_viewMatrix);
    shader.SetUniformMat4x4f("u_proj", m_projectionMatrix);

    shader.SetUniform1i("u_texture", 1);
}

void GameOfLifeTest::OnUpdate(float deltaTime)
{
    static constexpr float Frequency = 10.0f;
    static constexpr float RefreshTime = 1.0e3f / Frequency;

    static constexpr float ColorFrequency = 119.0f / 60.0f;
    static constexpr float ColorRefreshTime = 1.0e3f / ColorFrequency;

    m_time += deltaTime;
    m_colorTime += deltaTime;
    if (m_isPaused || m_time < RefreshTime)
    {
        return;
    }
    m_time = 0.0f;

    if (m_colorTime >= ColorRefreshTime)
    {
        m_colorTime = 0.0f;
        m_color = Uni::Grpx::Color::Green;
    }

    m_board.Update();

    for (size_t i = 0; i < m_board.GetWidth(); ++i)
    {
        for (size_t j = 0; j < m_board.GetHeight(); ++j)
        {
            if (m_board.GetCellState(i, j) == CellState::Alive)
            {
                m_bitmap.SetPixelColor(i, j, m_color);
            }
            else
            {
                m_bitmap.SetPixelColor(i, j, Uni::Grpx::Color::Black);
            }
        }
    }

    m_texture.SetData(m_bitmap.GetData(), m_bitmap.GetWidth(), m_bitmap.GetHeight());
}

void GameOfLifeTest::OnRender(Star::Window& window) {}

void GameOfLifeTest::OnImGuiRender() { ImGui::Checkbox("Paused", &m_isPaused); }

GameOfLifeTest::~GameOfLifeTest() {}
