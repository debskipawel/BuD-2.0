#include "ObjectListGuiLayer.h"

#include <imgui.h>

ObjectListGuiLayer::ObjectListGuiLayer(ObjectListViewModel& viewModel)
	: m_ViewModel(viewModel)
{
}

void ObjectListGuiLayer::DrawGui()
{
	if (ImGui::Begin("Object list"))
	{
		auto& objectList = m_ViewModel.m_Scene.m_ObjectList;

		ImGuiListClipper clipper(objectList.size(), ImGui::GetTextLineHeightWithSpacing());

		while (clipper.Step())
		{
			for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
			{
				auto& cadObject = objectList[i];

				auto id = std::format("{} {}", cadObject->m_Tag, i);
				ImGui::Selectable(id.c_str());
			}
		}

		clipper.End();

		ImGui::End();
	}
}
