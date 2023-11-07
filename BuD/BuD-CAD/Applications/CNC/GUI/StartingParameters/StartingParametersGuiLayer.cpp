#include "StartingParametersGuiLayer.h"

#include <imgui.h>

StartingParametersGuiLayer::StartingParametersGuiLayer(MainDataLayerCNC& dataLayer)
	: BaseGuiLayerCNC(dataLayer), m_ResolutionSelectedIndex(2), m_BlockParameters(MaterialBlockParameters::DEFAULT_PARAMETERS)
{
	m_AvailableResolutions = { 256, 512, 1024, 2048 };
}

void StartingParametersGuiLayer::DrawGui()
{
	if (m_MainDataLayer.m_SimulationDataLayer.Running())
	{
		return;
	}

	if (ImGui::Begin("Starting parameters ###starting_parameters"))
	{
		DrawResolutionComboBox();
		DrawSizeControls();

		DrawResetSizeButton();

		ImGui::End();
	}
}

void StartingParametersGuiLayer::DrawResolutionComboBox()
{
	auto selectedResolution = m_AvailableResolutions[m_ResolutionSelectedIndex];
	auto comboBoxPreviewValue = std::format("{} x {}", selectedResolution, selectedResolution);

	ImGui::Text("Mesh resolution", "");

	if (ImGui::BeginCombo("###cnc_resolution", comboBoxPreviewValue.c_str()))
	{
		for (auto i = 0; i < m_AvailableResolutions.size(); i++)
		{
			const auto& resolution = m_AvailableResolutions[i];

			auto optionLabel = std::format("{} x {}", resolution, resolution);
			auto selected = (resolution == selectedResolution);

			if (ImGui::Selectable(optionLabel.c_str(), &selected) && m_ResolutionSelectedIndex != i)
			{
				m_ResolutionSelectedIndex = i;
			}
		}

		ImGui::EndCombo();
	}
}

void StartingParametersGuiLayer::DrawSizeControls()
{
	ImGui::Text("Material size");
	ImGui::DragFloat3("###material_block_size", reinterpret_cast<float*>(&m_BlockParameters.m_Size), 0.1f, 1.0f, 30.0f);

	ImGui::Separator();

	ImGui::Text("Max tool immersion");
	ImGui::DragFloat("###material_block_tool_immersion", &m_BlockParameters.m_MaxImmersion, 0.1f, 0.0f, m_BlockParameters.m_Size.y, "%.3f", ImGuiSliderFlags_AlwaysClamp);
}

void StartingParametersGuiLayer::DrawResetSizeButton()
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

	if (ImGui::Button("Reset material", ImVec2(max.x - min.x, buttonHeight)))
	{
		auto resolution = m_AvailableResolutions[m_ResolutionSelectedIndex];

		m_BlockParameters.m_ResolutionWidth = resolution;
		m_BlockParameters.m_ResolutionHeight = resolution;

		m_MainDataLayer.m_SimulationDataLayer.ResetMaterial(m_BlockParameters);
	}
}
