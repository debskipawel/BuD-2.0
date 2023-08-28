#include "BaseBezierPatch.h"

BaseBezierPatch::BaseBezierPatch(BuD::Scene& scene, std::vector<std::weak_ptr<Point>> controlPoints, dxm::Vector2 rangeU, dxm::Vector2 rangeV)
	: PointBasedObjectCAD(scene, controlPoints), m_DisplayBezierPolygon(false)
{
	m_InstanceData.m_ParameterRangeU = rangeU;
	m_InstanceData.m_ParameterRangeV = rangeV;
}

void BaseBezierPatch::OnPointModify()
{
	m_InstanceData.m_Color = m_Color;

	for (int i = 0; i < m_InstanceData.m_ControlPoints.size(); i++)
	{
		auto& controlPoint = m_ControlPoints[i];

		auto controlPointShared = controlPoint.lock();

		m_InstanceData.m_ControlPoints[i] = controlPointShared
			? controlPointShared->m_Transform.m_Position
			: dxm::Vector3::Zero;
	}
}

bool BaseBezierPatch::ShouldDisplayPolygon()
{
	return m_DisplayBezierPolygon;
}

BuD::MeshDetails BaseBezierPatch::LoadPatchPrimitiveMesh()
{
	auto meshLoader = BuD::MeshLoader();
	
	return meshLoader.LoadPrimitiveMesh(
		BuD::MeshPrimitiveType::POINT_TESSELLATION,
		{
			{ "INS_POINT", 0,  DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_POINT", 1,  DXGI_FORMAT_R32G32B32_FLOAT, 1, 3  * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_POINT", 2,  DXGI_FORMAT_R32G32B32_FLOAT, 1, 6  * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_POINT", 3,  DXGI_FORMAT_R32G32B32_FLOAT, 1, 9  * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_POINT", 4,  DXGI_FORMAT_R32G32B32_FLOAT, 1, 12 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_POINT", 5,  DXGI_FORMAT_R32G32B32_FLOAT, 1, 15 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_POINT", 6,  DXGI_FORMAT_R32G32B32_FLOAT, 1, 18 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_POINT", 7,  DXGI_FORMAT_R32G32B32_FLOAT, 1, 21 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_POINT", 8,  DXGI_FORMAT_R32G32B32_FLOAT, 1, 24 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_POINT", 9,  DXGI_FORMAT_R32G32B32_FLOAT, 1, 27 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_POINT", 10, DXGI_FORMAT_R32G32B32_FLOAT, 1, 30 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_POINT", 11, DXGI_FORMAT_R32G32B32_FLOAT, 1, 33 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_POINT", 12, DXGI_FORMAT_R32G32B32_FLOAT, 1, 36 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_POINT", 13, DXGI_FORMAT_R32G32B32_FLOAT, 1, 39 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_POINT", 14, DXGI_FORMAT_R32G32B32_FLOAT, 1, 42 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_POINT", 15, DXGI_FORMAT_R32G32B32_FLOAT, 1, 45 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_PARAMETER_RANGE_U", 0,  DXGI_FORMAT_R32G32_FLOAT, 1, 48 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_PARAMETER_RANGE_V", 0,  DXGI_FORMAT_R32G32_FLOAT, 1, 50 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_COLOR", 0,  DXGI_FORMAT_R32G32B32_FLOAT, 1, 52 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		}
	);
}

BuD::MeshDetails BaseBezierPatch::LoadPolygonPrimitiveMesh()
{
	auto meshLoader = BuD::MeshLoader();

	return meshLoader.LoadPrimitiveMesh(
		BuD::MeshPrimitiveType::POINT_STANDARD,
		{
			{ "INS_POINT", 0,  DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_POINT", 1,  DXGI_FORMAT_R32G32B32_FLOAT, 1, 3 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_POINT", 2,  DXGI_FORMAT_R32G32B32_FLOAT, 1, 6 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_POINT", 3,  DXGI_FORMAT_R32G32B32_FLOAT, 1, 9 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_POINT", 4,  DXGI_FORMAT_R32G32B32_FLOAT, 1, 12 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_POINT", 5,  DXGI_FORMAT_R32G32B32_FLOAT, 1, 15 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_POINT", 6,  DXGI_FORMAT_R32G32B32_FLOAT, 1, 18 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_POINT", 7,  DXGI_FORMAT_R32G32B32_FLOAT, 1, 21 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_POINT", 8,  DXGI_FORMAT_R32G32B32_FLOAT, 1, 24 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_POINT", 9,  DXGI_FORMAT_R32G32B32_FLOAT, 1, 27 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_POINT", 10, DXGI_FORMAT_R32G32B32_FLOAT, 1, 30 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_POINT", 11, DXGI_FORMAT_R32G32B32_FLOAT, 1, 33 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_POINT", 12, DXGI_FORMAT_R32G32B32_FLOAT, 1, 36 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_POINT", 13, DXGI_FORMAT_R32G32B32_FLOAT, 1, 39 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_POINT", 14, DXGI_FORMAT_R32G32B32_FLOAT, 1, 42 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_POINT", 15, DXGI_FORMAT_R32G32B32_FLOAT, 1, 45 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_PARAMETER_RANGE_U", 0,  DXGI_FORMAT_R32G32_FLOAT, 1, 48 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_PARAMETER_RANGE_V", 0,  DXGI_FORMAT_R32G32_FLOAT, 1, 50 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_COLOR", 0,  DXGI_FORMAT_R32G32B32_FLOAT, 1, 52 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		}
	);
}

dxm::Vector3 BaseBezierPatch::SELECTED_COLOR = { 0.8f, 0.6f, 0.0f };
dxm::Vector3 BaseBezierPatch::UNSELECTED_COLOR = { 1.0f, 1.0f, 1.0f };
