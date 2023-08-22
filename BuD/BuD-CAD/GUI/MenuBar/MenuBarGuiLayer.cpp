#include "MenuBarGuiLayer.h"

#include <imgui.h>
#include <ImGuiFileDialog.h>

#include <Serializing/SceneSerializer.h>

#include <Visitors/Transform/UpdateTransformVisitor.h>
#include <Visitors/Serialization/SerializationVisitor.h>

MenuBarGuiLayer::MenuBarGuiLayer(MainDataLayer& dataLayer)
	: BaseGuiLayer(dataLayer), m_MultiEyeSettingsPopupOpen(false)
{
    m_MenuItems.emplace_back("File", [this]() { DrawFileSettings(); });
    m_MenuItems.emplace_back("Edit", [this]() { DrawEditSettings(); });
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
    
    DrawSaveSceneDialog();
    DrawLoadSceneDialog();
}

void MenuBarGuiLayer::DrawFileSettings()
{
    if (ImGui::MenuItem("Load scene"))
    {
        OpenLoadSceneDialog();
    }

    ImGui::Separator();

    if (ImGui::MenuItem("Save", "Ctrl+S"))
    {
        auto serializer = SceneSerializer(m_MainDataLayer);
        
        if (!serializer.Save())
        {
            OpenSaveSceneDialog();
        }
    }

    if (ImGui::MenuItem("Save as...", "Ctrl+Shift+S"))
    {
        OpenSaveSceneDialog();
    }
}

void MenuBarGuiLayer::DrawEditSettings()
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

void MenuBarGuiLayer::OpenSaveSceneDialog()
{
    m_MainDataLayer.m_AppStateDataLayer.Freeze();
    
    auto currentPath = m_MainDataLayer.m_SceneDataLayer.m_PathToFile.empty()
        ? std::filesystem::current_path()
        : m_MainDataLayer.m_SceneDataLayer.m_PathToFile.parent_path();

    auto fileDialog = ImGuiFileDialog::Instance();
    fileDialog->OpenDialog("SaveSceneFileDialog", "Save scene file", ".json", (const char*)currentPath.c_str(), 1, nullptr, ImGuiFileDialogFlags_Modal);
}

void MenuBarGuiLayer::DrawSaveSceneDialog()
{
    auto fileDialog = ImGuiFileDialog::Instance();

    bool flag = false;

    if (fileDialog->Display("SaveSceneFileDialog"))
    {
        flag = true;

        if (fileDialog->IsOk())
        {
            m_MainDataLayer.m_SceneDataLayer.m_PathToFile = fileDialog->GetFilePathName();
            
            auto serializer = SceneSerializer(m_MainDataLayer);
            serializer.Save();

            m_MainDataLayer.m_AppStateDataLayer.Unfreeze();
        }

        fileDialog->Close();
    }

    if (flag && !fileDialog->IsOpened())
    {
        m_MainDataLayer.m_AppStateDataLayer.Unfreeze();
    }
}

void MenuBarGuiLayer::OpenLoadSceneDialog()
{
    m_MainDataLayer.m_AppStateDataLayer.Freeze();

    auto currentPath = m_MainDataLayer.m_SceneDataLayer.m_PathToFile.empty()
        ? std::filesystem::current_path()
        : m_MainDataLayer.m_SceneDataLayer.m_PathToFile.parent_path();

    auto fileDialog = ImGuiFileDialog::Instance();
    fileDialog->OpenDialog("LoadSceneFileDialog", "Load scene file", ".json", (const char*)currentPath.c_str(), 1, nullptr, ImGuiFileDialogFlags_Modal);
}

void MenuBarGuiLayer::DrawLoadSceneDialog()
{
    auto fileDialog = ImGuiFileDialog::Instance();

    bool flag = false;

    if (fileDialog->Display("LoadSceneFileDialog"))
    {
        flag = true;

        if (fileDialog->IsOk())
        {
            m_MainDataLayer.m_SceneDataLayer.m_PathToFile = fileDialog->GetFilePathName();
            
            auto serializer = SceneSerializer(m_MainDataLayer);
            serializer.Load();

            m_MainDataLayer.m_AppStateDataLayer.Unfreeze();
        }

        fileDialog->Close();
    }

    if (flag && !fileDialog->IsOpened())
    {
        m_MainDataLayer.m_AppStateDataLayer.Unfreeze();
    }
}
