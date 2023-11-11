#include "MenuBarGuiLayerCNC.h"

#include <imgui.h>

MenuBarGuiLayerCNC::MenuBarGuiLayerCNC(MainDataLayerCNC& dataLayer)
	: BaseGuiLayerCNC(dataLayer)
{
}

void MenuBarGuiLayerCNC::DrawGui()
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
