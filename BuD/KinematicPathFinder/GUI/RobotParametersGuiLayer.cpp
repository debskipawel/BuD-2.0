#include "RobotParametersGuiLayer.h"

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
		DrawParameterSpaceTexture();

		ImGui::End();
	}
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
