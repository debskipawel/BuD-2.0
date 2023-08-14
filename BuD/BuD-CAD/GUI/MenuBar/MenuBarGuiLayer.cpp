#include "MenuBarGuiLayer.h"

#include <imgui.h>

MenuBarGuiLayer::MenuBarGuiLayer(MainDataLayer& dataLayer)
	: BaseGuiLayer(dataLayer)
{
    m_MenuItems.emplace_back("File", [this]() { DrawSerializationSettings(); });
    m_MenuItems.emplace_back("Renderer", [this]() { DrawRendererSettings(); });
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
    auto& actionList = m_MainDataLayer.m_SceneDataLayer.m_ActionList;

    std::string message;

    if (ImGui::MenuItem("Undo", "Ctrl+Z"))
    {

    }

    if (ImGui::MenuItem("Redo", "Ctrl+Y"))
    {

    }
}

void MenuBarGuiLayer::DrawRendererSettings()
{
}
