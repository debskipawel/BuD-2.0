#include "PumaMesh.h"

PumaMesh::PumaMesh(BuD::Scene& scene)
{
	auto robotJoints = std::array<dxm::Vector3, 5>
	{
		dxm::Vector3(0.0f, 0.0f, 0.0f),
		dxm::Vector3(0.0f, 2.0f, 0.0f),
		dxm::Vector3(1.0f, 3.0f, 0.0f),
		dxm::Vector3(2.0f, 2.0f, 0.0f),
		dxm::Vector3(2.0f, 2.0f, 1.0f),
	};

	auto colors = std::vector<dxm::Vector3>
	{
		dxm::Vector3::UnitX,
		dxm::Vector3::UnitY,
		dxm::Vector3::UnitZ,
		0.6f * dxm::Vector3::UnitX + 0.3f * dxm::Vector3::UnitY + 0.1f * dxm::Vector3::UnitZ,
	};

	for (size_t i = 0; i + 1 < robotJoints.size(); i++)
	{
		auto position = robotJoints[i];
		auto orientation = robotJoints[i + 1] - robotJoints[i];

		auto length = orientation.Length();

		orientation.Normalize();

		m_RobotSegments[i] = std::make_unique<RobotSegmentMesh>(scene, position, orientation, length, colors[i]);
	}
}

auto PumaMesh::UpdateMesh(std::array<dxm::Vector3, 5> robotJoints) -> void
{
	for (size_t i = 0; i + 1 < robotJoints.size(); i++)
	{
		auto position = robotJoints[i];
		auto orientation = robotJoints[i + 1] - robotJoints[i];

		auto length = orientation.Length();

		orientation.Normalize();

		m_RobotSegments[i]->UpdateInstanceData(position, orientation, length);
	}
}
