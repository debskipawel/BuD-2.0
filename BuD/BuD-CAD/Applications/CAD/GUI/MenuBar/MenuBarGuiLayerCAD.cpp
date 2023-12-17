#include "MenuBarGuiLayerCAD.h"

#include <imgui.h>
#include <ImGuiFileDialog.h>

#include <GCodeSerializer.h>

#include <Applications/CAD/Serializing/SceneSerializer.h>

#include <Applications/CAD/Visitors/Transform/UpdateTransformVisitor.h>
#include <Applications/CAD/Visitors/Serialization/SerializationVisitor.h>

#include <Applications/CAD/Path/ModelMillingToolPathsGenerator.h>

MenuBarGuiLayerCAD::MenuBarGuiLayerCAD(MainDataLayerCAD& dataLayer)
	: BaseGuiLayerCAD(dataLayer), m_MultiEyeSettingsPopupOpen(false), m_GenerateMillingPathsPopupOpen(false)
{
    ResetMillingParametersToDefault();

    m_MenuItems.emplace_back("File", [this]() { DrawFileSettings(); });
    m_MenuItems.emplace_back("Edit", [this]() { DrawEditSettings(); });
    m_MenuItems.emplace_back("Renderer", [this]() { DrawRendererSettings(); });
    m_MenuItems.emplace_back("Milling paths", [this]() { DrawMillingPathsMenu(); });

    m_RendererModeMenuItems.emplace_back("Standard mode", BuD::RenderingMode::STANDARD);
    m_RendererModeMenuItems.emplace_back("Anaglyph mode", BuD::RenderingMode::ANAGLYPH);
}

void MenuBarGuiLayerCAD::DrawGui()
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
    DrawGenerateMillingToolPathsPopup();
    
    DrawSaveSceneDialog();
    DrawLoadSceneDialog();
}

void MenuBarGuiLayerCAD::DrawFileSettings()
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

void MenuBarGuiLayerCAD::DrawEditSettings()
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

void MenuBarGuiLayerCAD::DrawRendererSettings()
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

void MenuBarGuiLayerCAD::DrawMillingPathsMenu()
{
    if (ImGui::MenuItem("Generate milling paths", nullptr, nullptr))
    {
        m_GenerateMillingPathsPopupOpen = true;
    }
}

void MenuBarGuiLayerCAD::ResetMillingParametersToDefault()
{
    m_MilledMaterialSize = dxm::Vector3(15.0f, 5.0f, 15.0f);
    m_MilledModelPlaneHeight = 1.6f;
}

void MenuBarGuiLayerCAD::DrawMultiEyeSettingsPopup()
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

        auto width = max.x - min.x;

        if (ImGui::Button("OK", { width, 20 }))
        {
            m_MultiEyeSettingsPopupOpen = false;
            ImGui::CloseCurrentPopup();

            m_MainDataLayer.m_AppStateDataLayer.Unfreeze();
        }

        ImGui::EndPopup();
    }
}

void MenuBarGuiLayerCAD::DrawGenerateMillingToolPathsPopup()
{
    if (m_GenerateMillingPathsPopupOpen)
    {
        m_MainDataLayer.m_AppStateDataLayer.Freeze();

        ImGui::OpenPopup("###generate_milling_paths");
    }

    if (ImGui::BeginPopupModal("Generate milling paths ###generate_milling_paths", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize))
    {
        ImGui::Text("Material size");
        ImGui::DragFloat3("###milling_material_size", reinterpret_cast<float*>(&m_MilledMaterialSize.x), 0.1f, 0.0f, 50.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp);

        ImGui::NewLine();

        ImGui::Text("Material model plane height");
        ImGui::DragFloat("###milling_model_plane_height", &m_MilledModelPlaneHeight, 0.1f, 0.0f, 10.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp);

        ImGui::Separator();

        auto max = ImGui::GetWindowContentRegionMax();
        auto min = ImGui::GetWindowContentRegionMin();

        auto width = max.x - min.x;
        auto innerSpacing = ImGui::GetStyle().ItemInnerSpacing.x;

        auto buttonWidth = (width - 2 * innerSpacing) / 2;

        if (ImGui::Button("OK", { buttonWidth, 20 }))
        {
            auto materialBlockDetails = MaterialBlockDetails(m_MilledMaterialSize, dxm::Vector3::Zero, 1.6f);
            auto millingPathGenerator = ModelMillingToolPathsGenerator(m_MainDataLayer.m_SceneDataLayer.m_SceneCAD);

            auto paths = millingPathGenerator.GeneratePaths(materialBlockDetails);

            auto serializer = GCP::GCodeSerializer();

            for (const auto& [filename, millingPath] : paths)
            {
                serializer.Serialize(millingPath, filename);
            }

            m_GenerateMillingPathsPopupOpen = false;
            ImGui::CloseCurrentPopup();

            m_MainDataLayer.m_AppStateDataLayer.Unfreeze();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel", { buttonWidth, 20 }))
        {
            m_GenerateMillingPathsPopupOpen = false;
            ImGui::CloseCurrentPopup();

            m_MainDataLayer.m_AppStateDataLayer.Unfreeze();
        }

        ImGui::EndPopup();
    }
}

void MenuBarGuiLayerCAD::OpenSaveSceneDialog()
{
    m_MainDataLayer.m_AppStateDataLayer.Freeze();
    
    auto currentPath = m_MainDataLayer.m_SceneDataLayer.m_PathToFile.empty()
        ? std::filesystem::current_path().string()
        : m_MainDataLayer.m_SceneDataLayer.m_PathToFile.parent_path().string();

    auto fileDialog = ImGuiFileDialog::Instance();
    fileDialog->OpenDialog("SaveSceneFileDialog", "Save scene file", ".json", currentPath.c_str(), 1, nullptr, ImGuiFileDialogFlags_Modal);
}

void MenuBarGuiLayerCAD::DrawSaveSceneDialog()
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

void MenuBarGuiLayerCAD::OpenLoadSceneDialog()
{
    m_MainDataLayer.m_AppStateDataLayer.Freeze();

    auto currentPath = m_MainDataLayer.m_SceneDataLayer.m_PathToFile.empty()
        ? std::filesystem::current_path().string()
        : std::format("{}\\", m_MainDataLayer.m_SceneDataLayer.m_PathToFile.parent_path().string());

    auto fileDialog = ImGuiFileDialog::Instance();
    fileDialog->OpenDialog("LoadSceneFileDialog", "Load scene file", ".json", currentPath.c_str(), 1, nullptr, ImGuiFileDialogFlags_Modal);
}

void MenuBarGuiLayerCAD::DrawLoadSceneDialog()
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
