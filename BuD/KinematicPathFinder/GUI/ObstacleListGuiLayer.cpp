#include "ObstacleListGuiLayer.h"

#include <imgui.h>
#include <ranges>

ObstacleListGuiLayer::ObstacleListGuiLayer(MainDataLayer& mainDataLayer)
    : BaseGuiLayer(mainDataLayer)
{
}

auto ObstacleListGuiLayer::DrawGui() -> void
{
    if (ImGui::Begin("Obstacle list"))
    {
        auto& obstacles = m_MainDataLayer.m_SceneDataLayer.m_ObstacleCollection;
        const auto& obstacleList = obstacles.GetAll();

        auto idx = 0;
        std::optional<int> obstacleToRemove = std::nullopt;

        for (const auto& obstacle : obstacleList)
        {
            auto& obstacleColor = obstacle->Color();

            auto buttonSize = ImVec2(20.0f, 20.0f);
            auto panelWidth = ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x;
            auto padding = ImGui::GetStyle().ItemInnerSpacing.x;

            auto rectSize = ImVec2(panelWidth - (buttonSize.x + 2 * padding), buttonSize.y);

            auto colorLabel = std::format("###color_{}", idx);
            auto color = ImVec4(obstacleColor.x, obstacleColor.y, obstacleColor.z, 1.0f);

            ImGui::ColorButton(colorLabel.c_str(), color, 0, rectSize);

            ImGui::PushStyleColor(ImGuiCol_Button, { 0.6f, 0.0f, 0.0f, 1.0f });

            auto removeButtonLabel = std::format("X###button_{}", idx);

            ImGui::SameLine();

            if (ImGui::Button(removeButtonLabel.c_str(), buttonSize))
            {
                obstacleToRemove = idx;
            }

            ImGui::PopStyleColor();

            idx++;
        }

        if (obstacleToRemove.has_value() && obstacles.RemoveObstacle(obstacleToRemove.value()))
        {
            m_MainDataLayer.m_SceneDataLayer.RecalculateRobotPathsInParameterSpace();
            m_MainDataLayer.m_SceneDataLayer.RedrawRobotAngleParameterSpace();
            m_MainDataLayer.m_SceneDataLayer.RedrawPathInRobotAngleParameterSpace();
        }

        ImGui::End();
    }
}
