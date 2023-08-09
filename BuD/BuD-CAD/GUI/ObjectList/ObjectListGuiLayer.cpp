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
			auto objectPair = objectList.begin();

			for (int i = 0; i < clipper.DisplayStart; i++)
			{
				objectPair++;
			}

			for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
			{
				auto& cadObject = objectPair->second;

				auto id = std::format("{}, id: {}", cadObject->m_Tag, i);
				
				auto selected = cadObject->m_Selected;

				ImGui::Selectable(id.c_str(), &cadObject->m_Selected);

				if (selected != cadObject->m_Selected)
				{
					auto& composite = m_ViewModel.m_Scene.m_SelectedGroup;

					cadObject->m_Selected ? composite.Add(cadObject) : composite.Remove(cadObject->Id());
				}
			}
		}

		clipper.End();

		ImGui::End();
	}
}
