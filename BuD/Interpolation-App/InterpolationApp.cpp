#include "InterpolationApp.h"

InterpolationApp::InterpolationApp()
	: m_Time(0.0f), m_EulerViewportGuiLayer("Euler"), m_QuaternionViewportGuiLayer("Quaternion"), 
	m_DataLayer(), m_SimulationGuiLayer(m_DataLayer), m_KeyframeListGuiLayer(m_DataLayer),
	m_EulerScene(), m_QuaternionScene(), m_EulerFrame(m_EulerScene), m_QuaternionFrame(m_QuaternionScene)
{
}

void InterpolationApp::OnUpdate(float deltaTime)
{
	m_DataLayer.Update(deltaTime);

	auto animationFrame = m_DataLayer.Interpolate();

	auto eulerAngles = dxm::Vector3(
		DirectX::XMConvertToRadians(animationFrame.m_EulerAngles.x),
		DirectX::XMConvertToRadians(animationFrame.m_EulerAngles.y),
		DirectX::XMConvertToRadians(animationFrame.m_EulerAngles.z)
	);

	auto modelMatrixEuler = dxm::Matrix::CreateFromYawPitchRoll(eulerAngles) * dxm::Matrix::CreateTranslation(animationFrame.m_Position);
	auto modelMatrixQuaternion = dxm::Matrix::CreateFromQuaternion(animationFrame.m_Quaternion) * dxm::Matrix::CreateTranslation(animationFrame.m_Position);

	m_EulerFrame.SetModelMatrix(modelMatrixEuler);
	m_QuaternionFrame.SetModelMatrix(modelMatrixQuaternion);
}

void InterpolationApp::OnRender()
{
	const auto& eulerViewportInfo = m_EulerViewportGuiLayer.GetViewportInfo();
	const auto& quaternionViewportInfo = m_QuaternionViewportGuiLayer.GetViewportInfo();

	BuD::Renderer::BeginTarget(eulerViewportInfo.m_Width, eulerViewportInfo.m_Height);
	BuD::Renderer::Clear(0.0f, 0.0f, 0.0f, 0.0f);

	BuD::Renderer::Render(m_EulerScene);

	auto eulerViewportImage = BuD::Renderer::EndTarget();

	BuD::Renderer::BeginTarget(quaternionViewportInfo.m_Width, quaternionViewportInfo.m_Height);
	BuD::Renderer::Clear(0.0f, 0.0f, 0.0f, 0.0f);

	BuD::Renderer::Render(m_QuaternionScene);

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
