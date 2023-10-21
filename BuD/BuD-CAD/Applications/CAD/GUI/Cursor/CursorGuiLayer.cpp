#include "CursorGuiLayer.h"

#include <imgui.h>

CursorGuiLayer::CursorGuiLayer(MainDataLayerCAD& dataLayer)
	: BaseGuiLayerCAD(dataLayer)
{
}

void CursorGuiLayer::DrawGui()
{
	auto& scene = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD;

	auto& cursor = scene.m_MainCursor;

	if (ImGui::Begin("Cursor ###cursor_panel"))
	{
		auto position = cursor->GetPosition();

		if (ImGui::DragFloat3("Position ###cursor_position", (float*)&position, 0.1f))
		{
			cursor->SetPosition(position);
		}

		ImGui::End();
	}
}
