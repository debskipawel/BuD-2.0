#include "InterpolationApp.h"

InterpolationApp::InterpolationApp()
	: m_Time(0.0f), m_EulerViewportGuiLayer("Euler"), m_QuaternionViewportGuiLayer("Quaternion"), m_MouseMove(false),
	m_SimulationDataLayer(), m_SimulationGuiLayer(m_SimulationDataLayer), m_KeyframeListGuiLayer(m_SimulationDataLayer)
{
}

void InterpolationApp::OnUpdate(float deltaTime)
{
	m_SimulationDataLayer.Update(deltaTime);

	auto animationFrame = m_SimulationDataLayer.Interpolate();

	auto eulerAngles = dxm::Vector3(
		DirectX::XMConvertToRadians(animationFrame.m_EulerAngles.x),
		DirectX::XMConvertToRadians(animationFrame.m_EulerAngles.y),
		DirectX::XMConvertToRadians(animationFrame.m_EulerAngles.z)
	);

	auto modelMatrixEuler = dxm::Matrix::CreateFromYawPitchRoll(eulerAngles) * dxm::Matrix::CreateTranslation(animationFrame.m_Position);
	auto modelMatrixQuaternion = dxm::Matrix::CreateFromQuaternion(animationFrame.m_Quaternion) * dxm::Matrix::CreateTranslation(animationFrame.m_Position);

	m_SceneDataLayer.m_EulerFrame.SetModelMatrix(modelMatrixEuler);
	m_SceneDataLayer.m_QuaternionFrame.SetModelMatrix(modelMatrixQuaternion);
}

void InterpolationApp::OnRender()
{
	const auto& eulerViewportInfo = m_EulerViewportGuiLayer.GetViewportInfo();
	const auto& quaternionViewportInfo = m_QuaternionViewportGuiLayer.GetViewportInfo();

	BuD::Renderer::BeginTarget(eulerViewportInfo.m_Width, eulerViewportInfo.m_Height);
	BuD::Renderer::Clear(0.0f, 0.0f, 0.0f, 0.0f);

	BuD::Renderer::Render(m_SceneDataLayer.m_EulerScene);

	auto eulerViewportImage = BuD::Renderer::EndTarget();

	BuD::Renderer::BeginTarget(quaternionViewportInfo.m_Width, quaternionViewportInfo.m_Height);
	BuD::Renderer::Clear(0.0f, 0.0f, 0.0f, 0.0f);

	BuD::Renderer::Render(m_SceneDataLayer.m_QuaternionScene);

	auto quaternionViewportImage = BuD::Renderer::EndTarget();

	m_EulerViewportGuiLayer.UpdateImage(eulerViewportImage);
	m_QuaternionViewportGuiLayer.UpdateImage(quaternionViewportImage);
}

void InterpolationApp::OnGuiRender()
{
	m_EulerViewportGuiLayer.DrawGui();
	m_QuaternionViewportGuiLayer.DrawGui();

	m_SimulationGuiLayer.DrawGui();
	m_KeyframeListGuiLayer.DrawGui();
}

void InterpolationApp::OnConcreteEvent(BuD::KeyDownEvent& e)
{
	if (e.m_Key == BuD::KeyboardKeys::Space)
	{
		m_SimulationDataLayer.m_Running ? m_SimulationDataLayer.Stop() : m_SimulationDataLayer.Run();
	}
}

void InterpolationApp::OnConcreteEvent(BuD::MouseScrolledEvent& e)
{
	m_SceneDataLayer.m_EulerScene.ActiveCamera()->Zoom(-0.03f * e.m_WheelDelta);
	m_SceneDataLayer.m_QuaternionScene.ActiveCamera()->Zoom(-0.03f * e.m_WheelDelta);
}

void InterpolationApp::OnConcreteEvent(BuD::MouseButtonDownEvent& e)
{
	if (e.m_Button == BuD::MouseCode::MIDDLE)
	{
		m_MouseMove = true;
	}
}

void InterpolationApp::OnConcreteEvent(BuD::MouseButtonReleasedEvent& e)
{
	if (e.m_Button == BuD::MouseCode::MIDDLE)
	{
		m_MouseMove = false;
	}
}

void InterpolationApp::OnConcreteEvent(BuD::MouseMovedEvent& e)
{
	if (m_MouseMove)
	{
		auto eulerCamera = m_SceneDataLayer.m_EulerScene.ActiveCamera();
		auto quaternionCamera = m_SceneDataLayer.m_QuaternionScene.ActiveCamera();

		eulerCamera->RotateCamera(0.005 * e.m_OffsetX, 0.005 * e.m_OffsetY);
		quaternionCamera->RotateCamera(0.005 * e.m_OffsetX, 0.005 * e.m_OffsetY);
	}
}
