#include <BuD.h>
#include <Core/EntryPoint.h>

#include <Renderer/Renderer.h>

#include "imgui.h"

class SandboxApp : public BuD::AppLayer
{
public:
	SandboxApp()
		: m_Counter(), m_Viewport()
	{}

	void OnUpdate(float deltaTime) override
	{
		m_Counter++;
	}

	void OnRender() override
	{
		BuD::Renderer::Clear(0.2f, 0.2f, 0.2f, 1.0f);

		BuD::Renderer::StartTarget(400, 200);
		BuD::Renderer::Clear(powf(sinf(BuD::Clock::Now()), 2), 0.2f, 0.5f, 1.0f);
		m_Viewport = BuD::Renderer::GetTarget();
	}

	void OnGuiRender() override
	{
		ImGui::Begin("Viewport1");
		ImGui::Image(m_Viewport.Get(), { 400, 200 });
		ImGui::End();
	}

protected:
	ComPtr<ID3D11ShaderResourceView> m_Viewport;
	int m_Counter;
};

std::shared_ptr<BuD::AppLayer> BuD::CreateClientApp()
{
	return std::shared_ptr<BuD::AppLayer>(new SandboxApp());
}
