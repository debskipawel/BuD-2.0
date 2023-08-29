#include "ApplyGroupTransformVisitor.h"

ApplyGroupTransformVisitor::ApplyGroupTransformVisitor(const TransformComponent& additionalTransform, const dxm::Vector3& centroid)
	: m_AdditionalTransform(additionalTransform), m_Centroid(centroid)
{
}

void ApplyGroupTransformVisitor::Visit(std::weak_ptr<SceneObjectCAD> object)
{
	AbstractVisitor::Visit(object);
}

void ApplyGroupTransformVisitor::SetInitialTransform(const TransformComponent& transform)
{
	m_InitialTransform = transform;
}

void ApplyGroupTransformVisitor::Visit(Torus& torus)
{
	auto centroidToTorus = m_InitialTransform.m_Position - m_Centroid;
	auto rotation = m_AdditionalTransform.m_Rotation;
	rotation.x = DirectX::XMConvertToRadians(rotation.x);
	rotation.y = DirectX::XMConvertToRadians(rotation.y);
	rotation.z = DirectX::XMConvertToRadians(rotation.z);

	auto scaleMatrix = dxm::Matrix::CreateScale(m_AdditionalTransform.m_Scale);
	auto scaledCentroidToTorus = dxm::Vector3::Transform(centroidToTorus, scaleMatrix);

	auto rotationMatrix = dxm::Matrix::CreateFromYawPitchRoll(rotation);
	auto rotatedCentroidToTorus = dxm::Vector3::Transform(scaledCentroidToTorus, rotationMatrix);

	auto accumulatedPosition = m_Centroid + rotatedCentroidToTorus;

	auto initialRotation = m_InitialTransform.m_Rotation;
	initialRotation.x = DirectX::XMConvertToRadians(initialRotation.x);
	initialRotation.y = DirectX::XMConvertToRadians(initialRotation.y);
	initialRotation.z = DirectX::XMConvertToRadians(initialRotation.z);

	auto initialRotationMatrix = dxm::Matrix::CreateFromYawPitchRoll(initialRotation);
	auto combinedRotation = initialRotationMatrix * rotationMatrix;
	auto newRotation = combinedRotation.ToEuler();
	newRotation.x = DirectX::XMConvertToDegrees(newRotation.x);
	newRotation.y = DirectX::XMConvertToDegrees(newRotation.y);
	newRotation.z = DirectX::XMConvertToDegrees(newRotation.z);

	auto newScale = m_InitialTransform.m_Scale * m_AdditionalTransform.m_Scale;

	torus.m_Transform.m_Position = m_AdditionalTransform.m_Position + accumulatedPosition;
	torus.m_Transform.m_Rotation = newRotation;
	torus.m_Transform.m_Scale = newScale;

	torus.m_InstanceData.m_ModelMatrix = 
		combinedRotation *
		dxm::Matrix::CreateScale(torus.m_Transform.m_Scale) * 
		dxm::Matrix::CreateTranslation(torus.m_Transform.m_Position);
}

void ApplyGroupTransformVisitor::Visit(Point& point)
{
	auto centroidToPoint = m_InitialTransform.m_Position - m_Centroid;
	
	auto rotation = m_AdditionalTransform.m_Rotation;
	rotation.x = DirectX::XMConvertToRadians(rotation.x);
	rotation.y = DirectX::XMConvertToRadians(rotation.y);
	rotation.z = DirectX::XMConvertToRadians(rotation.z);

	auto scaleMatrix = dxm::Matrix::CreateScale(m_AdditionalTransform.m_Scale);
	auto scaledCentroidToPoint = dxm::Vector3::Transform(centroidToPoint, scaleMatrix);

	auto rotationMatrix = dxm::Matrix::CreateFromYawPitchRoll(rotation);
	auto rotatedCentroidToPoint = dxm::Vector3::Transform(scaledCentroidToPoint, rotationMatrix);

	auto accumulatedPosition = m_Centroid + rotatedCentroidToPoint;

	point.m_Transform.m_Position = m_AdditionalTransform.m_Position + accumulatedPosition;
	point.m_InstanceData.m_Position = point.m_Transform.m_Position;
}
