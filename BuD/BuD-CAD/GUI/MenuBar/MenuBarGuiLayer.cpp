#include "MenuBarGuiLayer.h"

#include <imgui.h>

#include <Visitors/Transform/UpdateTransformVisitor.h>

MenuBarGuiLayer::MenuBarGuiLayer(MainDataLayer& dataLayer)
	: BaseGuiLayer(dataLayer)
{
    m_MenuItems.emplace_back("File", [this]() { DrawSerializationSettings(); });
    m_MenuItems.emplace_back("Renderer", [this]() { DrawRendererSettings(); });

    m_RendereModeMenuItems.emplace_back("Anaglyph mode", BuD::RenderingMode::ANAGLYPH);
    m_RendereModeMenuItems.emplace_back("Standard mode", BuD::RenderingMode::STANDARD);
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
        for (auto& rendererModeMenuItem : m_RendereModeMenuItems)
        {
            auto selected = renderingMode == rendererModeMenuItem.m_RenderingMode;

            if (ImGui::MenuItem(rendererModeMenuItem.m_Label.c_str(), nullptr, &selected))
            {
                BuD::Renderer::SetRenderingMode(rendererModeMenuItem.m_RenderingMode);
            }
        }

        ImGui::EndMenu();
    }

    auto isRenderingModeAnaglyph = renderingMode == BuD::RenderingMode::ANAGLYPH;

    if (ImGui::MenuItem("Anaglyph settings", nullptr, nullptr, isRenderingModeAnaglyph))
    {

    }
}
