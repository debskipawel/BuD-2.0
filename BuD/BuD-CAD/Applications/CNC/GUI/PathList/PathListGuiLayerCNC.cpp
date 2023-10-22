#include "PathListGuiLayerCNC.h"

#include <GCodeParser.h>

#include <filesystem>
#include <format>
#include <imgui.h>
#include <ImGuiFileDialog.h>

PathListGuiLayerCNC::PathListGuiLayerCNC(MainDataLayerCNC& dataLayer)
	: BaseGuiLayerCNC(dataLayer)
{
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
	auto pathIndex = 0;

	for (const auto& path : m_MainDataLayer.m_PathListDataLayer.m_PathList)
	{
		auto name = path ? std::format("{} ###path_{}", path->m_PathName, pathIndex) : "dupa";

		auto buttonSize = ImVec2(20.0f, 20.0f);
		auto panelWidth = ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x;
		auto padding = ImGui::GetStyle().ItemSpacing.x;

		auto selectableWidth = std::max(50.0f, panelWidth - padding - 3 * (buttonSize.x + padding));

		bool selected = false;
		ImGui::Selectable(name.c_str(), &selected, 0, ImVec2(selectableWidth, 0));
		ImGui::SameLine(selectableWidth + padding, padding);

		if (ImGui::SmallButton("up"))
		{
			// TODO: swap this and previous path
		}

		ImGui::SameLine();

		if (ImGui::SmallButton("down"))
		{
			// TODO: swap this and next path
		}

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, { 0.6f, 0.0f, 0.0f, 1.0f });

		if (ImGui::SmallButton("X"))
		{
			// TODO: remove path from the list
		}

		ImGui::PopStyleColor();

		pathIndex++;
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
			std::filesystem::path path = fileDialog->GetFilePathName();

			auto gCodeParser = GCP::GCodeParser();
			auto parsingResult = gCodeParser.LoadFromFile(path);

			if (parsingResult.m_Result == GCP::GCodeParserCode::SUCCESS)
			{
				auto& pathList = m_MainDataLayer.m_PathListDataLayer.m_PathList;

				pathList.emplace_back(
					std::make_shared<ToolPath>(path.filename().string(), parsingResult.m_Program)
				);
			}
		}

		fileDialog->Close();
	}
}
