#include "CursorGuiLayer.h"

#include <imgui.h>

CursorGuiLayer::CursorGuiLayer(CursorViewModel& cursor)
	: m_CursorViewModel(cursor)
{
}

void CursorGuiLayer::DrawGui()
{
	auto& cursor = m_CursorViewModel.m_Cursor;

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
