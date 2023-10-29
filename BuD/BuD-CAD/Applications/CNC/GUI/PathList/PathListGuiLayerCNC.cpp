#include "PathListGuiLayerCNC.h"

#include <GCodeParser.h>

#include <filesystem>
#include <format>
#include <imgui.h>
#include <imgui_internal.h>
#include <ImGuiFileDialog.h>

#include <Applications/CNC/Factories/MillingTool/MainMillingToolFactory.h>

PathListGuiLayerCNC::PathListGuiLayerCNC(MainDataLayerCNC& dataLayer)
	: BaseGuiLayerCNC(dataLayer)
{
	m_MillingToolFactory = std::make_unique<MainMillingToolFactory>();
}

void PathListGuiLayerCNC::DrawGui()
{
	if (ImGui::Begin("Path list ###path_list"))
	{
		DrawPathList();

		DrawAddPathButton();

		DrawImportPathDialog();

		ImGui::End();
	}
}

void PathListGuiLayerCNC::DrawPathList()
{
	auto& pathList = m_MainDataLayer.m_PathListDataLayer.m_PathList;

	for (size_t pathIndex = 0; pathIndex < pathList.size(); ++pathIndex)
	{
		const auto& path = pathList[pathIndex];
		
		auto buttonSize = ImVec2(20.0f, 20.0f);
		auto panelWidth = ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x;
		auto padding = ImGui::GetStyle().ItemSpacing.x;

		auto selectableWidth = max(50.0f, panelWidth - padding - 3 * (buttonSize.x + padding));

		DrawPathSelectable(path, selectableWidth);

		ImGui::SameLine(selectableWidth + padding, padding);

		// ------------------------- BUTTON FOR MOVING PATH UP -----------------------
		if (pathIndex == 0)
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}

		if (ImGui::Button("+", buttonSize))
		{
			auto temp = pathList[pathIndex - 1];
			pathList[pathIndex - 1] = m_MainDataLayer.m_PathListDataLayer.m_PathList[pathIndex];
			m_MainDataLayer.m_PathListDataLayer.m_PathList[pathIndex] = temp;
		}

		if (pathIndex == 0)
		{
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}

		ImGui::SameLine();

		// ------------------------ BUTTON FOR MOVING PATH DOWN ----------------------
		if (pathIndex >= pathList.size() - 1)
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}

		if (ImGui::Button("-", buttonSize))
		{
			auto temp = pathList[pathIndex + 1];
			pathList[pathIndex + 1] = pathList[pathIndex];
			pathList[pathIndex] = temp;
		}

		if (pathIndex >= pathList.size() - 1)
		{
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}

		ImGui::SameLine();

		// ------------------------ BUTTON FOR REMOVING PATH -----------------------
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.6f, 0.0f, 0.0f, 1.0f });

		if (ImGui::Button("X", buttonSize))
		{
			pathList.erase(pathList.begin() + pathIndex);
			pathIndex--;
		}

		ImGui::PopStyleColor();
	}
}

void PathListGuiLayerCNC::DrawPathSelectable(std::shared_ptr<PathProgram> toolPath, float selectableWidth)
{
	auto name = toolPath ? std::format("{} ###path_{}", toolPath->m_PathName, toolPath->m_PathName) : "dupa";

	auto& simulationDataLayer = m_MainDataLayer.m_SimulationDataLayer;

	auto selected = (toolPath == simulationDataLayer.GetSelectedPath());

	if (ImGui::Selectable(name.c_str(), &selected, 0, ImVec2(selectableWidth, 0)))
	{
		simulationDataLayer.SetSelectedPath(selected ? toolPath : std::shared_ptr<PathProgram>());
	}
}

void PathListGuiLayerCNC::DrawAddPathButton()
{
	auto max = ImGui::GetWindowContentRegionMax();
	auto min = ImGui::GetWindowContentRegionMin();

	auto cursorPos = ImGui::GetCursorPos();
	auto style = ImGui::GetStyle();

	auto buttonHeight = 20;
	auto buttonHeightWithSpacing = buttonHeight + style.ItemInnerSpacing.y;
	auto fullHeight = buttonHeightWithSpacing;

	if (max.y - fullHeight > cursorPos.y)
	{
		ImGui::SetCursorPos({ min.x, max.y - fullHeight });
	}

	ImGui::Separator();

	if (ImGui::Button("Import path from file", ImVec2(max.x - min.x, buttonHeight)))
	{
		OpenImportPathDialog();
	}
}

void PathListGuiLayerCNC::OpenImportPathDialog()
{
	auto currentPath = std::filesystem::current_path().string();

	auto fileDialog = ImGuiFileDialog::Instance();
	fileDialog->OpenDialog("ImportPathFileDialog", "Import path file", ".*", currentPath.c_str(), 1, nullptr, ImGuiFileDialogFlags_Modal);
}

void PathListGuiLayerCNC::DrawImportPathDialog()
{
	auto fileDialog = ImGuiFileDialog::Instance();

	if (fileDialog->Display("ImportPathFileDialog"))
	{
		if (fileDialog->IsOk())
		{
			auto& scene = m_MainDataLayer.m_SceneDataLayer.m_Scene;
			std::filesystem::path path = fileDialog->GetFilePathName();

			auto millingTool = m_MillingToolFactory->Create(scene, path);

			if (millingTool)
			{
				auto gCodeParser = GCP::GCodeParser();
				auto parsingResult = gCodeParser.LoadFromFile(path);

				if (parsingResult.m_Result == GCP::GCodeParserCode::SUCCESS)
				{
					auto& pathList = m_MainDataLayer.m_PathListDataLayer.m_PathList;

					pathList.emplace_back(
						std::make_shared<PathProgram>(path.filename().string(), parsingResult.m_Program, millingTool)
					);

					m_MainDataLayer.m_SimulationDataLayer.SetSelectedPath(pathList.back());
				}
			}
		}

		fileDialog->Close();
	}
}
