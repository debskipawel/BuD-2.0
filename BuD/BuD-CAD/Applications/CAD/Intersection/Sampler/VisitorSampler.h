#pragma once

#include <Applications/CAD/Intersection/Sampler/ISampler.h>

class VisitorSampler : public ISampler
{
public:
	VisitorSampler();

	virtual dxm::Vector3 GetPoint(std::weak_ptr<SceneObjectCAD> surface, float u, float v) override;
	virtual dxm::Vector3 GetNormal(std::weak_ptr<SceneObjectCAD> surface, float u, float v) override;
	virtual dxm::Vector3 GetDerivativeU(std::weak_ptr<SceneObjectCAD> surface, float u, float v) override;
	virtual dxm::Vector3 GetDerivativeV(std::weak_ptr<SceneObjectCAD> surface, float u, float v) override;

	virtual WrappedParameter WrapParameter(std::weak_ptr<SceneObjectCAD> surface, float u, float v) override;

private:
	std::unique_ptr<ParameterWrapperVisitor> m_ParameterWrapper;

	std::unique_ptr<CalculatorParameterized> m_PointCalculator;
	std::unique_ptr<CalculatorParameterized> m_UDerivativeCalculator;
	std::unique_ptr<CalculatorParameterized> m_VDerivativeCalculator;
	std::unique_ptr<CalculatorParameterized> m_NormalCalculator;
};
