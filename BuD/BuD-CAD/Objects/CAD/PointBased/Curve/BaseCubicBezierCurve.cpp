#include "BaseCubicBezierCurve.h"

BaseCubicBezierCurve::BaseCubicBezierCurve(BuD::Scene& scene, std::vector<std::weak_ptr<Point>> controlPoints)
	: PointBasedObjectCAD(scene, controlPoints), m_ControlPointBorder(false)
{
}

bool BaseCubicBezierCurve::ShouldRenderControlPointBorder() const
{
	return m_ControlPointBorder;
}

BuD::MeshDetails&& BaseCubicBezierCurve::LoadCurvePrimitiveMesh()
{
	auto meshLoader = BuD::MeshLoader();

	return meshLoader.LoadPrimitiveMesh(
		BuD::MeshPrimitiveType::POINT_TESSELLATION,
		{
			{ "INS_POINT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_POINT", 1, DXGI_FORMAT_R32G32B32_FLOAT, 1, 3 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{ "INS_POINT", 2, DXGI_FORMAT_R32G32B32_FLOAT, 1, 6 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_POINT", 3, DXGI_FORMAT_R32G32B32_FLOAT, 1, 9 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 12 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		}
	);
}

BuD::MeshDetails&& BaseCubicBezierCurve::LoadBorderPrimitiveMesh()
{
	auto meshLoader = BuD::MeshLoader();

	return meshLoader.LoadPrimitiveMesh(
		BuD::MeshPrimitiveType::POINT_STANDARD,
		{
			{ "INS_POINT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_POINT", 1, DXGI_FORMAT_R32G32B32_FLOAT, 1, 3 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{ "INS_POINT", 2, DXGI_FORMAT_R32G32B32_FLOAT, 1, 6 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_POINT", 3, DXGI_FORMAT_R32G32B32_FLOAT, 1, 9 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 12 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		}
	);
}

BuD::InstanceComponent BaseCubicBezierCurve::CreateCurveInstancingComponent()
{
	return BuD::InstanceComponent
	{
		[this]()
		{
			BuD::InstanceData instanceData;
			instanceData.m_Data = (uint8_t*)m_InstanceData.m_Segments.data();
			instanceData.m_InstancesCount = m_InstanceData.m_Segments.size();
			instanceData.m_Size = m_InstanceData.m_Segments.size() * sizeof(CubicBezierCurveSegment);

			return instanceData;
		}
	};
}
