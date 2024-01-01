#include "AllJointFramesCalculator.h"

auto AllJointFramesCalculator::Calculate(const RobotConfiguration& configuration, const RobotParameters& parameters)->std::array<Frame, 6>
{
	auto F0 = Frame();

	auto F1 = F0
		.Rotate(dxm::Matrix::CreateFromAxisAngle(F0.Up(), DirectX::XMConvertToRadians(configuration.m_A1)));

	auto F2 = F1
		.Translate(parameters.m_L1 * F1.Up())
		.Rotate(dxm::Matrix::CreateFromAxisAngle(F1.Front(), DirectX::XMConvertToRadians(configuration.m_A2)));

	auto F3 = F2
		.Translate(configuration.m_Q2 * F2.Right())
		.Rotate(dxm::Matrix::CreateFromAxisAngle(F2.Front(), DirectX::XMConvertToRadians(configuration.m_A3)));

	auto F4 = F3
		.Translate(parameters.m_L3 * F3.Up())
		.Rotate(dxm::Matrix::CreateFromAxisAngle(F3.Up(), DirectX::XMConvertToRadians(configuration.m_A4)));

	auto F5 = F4
		.Translate(parameters.m_L4 * F4.Right())
		.Rotate(dxm::Matrix::CreateFromAxisAngle(F4.Right(), DirectX::XMConvertToRadians(configuration.m_A5)));

	return { F0, F1, F2, F3, F4, F5 };
}
