#include "ObjectListGuiLayer.h"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include <Applications/CAD/Visitors/Selection/ObjectSelectVisitor.h>
#include <Applications/CAD/Visitors/Selection/ObjectUnselectVisitor.h>
#include <Applications/CAD/Visitors/Validation/FilterValidationVisitor.h>

ObjectListGuiLayer::ObjectListGuiLayer(MainDataLayer& dataLayer)
	: BaseGuiLayerCAD(dataLayer)
{
}

void ObjectListGuiLayer::DrawGui()
{
	if (ImGui::Begin("Object list"))
	{
		DrawGuiForFilters();

		if (m_Filter != FilterInfo::DEFAULT_FILTER)
		{
			DrawListFiltered();
		}
		else
		{
			DrawListClipped();
		}

		ImGui::End();
	}
}

void ObjectListGuiLayer::DrawListFiltered()
{
	auto& appState = m_MainDataLayer.m_AppStateDataLayer;

	auto& scene = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD;
	auto& objectList = scene.m_ObjectList;

	auto& manuallySelected = m_MainDataLayer.m_SceneDataLayer.m_ManuallySelected;

	std::unique_ptr<BaseValidationVisitor> visitor = std::make_unique<FilterValidationVisitor>(m_Filter);

	for (auto& [id, cadObject] : objectList)
	{
		visitor->Visit(cadObject);

		if (!visitor->Valid())
		{
			continue;
		}

		auto id = cadObject->Id();
		auto tag = std::format("{}###{}", cadObject->m_Tag, id);

		auto selected = manuallySelected.Selected(id);

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
			while (manuallySelected.Count() > 0)
			{
				auto object = manuallySelected.First();
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

void ObjectListGuiLayer::DrawListClipped()
{
	auto& appState = m_MainDataLayer.m_AppStateDataLayer;

	auto& scene = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD;
	auto& objectList = scene.m_ObjectList;

	auto& manuallySelected = m_MainDataLayer.m_SceneDataLayer.m_ManuallySelected;

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

			auto id = cadObject->Id();
			auto tag = std::format("{}###{}", cadObject->m_Tag, id);

			auto selected = manuallySelected.Selected(id);

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
				while (manuallySelected.Count() > 0)
				{
					auto object = manuallySelected.First();
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
		ImGui::Text("Filter by tag", "");
		ImGui::InputText("###tag_filter", &m_Filter.m_TagFilter);

		ImGui::Separator();

		ImGui::Checkbox("Skip points ###points_filter", &m_Filter.m_SkipPoints);

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
