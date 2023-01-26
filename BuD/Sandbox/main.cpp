#include <BuD.h>
#include <Core/EntryPoint.h>

#include <Objects/PointLight.h>
#include <Renderer/Renderer.h>

#include "FaceMesh.h"

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

		m_Light = BuD::PointLight(m_Scene, { 1.0f, -1.0f, 0.0f }, dxm::Vector3::One);
		m_Face = std::make_shared<FaceMesh>(m_Scene);
	}

	void OnUpdate(float deltaTime) override
	{
	}

	void OnRender() override
	{
		BuD::Renderer::Clear(0.0f, 0.0f, 0.0f, 1.0f);
		BuD::Renderer::Render(m_Scene);
	}

	void OnGuiRender() override
	{
		ImGui::Begin("Controls");

		if (ImGui::CollapsingHeader("Depth map"))
		{
			ImGui::Checkbox("Use depth map", &m_Face->m_DepthMapAbsorptionOn);
			ImGui::SliderFloat("Grow", &m_Face->m_Grow, 0.0f, 0.1f);

			ImGui::DragFloat3("Passing light multiplier", &m_Face->m_PassingExpMultiplier.x, 1.0f, 0.0f, 300.0f);
		}

		if (ImGui::CollapsingHeader("Use textures"))
		{
			ImGui::Checkbox("Transmission map", &m_Face->m_TransmissionMapOn);
			ImGui::Checkbox("Specular map", &m_Face->m_SpecularMapOn);
			ImGui::Checkbox("SSS map", &m_Face->m_SSSMapOn);
			ImGui::Checkbox("Roughness map", &m_Face->m_RoughnessMapOn);
			ImGui::Checkbox("Normal map", &m_Face->m_NormalMapOn);
			ImGui::Checkbox("Micronormal map", &m_Face->m_MicronormalMapOn);
			ImGui::Checkbox("Micronormal mask", &m_Face->m_MicronormalMaskOn);
			ImGui::Checkbox("Diffuse map", &m_Face->m_DiffuseMapOn);
			ImGui::Checkbox("Ambient occlusion map", &m_Face->m_AmbientOcclussionMapOn);
		}

		if (ImGui::CollapsingHeader("Light"))
		{
			auto& state = m_Light.State();
			auto& emissive = m_Light.Emissive();

			ImGui::DragFloat3("Position ##position", &state.Position.x, 0.05f);
			ImGui::Separator();

			ImGui::ColorPicker3("Color", &emissive.Color.x);
		}

		ImGui::End();
	}

	void OnConcreteEvent(BuD::MouseMovedEvent& e) override
	{
		auto camera = m_Scene.ActiveCamera();

		if (m_MoveMouse)
			camera->RotateCamera(0.01 * e.m_OffsetX, 0.01 * e.m_OffsetY);
	}

	void OnConcreteEvent(BuD::MouseScrolledEvent& e) override
	{
		auto camera = m_Scene.ActiveCamera();
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
	BuD::Scene m_Scene;
	BuD::PointLight m_Light;

	std::shared_ptr<FaceMesh> m_Face;

	bool m_MoveMouse = false;
};

std::shared_ptr<BuD::AppLayer> BuD::CreateClientApp()
{
	return std::shared_ptr<BuD::AppLayer>(new SandboxApp());
}
