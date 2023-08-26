#include "CalculatorParameterized.h"

static const dxm::Matrix bSplineToBernstein =
{
	1.0f / 6,	0.0f,		0.0f,		0.0f,
	2.0f / 3,	2.0f / 3,	1.0f / 3,	1.0f / 6,
	1.0f / 6,	1.0f / 3,	2.0f / 3,	2.0f / 3,
	0.0,		0.0f,		0.0f,		1.0f / 6
};

void CalculatorParameterized::Visit(std::weak_ptr<SceneObjectCAD> object)
{
	m_Result = dxm::Vector3{ NAN, NAN, NAN };

	AbstractVisitor::Visit(object);
}

void CalculatorParameterized::WrapParameter(bool uWrapped, bool vWrapped)
{
	if (uWrapped)
	{
		m_Parameter.x -= floor(m_Parameter.x);
	}

	if (vWrapped)
	{
		m_Parameter.y -= floor(m_Parameter.y);
	}
}

dxm::Vector2 CalculatorParameterized::GetPatchParameter(BaseBezierSurface& surface)
{
	auto uScaled = m_Parameter.x * surface.m_SizeU;
	auto vScaled = m_Parameter.y * surface.m_SizeV;

	auto uIndex = floor(uScaled);
	auto vIndex = floor(vScaled);

	auto u = uScaled - uIndex;
	auto v = vScaled - vIndex;

	if (uIndex >= surface.m_SizeU)
	{
		uIndex--;
		u = 1.0f;
	}

	if (vIndex >= surface.m_SizeV)
	{
		vIndex--;
		v = 1.0f;
	}

	return { u, v };
}

std::array<dxm::Vector3, 16> CalculatorParameterized::GetControlPoints(BaseBezierSurface& surface)
{
	std::array<dxm::Vector3, 16> controlPoints;
	
	auto uScaled = m_Parameter.x * surface.m_SizeU;
	auto vScaled = m_Parameter.y * surface.m_SizeV;

	auto uIndex = floor(uScaled);
	auto vIndex = floor(vScaled);

	if (uIndex >= surface.m_SizeU)
	{
		uIndex--;
	}

	if (vIndex >= surface.m_SizeV)
	{
		vIndex--;
	}

	auto& patch = surface.m_BezierPatches[vIndex * surface.m_SizeU + uIndex];
	auto patchShared = patch.lock();

	if (!patchShared)
	{
		return controlPoints;
	}

	for (int i = 0; i < 16; i++)
	{
		auto point = patchShared->m_ControlPoints[i].lock();

		controlPoints[i] = point->m_Transform.m_Position;
	}

	return controlPoints;
}

dxm::Vector3 CalculatorParameterized::DeCastiljeau2(std::array<dxm::Vector3, 3> controlPoints, float t)
{
	auto a = dxm::Vector3::Lerp(controlPoints[0], controlPoints[1], t);
	auto b = dxm::Vector3::Lerp(controlPoints[1], controlPoints[2], t);

	return dxm::Vector3::Lerp(a, b, t);
}

dxm::Vector3 CalculatorParameterized::DeCastiljeau3(std::array<dxm::Vector3, 4> controlPoints, float t)
{
	auto a = dxm::Vector3::Lerp(controlPoints[0], controlPoints[1], t);
	auto b = dxm::Vector3::Lerp(controlPoints[1], controlPoints[2], t);
	auto c = dxm::Vector3::Lerp(controlPoints[2], controlPoints[3], t);
	auto d = dxm::Vector3::Lerp(a, b, t);
	auto e = dxm::Vector3::Lerp(b, c, t);

	return dxm::Vector3::Lerp(d, e, t);
}

std::array<dxm::Vector3, 4> CalculatorParameterized::BSplineToBernstein(std::array<dxm::Vector3, 4> controlPoints)
{
	auto x = dxm::Vector4{ controlPoints[0].x, controlPoints[1].x, controlPoints[2].x, controlPoints[3].x };
	auto y = dxm::Vector4{ controlPoints[0].y, controlPoints[1].y, controlPoints[2].y, controlPoints[3].y };
	auto z = dxm::Vector4{ controlPoints[0].z, controlPoints[1].z, controlPoints[2].z, controlPoints[3].z };

	auto xt = dxm::Vector4::Transform(x, bSplineToBernstein.Transpose());
	auto yt = dxm::Vector4::Transform(y, bSplineToBernstein.Transpose());
	auto zt = dxm::Vector4::Transform(z, bSplineToBernstein.Transpose());

	return
	{
		dxm::Vector3{ xt.x, yt.x, zt.x },
		dxm::Vector3{ xt.y, yt.y, zt.y },
		dxm::Vector3{ xt.z, yt.z, zt.z },
		dxm::Vector3{ xt.w, yt.w, zt.w },
	};
}
