#include "CursorGuiLayer.h"

#include <imgui.h>

CursorGuiLayer::CursorGuiLayer(MainDataLayer& dataLayer)
	: BaseGuiLayer(dataLayer)
{
}

void CursorGuiLayer::DrawGui()
{
	auto& scene = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD;

	if (scene.m_SelectedGroup.m_Objects.size() > 1)
	{
		return;
	}

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
