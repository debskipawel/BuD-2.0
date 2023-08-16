#include "ObjectCreationGuiLayer.h"

#include <imgui.h>

#include <Visitors/PointBased/PointAddedVisitor.h>

ObjectCreationGuiLayer::ObjectCreationGuiLayer(MainDataLayer& dataLayer)
	: BaseGuiLayer(dataLayer)
{
	m_Buttons.emplace_back(ButtonInfo{ "Create torus", [this]() { CreateTorus(); } });
	m_Buttons.emplace_back(ButtonInfo{ "Create point", [this]() { CreatePoint(); } });
}

void ObjectCreationGuiLayer::DrawGui()
{
	if (ImGui::Begin("Object creation panel"))
	{
		auto panelWidth = ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x;

		constexpr float MAX_BUTTON_WIDTH = 150.0f;
		auto buttonsInLine = (int)(ceil(panelWidth / MAX_BUTTON_WIDTH));

		auto& style = ImGui::GetStyle();
		auto spacing = style.ItemSpacing;

		ImVec2 buttonSize = { panelWidth / buttonsInLine - spacing.x / 2, 0.2f * panelWidth / buttonsInLine };

		int i = 0;

		for (auto& buttonInfo : m_Buttons)
		{
			if (ImGui::Button(buttonInfo.m_Label.c_str(), buttonSize))
			{
				buttonInfo.m_OnClick();
			}

			if (++i % buttonsInLine != 0)
			{
				ImGui::SameLine();
			}
		}

		ImGui::End();
	}
}

void ObjectCreationGuiLayer::CreateTorus()
{
	auto& scene = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD;
	auto position = scene.m_MainCursor->GetPosition();

	scene.CreateTorus(position);
}

void ObjectCreationGuiLayer::CreatePoint()
{
	auto& scene = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD;
	auto position = scene.m_MainCursor->GetPosition();

	auto object = scene.CreatePoint(position);

	if (m_MainDataLayer.m_SceneDataLayer.m_ManuallySelected.ValidatedForAddingControlPoint())
	{
		auto first = m_MainDataLayer.m_SceneDataLayer.m_ManuallySelected.First();

		auto objectShared = object.lock();
		auto point = std::dynamic_pointer_cast<Point>(objectShared);

		std::unique_ptr<AbstractVisitor> visitor = std::make_unique<PointAddedVisitor>(m_MainDataLayer, point);
		visitor->Visit(first);
	}
}
