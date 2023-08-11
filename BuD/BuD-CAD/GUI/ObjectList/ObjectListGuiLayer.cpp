#include "ObjectListGuiLayer.h"

#include <imgui.h>

ObjectListGuiLayer::ObjectListGuiLayer(MainDataLayer& dataLayer)
	: BaseGuiLayer(dataLayer)
{
}

void ObjectListGuiLayer::DrawGui()
{
	if (ImGui::Begin("Object list"))
	{
		DrawGuiForFilters();

		auto& appState = m_MainDataLayer.m_AppStateDataLayer;

		auto& scene = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD;
		auto& objectList = scene.m_ObjectList;

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

				auto id = std::format("{}###{}", cadObject->m_Tag, cadObject->Id());
				
				auto selected = cadObject->m_Selected;
				
				if (!ImGui::Selectable(id.c_str(), selected))
				{
					continue;
				}

				auto& composite = scene.m_SelectedGroup;
				
				auto multiselect = appState.m_MultiselectOn;
				auto newSelected = !selected;

				if (!multiselect)
				{
					composite.Clear();
					composite.Add(cadObject);

					continue;
				}

				if (newSelected)
				{
					composite.Add(cadObject);
				}
				else
				{
					composite.Remove(cadObject->Id());
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

	auto& appState = m_MainDataLayer.m_AppStateDataLayer;

	if (ImGui::Button("Open filters menu", filtersButtonSize))
	{
		appState.Freeze();
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
			appState.Unfreeze();
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}
