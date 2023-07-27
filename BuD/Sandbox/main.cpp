#include <BuD.h>

#include <sstream>

#include <Core/EntryPoint.h>

#include <Profiler/Profiler.h>
#include <Renderer/Renderer.h>
#include <Utils/HelperFunctions.h>

#include "BlackHoleQuadStandard.h"
#include "BlackHoleQuadInstanced.h"

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
				OutputDebugStringW(L"\n");
			}
		);

		m_BlackHole = std::make_unique<BlackHoleQuadStandard>(m_Scene);
		m_BlackHoleInstanced = std::make_unique<BlackHoleQuadInstanced>(m_SceneInstanced);

		// set initial camera distance
		const auto initialDistance = 250.0f;
		auto camera = m_Scene.ActiveCamera();
		auto cameraInstanced = m_SceneInstanced.ActiveCamera();

		auto blackHolePosition = m_BlackHole->Position();
		auto cameraPosition = camera->EyePosition();
		auto toCameraVector = cameraPosition - blackHolePosition;

		float currentDistance = toCameraVector.Length();

		camera->Zoom(initialDistance - currentDistance);
		cameraInstanced->Zoom(initialDistance - currentDistance);
	}

	template <typename T>
	std::string ToStringWithPrecision(T value, unsigned int precision)
	{
		std::ostringstream out;
		out.precision(precision);
		out << std::fixed << value;

		return std::move(out).str();
	}

	void OnUpdate(float deltaTime) override
	{
	}

	void OnRender() override
	{
		BuD::Renderer::BeginTarget(m_ViewportSize.x, m_ViewportSize.y);
		BuD::Renderer::Clear(0.0f, 0.0f, 0.0f, 1.0f);
		
		BuD::Profiler::BeginScope("Render");
		BuD::Renderer::Render(m_DrawInstanced ? m_SceneInstanced : m_Scene);
		BuD::Profiler::EndScope();

		m_Viewport = BuD::Renderer::EndTarget();
	}

	void OnGuiRender() override
	{
		BuD::Profiler::BeginScope("Gui render");

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

		auto camera = m_Scene.ActiveCamera();
		auto cameraInstanced = m_SceneInstanced.ActiveCamera();

		auto blackHolePosition = m_BlackHoleInstanced->Position();
		auto cameraPosition = camera->EyePosition();

		auto toCameraVector = cameraPosition - blackHolePosition;

		// DISTANCE
		float distance = toCameraVector.Length();
		float distanceCopy = distance;

		constexpr float MIN_DIST = 1.0f;
		constexpr float MAX_DIST = 500.0f;

		ImGui::DragFloat("Distance ##blackHoleDistance", &distance, 1.0f, MIN_DIST, MAX_DIST, "%.1f");

		distance = min(distance, MAX_DIST);
		distance = max(distance, MIN_DIST);

		camera->Zoom(distance - distanceCopy);
		cameraInstanced->Zoom(distance - distanceCopy);

		// MASS
		auto& mass = m_BlackHoleInstanced->m_BlackHoleMass;

		constexpr float MIN_MASS = 0.1f;
		constexpr float MAX_MASS = 10.0f;

		ImGui::DragFloat("Mass ##blackHoleMass", &mass, 0.1f, MIN_MASS, MAX_MASS, "%.1f");

		mass = min(mass, MAX_MASS);
		mass = max(mass, MIN_MASS);

		m_BlackHole->m_BlackHoleMass = mass;

		ImGui::Separator();

		const char* items[] = { "Red Galaxy", "Linus Sebastian" };
		static int item_current_idx = 0; // Here we store our selection data as an index.
		const char* combo_preview_value = items[item_current_idx];

		ImGui::Text("Choose skybox:");

		if (ImGui::BeginCombo("##skyboxCombo", items[item_current_idx]))
		{
			for (int n = 0; n < IM_ARRAYSIZE(items); n++)
			{
				const bool is_selected = (item_current_idx == n);
				if (ImGui::Selectable(items[n], is_selected))
					item_current_idx = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		m_BlackHole->m_ActiveCubemap = (ActiveCubemap)item_current_idx;
		m_BlackHoleInstanced->m_ActiveCubemap = (ActiveCubemap)item_current_idx;

		ImGui::Separator();

		if (ImGui::CollapsingHeader("Performance"))
		{
			ImGui::Checkbox("Instancing enabled ##draw_instanced", &m_DrawInstanced);

			auto frameStats = BuD::Renderer::GetLastFrameStats();
			auto frameTime = BuD::Profiler::FrameTime();

			std::string performanceText = "Frame time: " + ToStringWithPrecision(frameTime, 1) + " [ms], FPS: " + ToStringWithPrecision(1000.0f / frameTime, 2);
			ImGui::Text(performanceText.c_str());

			performanceText = "Draw calls: " + std::to_string(frameStats.m_DrawCalls) + ", instances drawn: " + std::to_string(frameStats.m_InstancesDrawn);
			ImGui::Text(performanceText.c_str());

			BuD::Profiler::InOrder(
				[](std::shared_ptr<BuD::Profiler::ScopeNode> node, unsigned int recursionLevel, unsigned int childId, unsigned int selfId)
				{
					auto duration = BuD::HelperFunctions::FormatWithPrecision((float)node->DurationMs(), 2);
					auto text = std::format("{} [{} ms]", node->m_Name, duration);

					if (node->m_Children.size())
					{
						text += std::format("###tree_node_{}_{}_{}", recursionLevel, selfId, childId);
						return ImGui::TreeNode(text.c_str());
					}

					ImGui::Text(text.c_str());
				},
				[](std::shared_ptr<BuD::Profiler::ScopeNode> node, unsigned int recursionLevel, unsigned int childId, unsigned int selfId)
				{
					if (node->m_Children.size())
					{
						ImGui::TreePop();
					}
				}
			);
		}

		ImGui::End();

		BuD::Profiler::EndScope();
	}

	void OnConcreteEvent(BuD::MouseMovedEvent& e) override
	{
		if (m_MoveMouse)
		{
			auto camera = m_Scene.ActiveCamera();
			auto cameraInstanced = m_SceneInstanced.ActiveCamera();

			camera->RotateCamera(0.005 * e.m_OffsetX, 0.005 * e.m_OffsetY);
			cameraInstanced->RotateCamera(0.005 * e.m_OffsetX, 0.005 * e.m_OffsetY);
		}
	}

	void OnConcreteEvent(BuD::MouseScrolledEvent& e) override
	{
		auto camera = m_Scene.ActiveCamera();
		auto cameraInstanced = m_SceneInstanced.ActiveCamera();

		camera->Zoom(-0.03f * e.m_WheelDelta);
		cameraInstanced->Zoom(-0.03f * e.m_WheelDelta);
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

	void OnConcreteEvent(BuD::KeyDownEvent& e) override
	{
		if (e.m_Key == BuD::KeyboardKeys::Q)
		{
			m_MoveMouse = true;
		}
	}

	void OnConcreteEvent(BuD::KeyReleaseEvent& e) override
	{
		if (e.m_Key == BuD::KeyboardKeys::Q)
		{
			m_MoveMouse = false;
		}
	}

protected:

	BuD::Texture m_Viewport;
	ImVec2 m_ViewportSize = { 0, 0 };

	BuD::Scene m_Scene;
	BuD::Scene m_SceneInstanced;
	std::unique_ptr<BlackHoleQuadBase> m_BlackHole;
	std::unique_ptr<BlackHoleQuadBase> m_BlackHoleInstanced;

	bool m_MoveMouse = false;
	bool m_DrawInstanced = true;
};

std::shared_ptr<BuD::AppLayer> BuD::CreateClientApp()
{
	return std::shared_ptr<BuD::AppLayer>(new SandboxApp());
}
