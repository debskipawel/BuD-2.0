#include "RobotParametersGuiLayer.h"

#include <ranges>

#include <imgui.h>

RobotParametersGuiLayer::RobotParametersGuiLayer(MainDataLayer& mainDataLayer)
	: BaseGuiLayer(mainDataLayer)
{
	m_Texture = BuD::Texture::LoadFromFile("Resources/Textures/malysz.jpg");
}

auto RobotParametersGuiLayer::DrawGui() -> void
{
	if (ImGui::Begin("Parameters"))
	{
		DrawStartPointOptionsComboBox();
		DrawEndPointOptionsComboBox();

		DrawParameterSpaceTexture();

		ImGui::End();
	}
}

auto RobotParametersGuiLayer::DrawStartPointOptionsComboBox() -> void
{
	auto& sceneDataLayer = m_MainDataLayer.m_SceneDataLayer;
	auto& startPointOptions = sceneDataLayer.m_StartConfiguration.m_PointOptions;

	if (startPointOptions.size() <= 1)
	{
		return;
	}

	auto previewValue = std::format("{}", sceneDataLayer.m_StartConfiguration.m_PointOptionIndex + 1);

	ImGui::Text("Start option");

	if (ImGui::BeginCombo("###start_combo_box", previewValue.c_str()))
	{
		for (int i = 0; i < startPointOptions.size(); ++i)
		{
			auto label = std::format("{}", i + 1);
			auto selected = (sceneDataLayer.m_StartConfiguration.m_PointOptionIndex == i);

			if (ImGui::Selectable(label.c_str(), &selected))
			{
				sceneDataLayer.m_StartConfiguration.m_PointOptionIndex = i;
				sceneDataLayer.UpdateMeshes();
			}
		}

		ImGui::EndCombo();
	}

	ImGui::Separator();
}

auto RobotParametersGuiLayer::DrawEndPointOptionsComboBox() -> void
{
	auto& sceneDataLayer = m_MainDataLayer.m_SceneDataLayer;
	auto& endPointOptions = sceneDataLayer.m_EndConfiguration.m_PointOptions;

	if (endPointOptions.size() <= 1)
	{
		return;
	}

	auto previewValue = std::format("{}", sceneDataLayer.m_EndConfiguration.m_PointOptionIndex + 1);

	ImGui::Text("End option");

	if (ImGui::BeginCombo("###end_combo_box", previewValue.c_str()))
	{
		for (int i = 0; i < endPointOptions.size(); ++i)
		{
			auto label = std::format("{}", i + 1);
			auto selected = (sceneDataLayer.m_EndConfiguration.m_PointOptionIndex == i);

			if (ImGui::Selectable(label.c_str(), &selected))
			{
				sceneDataLayer.m_EndConfiguration.m_PointOptionIndex = i;
				sceneDataLayer.UpdateMeshes();
			}
		}

		ImGui::EndCombo();
	}

	ImGui::Separator();
}

auto RobotParametersGuiLayer::DrawParameterSpaceTexture() -> void
{
	auto& style = ImGui::GetStyle();

	auto max = ImGui::GetWindowContentRegionMax();
	auto min = ImGui::GetWindowContentRegionMin();

	auto width = max.x - min.x;
	auto imageSize = ImVec2(width, width);

	auto newCursorY = max.y - imageSize.y - 2 * style.ItemInnerSpacing.y;

	if (newCursorY >= ImGui::GetCursorPosY())
	{
		ImGui::SetCursorPosY(newCursorY);
	}

	ImGui::Separator();
	ImGui::Image(m_Texture.SRV(), imageSize);
}
