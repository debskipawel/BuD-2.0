#include <BuD.h>
#include <Core/EntryPoint.h>

#include <Renderer/Renderer.h>

#include "imgui.h"

class SandboxApp : public BuD::AppLayer
{
public:
	SandboxApp()
		: m_Scene()
	{}

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
	}

protected:
	BuD::Scene m_Scene;
};

std::shared_ptr<BuD::AppLayer> BuD::CreateClientApp()
{
	return std::shared_ptr<BuD::AppLayer>(new SandboxApp());
}
