#include "ViewportGuiLayer.h"

#include <imgui.h>
#include <imgui_internal.h>

ViewportGuiLayer::ViewportGuiLayer(MainDataLayer& dataLayer)
	: BaseGuiLayer(dataLayer)
{
}

void ViewportGuiLayer::DrawGui()
{
	DrawViewport(m_MainDataLayer.m_ViewportDataLayer, "Viewport");
}

auto ViewportGuiLayer::DrawViewport(ViewportDataLayer& viewport, const std::string& viewportName) -> void
{
	auto label = std::format("{}", viewportName, viewportName);

	if (ImGui::Begin(label.c_str()))
	{
		auto windowPosition = ImGui::GetWindowPos();
		viewport.m_ViewportPosition = { windowPosition.x, windowPosition.y };

		ImVec2 vMin = ImGui::GetWindowContentRegionMin();
		ImVec2 vMax = ImGui::GetWindowContentRegionMax();
		vMin.x += windowPosition.x;
		vMin.y += windowPosition.y;
		vMax.x += windowPosition.x;
		vMax.y += windowPosition.y;

		viewport.m_ViewportWidth = vMax.x - vMin.x;
		viewport.m_ViewportHeight = vMax.y - vMin.y;

		DrawSceneElements();

		ImGui::End();
	}
}

auto ViewportGuiLayer::DrawSceneElements() -> void
{
	auto R = m_MainDataLayer.m_SimulationDataLayer.m_Radius;
	auto L = m_MainDataLayer.m_SimulationDataLayer.m_ArmLength;

	auto width = 2.0f * (R + L);

	auto circlePosition = dxm::Vector2(-0.5f * sqrtf(L * L - R * R), 0.0f);
	
	auto blockSize = dxm::Vector2(1.0f, 1.0f);
	auto blockPosition = dxm::Vector2(0.5f * sqrtf(L * L - R * R), 0.0f);

	if (m_MainDataLayer.m_SimulationDataLayer.IsRunning() && m_MainDataLayer.m_SimulationDataLayer.m_TimeValues.size() > 0)
	{
		blockPosition.x += m_MainDataLayer.m_SimulationDataLayer.m_PositionValues.back();
	}
	else
	{
		blockPosition.x = circlePosition.x + R + L;
	}

	auto alpha = m_MainDataLayer.m_SimulationDataLayer.Alpha();
	auto armPosition = dxm::Vector2(circlePosition.x + cosf(alpha) * R, circlePosition.y + sinf(alpha) * R);

	auto scaledRadius = R / width;

	auto screenSpaceBlockSize = blockSize / width;

	auto screenSpaceCirclePosition = circlePosition / width + dxm::Vector2(0.5f, 0.5f);
	auto screenSpaceBlockPosition = blockPosition / width + dxm::Vector2(0.5f, 0.5f);

	const auto& viewport = m_MainDataLayer.m_ViewportDataLayer;

	auto pixelRadius = scaledRadius * viewport.m_ViewportWidth;

	auto pixelSpaceBlockSize = screenSpaceBlockSize * viewport.m_ViewportWidth;

	auto pixelSpaceCirclePosition = viewport.m_ViewportPosition + screenSpaceCirclePosition * dxm::Vector2(viewport.m_ViewportWidth, viewport.m_ViewportHeight);
	auto pixelSpaceBlockPosition = viewport.m_ViewportPosition + screenSpaceBlockPosition * dxm::Vector2(viewport.m_ViewportWidth, viewport.m_ViewportHeight);
	auto pixelSpaceArmPosition = pixelSpaceCirclePosition + pixelRadius * dxm::Vector2(cosf(alpha), -sinf(alpha));

	auto imguiContext = ImGui::GetCurrentContext();

	auto& imguiDrawList = imguiContext->CurrentWindow->DrawList;

	imguiDrawList->AddCircle({ pixelSpaceCirclePosition.x, pixelSpaceCirclePosition.y }, pixelRadius, ImGui::GetColorU32({ 1.0f, 1.0f, 1.0f, 1.0f }));
	
	imguiDrawList->AddRectFilled(
		{ pixelSpaceBlockPosition.x - 0.5f * pixelSpaceBlockSize.x, pixelSpaceBlockPosition.y - 0.5f * pixelSpaceBlockSize.y },
		{ pixelSpaceBlockPosition.x + 0.5f * pixelSpaceBlockSize.x, pixelSpaceBlockPosition.y + 0.5f * pixelSpaceBlockSize.y },
		ImGui::GetColorU32({ 0.0f, 1.0f, 1.0f, 1.0f })
	);
	
	imguiDrawList->AddLine(
		{ pixelSpaceCirclePosition.x, pixelSpaceCirclePosition.y }, 
		{ pixelSpaceArmPosition.x, pixelSpaceArmPosition.y }, 
		ImGui::GetColorU32({ 0.8f, 0.0f, 0.0f, 1.0f }), 3.0f
	);

	imguiDrawList->AddLine(
		{ pixelSpaceArmPosition.x, pixelSpaceArmPosition.y },
		{ pixelSpaceBlockPosition.x, pixelSpaceBlockPosition.y },
		ImGui::GetColorU32({ 0.7f, 0.7f, 0.0f, 1.0f }), 3.0f
	);
}
