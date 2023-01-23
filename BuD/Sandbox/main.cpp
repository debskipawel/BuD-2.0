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
		BuD::Renderer::BeginTarget(400, 200);
		BuD::Renderer::Clear(0.4f, 0.2f, 0.5f, 1.0f);
		m_Viewport = BuD::Renderer::EndTarget();
	}

	void OnGuiRender() override
	{
		ImGui::Begin("Viewport 1");
		ImGui::Image(m_Viewport.Get(), { 400, 200 });
		ImGui::End();
	}

protected:
	BuD::Scene m_Scene;
	ComPtr<ID3D11ShaderResourceView> m_Viewport;
};

std::shared_ptr<BuD::AppLayer> BuD::CreateClientApp()
{
	return std::shared_ptr<BuD::AppLayer>(new SandboxApp());
}
