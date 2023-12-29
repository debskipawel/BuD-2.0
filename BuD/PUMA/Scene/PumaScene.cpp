#include "PumaScene.h"

#include <Robot/Frame.h>

PumaScene::PumaScene()
	: m_Scene()
{
	m_Grid = std::make_unique<Grid>(m_Scene);
	m_RobotMesh = std::make_unique<PumaMesh>(m_Scene);
}

auto PumaScene::UpdateRobotMesh(const RobotParameters& parameters, const RobotConfiguration& robotConfiguration) -> void
{
	auto F0 = Frame();

	auto F1 = F0
		.Rotate(dxm::Matrix::CreateFromAxisAngle(F0.Up(), DirectX::XMConvertToRadians(robotConfiguration.m_A1)));
	
	auto F2 = F1
		.Translate(parameters.m_L1 * F1.Up())
		.Rotate(dxm::Matrix::CreateFromAxisAngle(F1.Front(), DirectX::XMConvertToRadians(robotConfiguration.m_A2)));
	
	auto F3 = F2
		.Translate(robotConfiguration.m_Q2 * F2.Right())
		.Rotate(dxm::Matrix::CreateFromAxisAngle(F2.Front(), DirectX::XMConvertToRadians(robotConfiguration.m_A3)));

	auto F4 = F3
		.Translate(parameters.m_L3 * F3.Up())
		.Rotate(dxm::Matrix::CreateFromAxisAngle(F3.Up(), DirectX::XMConvertToRadians(robotConfiguration.m_A4)));

	auto F5 = F4
		.Translate(parameters.m_L4 * F4.Right())
		.Rotate(dxm::Matrix::CreateFromAxisAngle(F4.Right(), DirectX::XMConvertToRadians(robotConfiguration.m_A5)));

	m_RobotMesh->UpdateMesh({ F1.Position(), F2.Position(), F3.Position(), F4.Position(), F5.Position() });
}

auto PumaScene::Scene() -> BuD::Scene&
{
	return m_Scene;
}
