#pragma once

#include <Visitors/Intersection/CalculatorParameterized.h>
#include <Visitors/Intersection/ParameterWrapperVisitor.h>

class ISampler
{
protected:
	ISampler();

	virtual dxm::Vector3 GetPoint(std::shared_ptr<SceneObjectCAD> surface, float u, float v);
	virtual dxm::Vector3 GetNormal(std::shared_ptr<SceneObjectCAD> surface, float u, float v);
	virtual dxm::Vector3 GetDerivativeU(std::shared_ptr<SceneObjectCAD> surface, float u, float v);
	virtual dxm::Vector3 GetDerivativeV(std::shared_ptr<SceneObjectCAD> surface, float u, float v);

	std::unique_ptr<ParameterWrapperVisitor> m_ParameterWrapper;

private:
	std::unique_ptr<CalculatorParameterized> m_PointCalculator;
	std::unique_ptr<CalculatorParameterized> m_UDerivativeCalculator;
	std::unique_ptr<CalculatorParameterized> m_VDerivativeCalculator;
	std::unique_ptr<CalculatorParameterized> m_NormalCalculator;
};
