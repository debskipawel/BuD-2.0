#include "UpdateTransformVisitor.h"

void UpdateTransformVisitor::Visit(Torus& torus)
{
	const auto& transform = torus.m_Transform;

	auto rotation = transform.m_Rotation;
	rotation.x = DirectX::XMConvertToRadians(rotation.x);
	rotation.y = DirectX::XMConvertToRadians(rotation.y);
	rotation.z = DirectX::XMConvertToRadians(rotation.z);

	auto model = dxm::Matrix::CreateScale(transform.m_Scale) * dxm::Matrix::CreateFromYawPitchRoll(rotation) * dxm::Matrix::CreateTranslation(transform.m_Position);

	torus.m_InstanceData.m_ModelMatrix = model;
}

void UpdateTransformVisitor::Visit(Point& point)
{
	if (point.m_InstanceData.m_Position == point.m_Transform.m_Position)
	{
		return;
	}

	point.m_InstanceData.m_Position = point.m_Transform.m_Position;
}

void UpdateTransformVisitor::Visit(FinitePlane& plane)
{
	const auto& transform = plane.m_Transform;

	auto rotation = transform.m_Rotation;
	rotation.x = DirectX::XMConvertToRadians(rotation.x);
	rotation.y = DirectX::XMConvertToRadians(rotation.y);
	rotation.z = DirectX::XMConvertToRadians(rotation.z);

	auto model = dxm::Matrix::CreateFromYawPitchRoll(rotation);

	auto rotatedU = dxm::Vector3::Transform(plane.GetU(), model);
	auto rotatedV = dxm::Vector3::Transform(plane.GetV(), model);

	plane.m_InstanceData.m_Position = transform.m_Position;
	plane.m_InstanceData.m_VectorU = rotatedU;
	plane.m_InstanceData.m_VectorV = rotatedV;
}
