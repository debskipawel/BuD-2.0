#include "MainDataLayer.h"

MainDataLayer::MainDataLayer()
	: m_SceneDataLayer(), m_SimulationDataLayer(), m_IntermediateFramesCount(20), m_ShowGhost(false)
{
}

void MainDataLayer::UpdateGhostMesh()
{
	if (!m_ShowGhost || m_IntermediateFramesCount == 0)
	{
		m_SceneDataLayer.m_EulerGhost.DisableRendering();
		m_SceneDataLayer.m_QuaternionGhost.DisableRendering();

		return;
	}

	m_SceneDataLayer.m_EulerGhost.EnableRendering();
	m_SceneDataLayer.m_QuaternionGhost.EnableRendering();

	auto& animationClip = m_SimulationDataLayer.m_AnimationClip;

	auto intermediateFrames = animationClip.GetIntermediateFrames(m_IntermediateFramesCount);

	auto eulerFrames = std::vector<dxm::Matrix>(intermediateFrames.size());
	auto quaternionFrames = std::vector<dxm::Matrix>(intermediateFrames.size());

	std::transform(intermediateFrames.begin(), intermediateFrames.end(), eulerFrames.begin(),
		[](const KeyFrame& frame)
		{
			auto rotation = dxm::Vector3(
				DirectX::XMConvertToRadians(frame.m_EulerAngles.x),
				DirectX::XMConvertToRadians(frame.m_EulerAngles.y),
				DirectX::XMConvertToRadians(frame.m_EulerAngles.z)
			);

			return dxm::Matrix::CreateFromYawPitchRoll(rotation) * dxm::Matrix::CreateTranslation(frame.m_Position);
		}
	);

	std::transform(intermediateFrames.begin(), intermediateFrames.end(), quaternionFrames.begin(),
		[](const KeyFrame& frame)
		{
			return dxm::Matrix::CreateFromQuaternion(frame.m_Quaternion) * dxm::Matrix::CreateTranslation(frame.m_Position);
		}
	);

	m_SceneDataLayer.m_EulerGhost.UpdateModelMatrices(eulerFrames);
	m_SceneDataLayer.m_QuaternionGhost.UpdateModelMatrices(quaternionFrames);
}
