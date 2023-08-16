#include "MenuBarGuiLayer.h"

#include <imgui.h>

#include <Visitors/Transform/UpdateTransformVisitor.h>

MenuBarGuiLayer::MenuBarGuiLayer(MainDataLayer& dataLayer)
	: BaseGuiLayer(dataLayer)
{
    m_ErrorSoundEffect = BuD::Audio::AudioSystem::Load("./Resources/Sounds/powiadomienie.mp3");
    
    auto defaultAudioDevice = BuD::Audio::AudioSystem::DefaultAudioDevice();
    BuD::Audio::AudioSystem::SetActiveDevice(defaultAudioDevice);

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
    std::string message;

    std::unique_ptr<AbstractVisitor> visitor = std::make_unique<UpdateTransformVisitor>();

    if (ImGui::MenuItem("Undo", "Ctrl+Z"))
    {
        auto undone = m_MainDataLayer.m_SceneDataLayer.m_SelectedForTransform.Undo();
        
        if (undone)
        {
            auto& cursor = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.m_MainCursor;

            cursor->SetPosition(m_MainDataLayer.m_SceneDataLayer.m_SelectedForTransform.Centroid());
        }
        else
        {
            BuD::Audio::AudioSystem::Play(m_ErrorSoundEffect);
        }
    }

    if (ImGui::MenuItem("Redo", "Ctrl+Y"))
    {
        auto redone = m_MainDataLayer.m_SceneDataLayer.m_SelectedForTransform.Redo();

        if (redone)
        {
            auto& cursor = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.m_MainCursor;

            cursor->SetPosition(m_MainDataLayer.m_SceneDataLayer.m_SelectedForTransform.Centroid());
        }
        else
        {
            BuD::Audio::AudioSystem::Play(m_ErrorSoundEffect);
        }
    }
}

void MenuBarGuiLayer::DrawRendererSettings()
{
}
