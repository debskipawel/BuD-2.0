#include "OffsetSampler.h"

OffsetSampler::OffsetSampler(float offsetValue)
	: m_OffsetValue(offsetValue), m_VisitorSampler(std::make_unique<VisitorSampler>())
{
}

dxm::Vector3 OffsetSampler::GetPoint(std::weak_ptr<SceneObjectCAD> surface, float u, float v)
{
	auto point = m_VisitorSampler->GetPoint(surface, u, v);
	auto normal = m_VisitorSampler->GetNormal(surface, u, v);

	return point + m_OffsetValue * normal;
}

dxm::Vector3 OffsetSampler::GetNormal(std::weak_ptr<SceneObjectCAD> surface, float u, float v)
{
	return m_VisitorSampler->GetNormal(surface, u, v);
}

dxm::Vector3 OffsetSampler::GetDerivativeU(std::weak_ptr<SceneObjectCAD> surface, float u, float v)
{
	return m_VisitorSampler->GetDerivativeU(surface, u, v);
}

dxm::Vector3 OffsetSampler::GetDerivativeV(std::weak_ptr<SceneObjectCAD> surface, float u, float v)
{
	return m_VisitorSampler->GetDerivativeV(surface, u, v);
}

WrappedParameter OffsetSampler::WrapParameter(std::weak_ptr<SceneObjectCAD> surface, float u, float v)
{
	return m_VisitorSampler->WrapParameter(surface, u, v);
}
