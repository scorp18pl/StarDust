#include "AxesTest.h"
#include "InstancingTest.h"
#include <StarDust/Shader/ShaderProgramRegistry.h>
#include <StarDust/Window.h>
#include <Universal/Graphics/ColorPalette.h>
#include <Universal/System/Clock.h>
#include <imgui.h>

int main()
{
    Str::Window window = Str::Window(1080, 1080, "StarDust Tests");

    Test* currentTest = nullptr;
    int pixelizationFactor = 4;
    float distortionFactor = 0.1f;
    float colorShiftFactor = 0.004f;
    bool enablePaletteSnap = true;

    Uni::Math::Vector3f lightDir = { 0.0f, 0.0f, -1.0f };
    Uni::Math::Vector4f lightColor = { 1.0f, 1.0f, 1.0f, 1.0f };
    float ambientStrenght = 0.1f;
    float diffuseStrenght = 0.4;
    float specularStrenght = 0.5f;
    float shininess = 32.0f;

    unsigned int currentColorPalette = 0U;

    std::vector<Uni::Grpx::ColorPalette> colorPalettes = {
        Uni::Grpx::ColorPalette::LoadFromFile(
            "../../../../Resources/Palettes/cretaceous-16.hex"),
        Uni::Grpx::ColorPalette::LoadFromFile(
            "../../../../Resources/Palettes/apollo.hex"),
        Uni::Grpx::ColorPalette::LoadFromFile(
            "../../../../Resources/Palettes/endesga-32.hex"),
        Uni::Grpx::ColorPalette::LoadFromFile(
            "../../../../Resources/Palettes/resurrect-64.hex"),
        Uni::Grpx::ColorPalette::LoadFromFile(
            "../../../../Resources/Palettes/universalis-42.hex"),
    };

    Str::ShaderProgram& modelInstanceShader =
        Str::ShaderProgramRegistry::Get().GetShaderProgram("model_instance");

    Uni::Sys::Clock clock;

    while (window.IsOpen())
    {
        window.Update();
        window.SetPixelizationFactor(pixelizationFactor);

        Str::ShaderProgramRegistry::Get()
            .GetShaderProgram("pixelate")
            .SetUniform1f("distortionFactor", distortionFactor);

        Str::ShaderProgramRegistry::Get()
            .GetShaderProgram("pixelate")
            .SetUniform1f("colorShiftFactor", colorShiftFactor);

        ImGui::Begin("Test Framework");

        if (ImGui::CollapsingHeader("Lighting"))
        {
            ImGui::Indent(4.0f);


            ImGui::InputFloat("Shininess:", &shininess);
            ImGui::InputFloat("Ambient strenght:", &ambientStrenght);
            ImGui::InputFloat("Diffuse strenght:", &diffuseStrenght);
            ImGui::InputFloat("Specular strenght:", &specularStrenght);

            ImGui::InputFloat3(
                "Light direction:",
                static_cast<float*>(static_cast<void*>(&lightDir)));

            ImGui::ColorEdit3(
                "Light color:",
                static_cast<float*>(static_cast<void*>(&lightColor)));

            ImGui::Unindent(4.0f);
        }

        ImGui::SliderInt("Pixelization factor:", &pixelizationFactor, 1, 8);
        ImGui::SliderFloat("Distortion factor:", &distortionFactor, 0.0f, 1.0f);
        ImGui::SliderFloat(
            "Color shift factor:", &colorShiftFactor, 0.0f, 1.0f);
        ImGui::Checkbox("Enable palette snap", &enablePaletteSnap);

        if (enablePaletteSnap)
        {
            ImGui::Text(
                "Color palette: %s",
                colorPalettes[currentColorPalette].GetName().c_str());

            ImGui::SliderInt(
                "Color palette:",
                (int*)&currentColorPalette,
                0,
                colorPalettes.size() - 1);
        }

        Str::ShaderProgramRegistry::Get()
            .GetShaderProgram("model_instance")
            .SetUniform1i(
                "u_colorPaletteCount",
                enablePaletteSnap *
                    colorPalettes[currentColorPalette].GetColors().size());

        modelInstanceShader.SetUniform4fArray(
            "u_colorPalette",
            colorPalettes[currentColorPalette].GetColors().data(),
            colorPalettes[currentColorPalette].GetColors().size());

        if (ImGui::BeginCombo(
                "Current Test",
                currentTest ? currentTest->GetName().c_str() : "None",
                ImGuiComboFlags_None))
        {
            for (auto& [type, name] : Test::TestTypeMap)
            {
                bool isSelected = currentTest && currentTest->GetName() == name;
                if (ImGui::Selectable(name.c_str(), isSelected))
                {
                    delete currentTest;
                    currentTest = Test::CreateTest(type);
                }
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        modelInstanceShader.SetUniform1f("u_ambientStrenght", ambientStrenght);
        modelInstanceShader.SetUniform1f("u_diffuseStrenght", diffuseStrenght);
        modelInstanceShader.SetUniform1f(
            "u_specularStrenght", specularStrenght);
        modelInstanceShader.SetUniform1f("u_shininess", shininess);

        modelInstanceShader.SetUniform3f(
            "u_lightDir", lightDir.m_x, lightDir.m_y, lightDir.m_z);

        modelInstanceShader.SetUniform4f(
            "u_lightColor",
            lightColor.m_x,
            lightColor.m_y,
            lightColor.m_z,
            lightColor.m_w);

        if (currentTest)
        {
            currentTest->OnUpdate(clock.GetElapsedTime());
            clock.Reset();
            currentTest->OnRender(window);

            if (ImGui::CollapsingHeader("Test Configuration"))
            {
                ImGui::Indent(4.0f);
                currentTest->OnImGuiRender();
                ImGui::Unindent(4.0f);
            }
        }
        ImGui::End();

        window.Clear();
    }
}