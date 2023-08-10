#include "ObjectListGuiLayer.h"

#include <imgui.h>

ObjectListGuiLayer::ObjectListGuiLayer(ObjectListViewModel& objectList, AppStateViewModel& appState)
	: m_ObjectListViewModel(objectList), m_AppStateViewModel(appState)
{
}

void ObjectListGuiLayer::DrawGui()
{
	if (ImGui::Begin("Object list"))
	{
		DrawGuiForFilters();

		auto& objectList = m_ObjectListViewModel.m_Scene.m_ObjectList;

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
				auto& cadObject = (objectPair++)->second;

				auto id = std::format("{} ({}) ###{}", cadObject->m_Tag, cadObject->Id(), cadObject->Id());
				
				auto selected = cadObject->m_Selected;

				ImGui::Selectable(id.c_str(), &cadObject->m_Selected);

				if (selected != cadObject->m_Selected)
				{
					auto& composite = m_ObjectListViewModel.m_Scene.m_SelectedGroup;

					cadObject->m_Selected ? composite.Add(cadObject) : composite.Remove(cadObject->Id());
				}
			}
		}

		clipper.End();

		ImGui::End();
	}
}

void ObjectListGuiLayer::DrawGuiForFilters()
{
	auto objectListMin = ImGui::GetWindowContentRegionMin();
	auto objectListMax = ImGui::GetWindowContentRegionMax();

	ImVec2 filtersButtonSize = { objectListMax.x - objectListMin.x, 0 };

	if (ImGui::Button("Open filters menu", filtersButtonSize))
	{
		m_AppStateViewModel.Freeze();
		ImGui::OpenPopup("###filters_popup");
	}

	ImGui::Separator();

	if (ImGui::BeginPopupModal("Filters ###filters_popup"))
	{
		ImGui::Text("TODO: No filters implemented yet.");

		auto windowPosition = ImGui::GetWindowPos();
		auto min = ImGui::GetWindowContentRegionMin();
		auto max = ImGui::GetWindowContentRegionMax();

		ImVec2 buttonSize = { max.x - min.x, 20 };
		ImVec2 position = { min.x, max.y - buttonSize.y };

		ImGui::SetCursorPos(position);
		
		if (ImGui::Button("Close filters menu", buttonSize))
		{
			m_AppStateViewModel.Unfreeze();
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}
