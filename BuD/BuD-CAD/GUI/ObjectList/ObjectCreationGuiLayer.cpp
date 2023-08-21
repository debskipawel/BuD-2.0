#include "ObjectCreationGuiLayer.h"

#include <imgui.h>

#include <Visitors/PointBased/PointAddedVisitor.h>

ObjectCreationGuiLayer::ObjectCreationGuiLayer(MainDataLayer& dataLayer)
	: BaseGuiLayer(dataLayer), m_OpenPopupForSurfaceC0(), m_OpenPopupForSurfaceC2()
{
	m_Buttons.emplace_back(ButtonInfo{ "Create torus", [this]() { CreateTorus(); } });
	m_Buttons.emplace_back(ButtonInfo{ "Create point", [this]() { CreatePoint(); } });
	m_Buttons.emplace_back(ButtonInfo{ "Create surface C0", [this]() { OpenPopupForSurfaceCreationC0(); } });
	m_Buttons.emplace_back(ButtonInfo{ "Create surface C2", [this]() { OpenPopupForSurfaceCreationC2(); } });
}

void ObjectCreationGuiLayer::DrawGui()
{
	if (ImGui::Begin("Object creation panel"))
	{
		auto panelWidth = ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x;

		constexpr float MAX_BUTTON_WIDTH = 200.0f;
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

		DrawGuiForSurfaceCreationC0();
		DrawGuiForSurfaceCreationC2();

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

		std::unique_ptr<AbstractVisitor> visitor = std::make_unique<PointAddedVisitor>(m_MainDataLayer.m_SceneDataLayer, point);
		visitor->Visit(first);
	}
}

void ObjectCreationGuiLayer::OpenPopupForSurfaceCreationC0()
{
	m_MainDataLayer.m_AppStateDataLayer.Freeze();
	
	auto cursorPosition = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.m_MainCursor->GetPosition();
	m_SurfaceParametersC0 = SurfaceCreationParameters(cursorPosition);

	m_OpenPopupForSurfaceC0 = true;
}

void ObjectCreationGuiLayer::OpenPopupForSurfaceCreationC2()
{
	m_MainDataLayer.m_AppStateDataLayer.Freeze();

	auto cursorPosition = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.m_MainCursor->GetPosition();
	m_SurfaceParametersC2 = SurfaceCreationParameters(cursorPosition);

	m_OpenPopupForSurfaceC2 = true;
}

void ObjectCreationGuiLayer::DrawGuiForSurfaceCreationC0()
{
	if (m_OpenPopupForSurfaceC0)
	{
		ImGui::OpenPopup("Create C0 surface ###surface_c0_creation_panel");
	}

	if (ImGui::BeginPopupModal("Create C0 surface ###surface_c0_creation_panel", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize))
	{
		ImGui::DragFloat3("Position", (float*)&m_SurfaceParametersC0.m_Position, 0.1f);
		ImGui::DragInt("U patches", (int*)&m_SurfaceParametersC0.m_PatchesU, 1.0f, 1, 100, "%d", ImGuiSliderFlags_AlwaysClamp);
		ImGui::DragInt("V patches", (int*)&m_SurfaceParametersC0.m_PatchesV, 1.0f, 1, 100, "%d", ImGuiSliderFlags_AlwaysClamp);
		ImGui::Checkbox("Cylinder", &m_SurfaceParametersC0.m_Cylinder);

		auto max = ImGui::GetWindowContentRegionMax();
		auto min = ImGui::GetWindowContentRegionMin();

		auto& style = ImGui::GetStyle();
		auto buttonWidth = 0.5f * (max.x - min.x - style.ItemInnerSpacing.x * 3);

		ImVec2 size = { buttonWidth, 0 };

		if (ImGui::Button("OK", size))
		{
			if (m_SurfaceParametersC0.m_Cylinder)
			{
				m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.CreateCylinderBezierSurfaceC0(m_SurfaceParametersC0.m_Position, m_SurfaceParametersC0.m_PatchesU, m_SurfaceParametersC0.m_PatchesV);
			}
			else
			{
				m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.CreateFlatBezierSurfaceC0(m_SurfaceParametersC0.m_Position, m_SurfaceParametersC0.m_PatchesU, m_SurfaceParametersC0.m_PatchesV);

			}

			m_OpenPopupForSurfaceC0 = false;
			m_MainDataLayer.m_AppStateDataLayer.Unfreeze();
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel", size))
		{
			m_OpenPopupForSurfaceC0 = false;
			m_MainDataLayer.m_AppStateDataLayer.Unfreeze();
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void ObjectCreationGuiLayer::DrawGuiForSurfaceCreationC2()
{
	if (m_OpenPopupForSurfaceC2)
	{
		ImGui::OpenPopup("Create C2 surface ###surface_c2_creation_panel");
	}

	if (ImGui::BeginPopupModal("Create C2 surface ###surface_c2_creation_panel", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize))
	{
		ImGui::DragFloat3("Position", (float*)&m_SurfaceParametersC2.m_Position, 0.1f);
		ImGui::DragInt("U patches", (int*)&m_SurfaceParametersC2.m_PatchesU, 0.2f, 1, 50, "%d", ImGuiSliderFlags_AlwaysClamp);
		ImGui::DragInt("V patches", (int*)&m_SurfaceParametersC2.m_PatchesV, 0.2f, 1, 50, "%d", ImGuiSliderFlags_AlwaysClamp);
		ImGui::Checkbox("Cylinder", &m_SurfaceParametersC2.m_Cylinder);

		auto max = ImGui::GetWindowContentRegionMax();
		auto min = ImGui::GetWindowContentRegionMin();

		auto& style = ImGui::GetStyle();
		auto buttonWidth = 0.5f * (max.x - min.x - style.ItemInnerSpacing.x * 3);

		ImVec2 size = { buttonWidth, 0 };

		if (ImGui::Button("OK", size))
		{
			if (m_SurfaceParametersC2.m_Cylinder)
			{
				m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.CreateCylinderBezierSurfaceC2(m_SurfaceParametersC2.m_Position, m_SurfaceParametersC2.m_PatchesU, m_SurfaceParametersC2.m_PatchesV);
			}
			else
			{
				m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.CreateFlatBezierSurfaceC2(m_SurfaceParametersC2.m_Position, m_SurfaceParametersC2.m_PatchesU, m_SurfaceParametersC2.m_PatchesV);

			}

			m_OpenPopupForSurfaceC2 = false;
			m_MainDataLayer.m_AppStateDataLayer.Unfreeze();
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel", size))
		{
			m_OpenPopupForSurfaceC2 = false;
			m_MainDataLayer.m_AppStateDataLayer.Unfreeze();
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

ObjectCreationGuiLayer::SurfaceCreationParameters::SurfaceCreationParameters()
	: SurfaceCreationParameters(dxm::Vector3::Zero)
{
}

ObjectCreationGuiLayer::SurfaceCreationParameters::SurfaceCreationParameters(dxm::Vector3 position)
	: m_Position(position), m_PatchesU(1), m_PatchesV(1), m_Cylinder(false)
{
}
