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
		m_Scene.m_SceneEntities.insert(std::make_pair(face->Id(), face));
	}

	void OnUpdate(float deltaTime) override
	{
	}

	void OnRender() override
	{
		BuD::Renderer::Render(m_Scene);
	}

	void OnGuiRender() override
	{
	}

	void OnConcreteEvent(BuD::MouseMovedEvent& e) override
	{
		auto camera = m_Scene.ActiveCamera();

		camera->RotateCamera(0.01 * e.m_OffsetX, 0.01 * e.m_OffsetY);
	}

protected:
	BuD::Scene m_Scene;
};

std::shared_ptr<BuD::AppLayer> BuD::CreateClientApp()
{
	return std::shared_ptr<BuD::AppLayer>(new SandboxApp());
}
