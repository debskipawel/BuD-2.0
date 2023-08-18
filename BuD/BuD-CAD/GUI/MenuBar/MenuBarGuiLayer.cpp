#include "MenuBarGuiLayer.h"

#include <imgui.h>

#include <Visitors/Transform/UpdateTransformVisitor.h>

MenuBarGuiLayer::MenuBarGuiLayer(MainDataLayer& dataLayer)
	: BaseGuiLayer(dataLayer), m_MultiEyeSettingsPopupOpen(false)
{
    m_MenuItems.emplace_back("File", [this]() { DrawSerializationSettings(); });
    m_MenuItems.emplace_back("Renderer", [this]() { DrawRendererSettings(); });

    m_RendererModeMenuItems.emplace_back("Standard mode", BuD::RenderingMode::STANDARD);
    m_RendererModeMenuItems.emplace_back("Anaglyph mode", BuD::RenderingMode::ANAGLYPH);
}

void MenuBarGuiLayer::DrawGui()
{
	if (ImGui::BeginMainMenuBar())
	{
        for (const auto& menuItem : m_MenuItems)
        {
            if (ImGui::BeginMenu(menuItem.m_Label.c_str()))
            {
                menuItem.m_Handler();
                
                ImGui::EndMenu();
            }
        }

        ImGui::EndMainMenuBar();
	}

    DrawMultiEyeSettingsPopup();
}

void MenuBarGuiLayer::DrawSerializationSettings()
{
    auto& selectedTransform = m_MainDataLayer.m_SceneDataLayer.m_SelectedForTransform;

    auto canUndo = selectedTransform.CanUndo();
    auto canRedo = selectedTransform.CanRedo();

    if (ImGui::MenuItem("Undo", "Ctrl+Z", nullptr, canUndo))
    {
        auto undone = m_MainDataLayer.m_SceneDataLayer.m_SelectedForTransform.Undo();
        
        if (undone)
        {
            auto& cursor = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.m_CentroidCursor;

            cursor->SetPosition(m_MainDataLayer.m_SceneDataLayer.m_SelectedForTransform.Centroid());
        }
    }

    if (ImGui::MenuItem("Redo", "Ctrl+Y", nullptr, canRedo))
    {
        auto redone = m_MainDataLayer.m_SceneDataLayer.m_SelectedForTransform.Redo();

        if (redone)
        {
            auto& cursor = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.m_CentroidCursor;

            cursor->SetPosition(m_MainDataLayer.m_SceneDataLayer.m_SelectedForTransform.Centroid());
        }
    }
}

void MenuBarGuiLayer::DrawRendererSettings()
{
    auto renderingMode = BuD::Renderer::GetRenderingMode();

    if (ImGui::BeginMenu("Rendering mode"))
    {
        for (auto& rendererModeMenuItem : m_RendererModeMenuItems)
        {
            auto selected = renderingMode == rendererModeMenuItem.m_RenderingMode;

            if (ImGui::MenuItem(rendererModeMenuItem.m_Label.c_str(), nullptr, &selected))
            {
                BuD::Renderer::SetRenderingMode(rendererModeMenuItem.m_RenderingMode);
            }
        }

        ImGui::EndMenu();
    }

    auto isMultiEyeMode = BuD::Renderer::IsMultiEyeMode();

    if (ImGui::MenuItem("Multi-eye settings", nullptr, nullptr, isMultiEyeMode))
    {
        m_MultiEyeSettingsPopupOpen = true;
    }
}

void MenuBarGuiLayer::DrawMultiEyeSettingsPopup()
{
    if (m_MultiEyeSettingsPopupOpen)
    {
        m_MainDataLayer.m_AppStateDataLayer.Freeze();

        ImGui::OpenPopup("###multi_eye_settings");
    }

    if (ImGui::BeginPopupModal("Multi-eye settings ###multi_eye_settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize))
    {
        auto multiEyeSettings = BuD::Renderer::GetMultiEyeSettings();

        if (ImGui::DragFloat("Eye distance", &multiEyeSettings.m_EyeDistance, 0.01f)
            || ImGui::DragFloat("Focus plane", &multiEyeSettings.m_FocusPlane, 0.25f))
        {
            multiEyeSettings.m_EyeDistance = max(multiEyeSettings.m_EyeDistance, 0.0f);
            multiEyeSettings.m_FocusPlane = max(multiEyeSettings.m_FocusPlane, 0.0f);

            BuD::Renderer::SetMultiEyeSettings(multiEyeSettings);
        }

        ImGui::Separator();

        auto max = ImGui::GetWindowContentRegionMax();
        auto min = ImGui::GetWindowContentRegionMin();

        if (ImGui::Button("OK", { max.x - min.x, 20 }))
        {
            m_MultiEyeSettingsPopupOpen = false;
            ImGui::CloseCurrentPopup();

            m_MainDataLayer.m_AppStateDataLayer.Unfreeze();
        }

        ImGui::EndPopup();
    }
}
