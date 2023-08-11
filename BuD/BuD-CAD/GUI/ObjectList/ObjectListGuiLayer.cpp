#include "ObjectListGuiLayer.h"

#include <imgui.h>

#include <Visitors/Selection/ObjectSelectVisitor.h>
#include <Visitors/Selection/ObjectUnselectVisitor.h>

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

			auto& selectedGroup = m_MainDataLayer.m_SceneDataLayer.m_SelectedGroup;

			for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
			{
				auto& cadObject = (objectPair++)->second;

				auto id = cadObject->Id();
				auto tag = std::format("{}###{}", cadObject->m_Tag, id);
				
				auto selected = selectedGroup.m_SelectedObjects.contains(id);
				
				if (!ImGui::Selectable(tag.c_str(), selected))
				{
					continue;
				}

				auto multiselect = appState.m_MultiselectOn;
				auto newSelected = !selected;

				std::unique_ptr<AbstractVisitor> selectVisitor = std::make_unique<ObjectSelectVisitor>(m_MainDataLayer.m_SceneDataLayer);
				std::unique_ptr<AbstractVisitor> unselectVisitor = std::make_unique<ObjectUnselectVisitor>(m_MainDataLayer.m_SceneDataLayer);

				if (!multiselect)
				{
					while (!selectedGroup.m_SelectedObjects.empty())
					{
						auto& id = *selectedGroup.m_SelectedObjects.begin();
						auto& object = scene.m_ObjectList[id];
						unselectVisitor->Visit(object);
					}

					selectVisitor->Visit(cadObject);

					continue;
				}

				if (newSelected)
				{
					selectVisitor->Visit(cadObject);
				}
				else
				{
					unselectVisitor->Visit(cadObject);
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
