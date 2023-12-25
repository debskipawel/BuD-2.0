#pragma once

#include <Applications/CAD/Intersection/Sampler/VisitorSampler.h>

class OffsetSampler : public ISampler
{
public:
	OffsetSampler(float offsetValue);

	virtual dxm::Vector3 GetPoint(std::weak_ptr<SceneObjectCAD> surface, float u, float v) override;
	virtual dxm::Vector3 GetNormal(std::weak_ptr<SceneObjectCAD> surface, float u, float v) override;
	virtual dxm::Vector3 GetDerivativeU(std::weak_ptr<SceneObjectCAD> surface, float u, float v) override;
	virtual dxm::Vector3 GetDerivativeV(std::weak_ptr<SceneObjectCAD> surface, float u, float v) override;

	virtual WrappedParameter WrapParameter(std::weak_ptr<SceneObjectCAD> surface, float u, float v) override;

protected:
	float m_OffsetValue;

	std::unique_ptr<VisitorSampler> m_VisitorSampler;
};
