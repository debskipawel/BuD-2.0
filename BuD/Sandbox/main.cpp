#include <BuD.h>
#include <Core/EntryPoint.h>

#include <Objects/PointLight.h>
#include <Renderer/Renderer.h>

#include "DepthMapFaceMesh.h"
#include "MergedFaceMesh.h"

#include "imgui.h"

class SandboxApp : public BuD::AppLayer
{
public:
	SandboxApp()
		: m_Scene()
	{
		BuD::Log::RegisterLogHandle(
			[](const BuD::Log::LogRecord& record)
			{
				OutputDebugStringW(record.message.c_str());
			}
		);

		m_Light[0] = BuD::PointLight(m_Scene[0], { 1.0f, -1.0f, 0.0f }, dxm::Vector3::One);
		m_Light[1] = BuD::PointLight(m_Scene[1], { 1.0f, -1.0f, 0.0f }, dxm::Vector3::One);
		
		m_Faces[0] = std::make_shared<DepthMapFaceMesh>(m_Scene[0]);
		m_Faces[1] = std::make_shared<MergedFaceMesh>(m_Scene[1]);
	}

	void OnUpdate(float deltaTime) override
	{
	}

	void OnRender() override
	{
		BuD::Renderer::BeginTarget(m_ViewportSize.x, m_ViewportSize.y);
		BuD::Renderer::Clear(0.0f, 0.0f, 0.0f, 1.0f);
		BuD::Renderer::Render(m_Scene[m_ActiveScene]);

		m_Viewport = BuD::Renderer::EndTarget();
	}

	void OnGuiRender() override
	{
		if (ImGui::Begin("Viewport"))
		{
			ImVec2 vMin = ImGui::GetWindowContentRegionMin();
			ImVec2 vMax = ImGui::GetWindowContentRegionMax();
			vMin.x += ImGui::GetWindowPos().x;
			vMin.y += ImGui::GetWindowPos().y;
			vMax.x += ImGui::GetWindowPos().x;
			vMax.y += ImGui::GetWindowPos().y;

			m_ViewportSize = { vMax.x - vMin.x, vMax.y - vMin.y };

			ImGui::Image(m_Viewport.SRV(), m_ViewportSize);

			ImGui::End();
		}

		ImGui::Begin("Controls");

		if (auto face = dynamic_cast<DepthMapFaceMesh*>(m_Faces[m_ActiveScene].get()))
		{
			if (ImGui::CollapsingHeader("Depth map"))
			{
				ImGui::Checkbox("Use depth map", &face->m_DepthMapAbsorptionOn);
				ImGui::SliderFloat("Grow", &face->m_Grow, 0.0f, 0.1f);

				ImGui::DragFloat3("Passing light multiplier", &face->m_PassingExpMultiplier.x, 1.0f, 0.0f, 300.0f);

				ImGui::Separator();

				ImVec2 vMin = ImGui::GetWindowContentRegionMin();
				ImVec2 vMax = ImGui::GetWindowContentRegionMax();
				auto width = vMax.x - vMin.x;

				ImGui::Image(face->m_LightDepthMap.SRV(), { width, width });
			}

			if (ImGui::CollapsingHeader("Light"))
			{
				auto& state = m_Light[0].State();
				auto& emissive = m_Light[0].Emissive();

				ImGui::DragFloat3("Position ##position", &state.Position.x, 0.05f);
				ImGui::Separator();

				ImGui::ColorPicker3("Color", &emissive.Color.x);
			}
		}

		if (auto face = dynamic_cast<MergedFaceMesh*>(m_Faces[m_ActiveScene].get()))
		{
			if (ImGui::CollapsingHeader("Depth map"))
			{
				ImGui::Checkbox("Use depth map", &face->m_DepthMapAbsorptionOn);
				ImGui::SliderFloat("Grow", &face->m_Grow, 0.0f, 0.1f);

				ImGui::DragFloat3("Passing light multiplier", &face->m_PassingExpMultiplier.x, 1.0f, 0.0f, 300.0f);

				ImGui::Separator();

				ImVec2 vMin = ImGui::GetWindowContentRegionMin();
				ImVec2 vMax = ImGui::GetWindowContentRegionMax();
				auto width = vMax.x - vMin.x;

				ImGui::Image(face->m_LightDepthMap.SRV(), { width, width });
			}

			if (ImGui::CollapsingHeader("Blurring passes"))
			{
				if (ImGui::TreeNode("1. Baking irradiance"))
				{
					ImVec2 vMin = ImGui::GetWindowContentRegionMin();
					ImVec2 vMax = ImGui::GetWindowContentRegionMax();
					auto width = vMax.x - vMin.x - 20;

					ImGui::Image(face->m_BakedLighting.SRV(), {width, width});
					ImGui::TreePop();
				}

				if (ImGui::TreeNode("2. 12-tap Gaussian blur"))
				{
					ImVec2 vMin = ImGui::GetWindowContentRegionMin();
					ImVec2 vMax = ImGui::GetWindowContentRegionMax();
					auto width = vMax.x - vMin.x - 20;

					ImGui::Image(face->m_BlurredBakedIrradiance.SRV(), { width, width });
					ImGui::TreePop();
				}
			}

			// TODO: gui for gaussian blur
			if (ImGui::CollapsingHeader("Light"))
			{
				auto& state = m_Light[1].State();
				auto& emissive = m_Light[1].Emissive();

				ImGui::DragFloat3("Position ##position", &state.Position.x, 0.05f);
				ImGui::Separator();

				ImGui::ColorPicker3("Color", &emissive.Color.x);
			}
		}

		ImGui::End();
	}

	void OnConcreteEvent(BuD::MouseMovedEvent& e) override
	{
		auto camera = m_Scene[m_ActiveScene].ActiveCamera();

		if (m_MoveMouse)
			camera->RotateCamera(0.01 * e.m_OffsetX, 0.01 * e.m_OffsetY);
	}

	void OnConcreteEvent(BuD::MouseScrolledEvent& e) override
	{
		auto camera = m_Scene[m_ActiveScene].ActiveCamera();
		camera->Zoom(-0.001f * e.m_WheelDelta);
	}

	void OnConcreteEvent(BuD::MouseButtonDownEvent& e) override
	{
		if (e.m_Button == BuD::MouseCode::MIDDLE)
		{
			m_MoveMouse = true;
		}
	}

	void OnConcreteEvent(BuD::MouseButtonReleasedEvent& e) override
	{
		if (e.m_Button == BuD::MouseCode::MIDDLE)
		{
			m_MoveMouse = false;
		}
	}

protected:
	BuD::Texture m_Viewport;
	BuD::Texture m_Map;
	ImVec2 m_ViewportSize = { 0, 0 };

	int m_ActiveScene = 1;
	BuD::Scene m_Scene[2];
	BuD::PointLight m_Light[2];
	std::shared_ptr<FaceMesh> m_Faces[2];

	bool m_MoveMouse = false;
};

std::shared_ptr<BuD::AppLayer> BuD::CreateClientApp()
{
	return std::shared_ptr<BuD::AppLayer>(new SandboxApp());
}
