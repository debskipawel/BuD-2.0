#include <BuD.h>
#include <Core/EntryPoint.h>

#include <Objects/MeshLoader/MeshLoader.h>
#include <Renderer/Renderer.h>

#include "FaceMesh.h"

#include "imgui.h"

class SandboxApp : public BuD::AppLayer
{
public:
	SandboxApp()
		: m_Scene()
	{
		auto face = std::make_shared<FaceMesh>(m_Scene);
	}

	void OnUpdate(float deltaTime) override
	{
	}

	void OnRender() override
	{
		BuD::Renderer::BeginTarget(500, 300);
		BuD::Renderer::Clear(0.1f, 0.0f, 0.2f, 1.0f);
		BuD::Renderer::Render(m_Scene);
		m_Viewport = BuD::Renderer::EndTarget();
	}

	void OnGuiRender() override
	{
		ImGui::Begin("Viewport");
		ImGui::Image(m_Viewport.SRV(), { 500, 300 });
		ImGui::End();
	}

	void OnConcreteEvent(BuD::MouseMovedEvent& e) override
	{
		auto camera = m_Scene.ActiveCamera();

		camera->RotateCamera(0.01 * e.m_OffsetX, 0.01 * e.m_OffsetY);
	}

protected:
	BuD::Scene m_Scene;
	BuD::Texture m_Viewport;
};

std::shared_ptr<BuD::AppLayer> BuD::CreateClientApp()
{
	return std::shared_ptr<BuD::AppLayer>(new SandboxApp());
}
