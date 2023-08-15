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
        auto action = m_MainDataLayer.m_SceneDataLayer.m_SelectedForTransform.Undo();
        
        if (action)
        {
            for (auto& [id, object] : action->m_TransformedObjects)
            {
                auto objectShared = object.lock();

                if (!objectShared)
                {
                    continue;
                }

                auto& initialTransform = action->m_OriginalTransforms.at(id);

                objectShared->m_Transform = initialTransform;

                visitor->Visit(object);
            }
        }
        else
        {
            BuD::Audio::AudioSystem::Play(m_ErrorSoundEffect);
        }
    }

    if (ImGui::MenuItem("Redo", "Ctrl+Y"))
    {
        auto action = m_MainDataLayer.m_SceneDataLayer.m_SelectedForTransform.Redo();

        if (action)
        {
            for (auto& [id, object] : action->m_TransformedObjects)
            {
                auto objectShared = object.lock();

                if (!objectShared)
                {
                    continue;
                }

                auto& targetTransform = action->m_TargetTransforms.at(id);

                objectShared->m_Transform = targetTransform;

                visitor->Visit(object);
            }
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
