#pragma once

#include <memory>

#include <Intersection/IntersectionAlgorithmParameters.h>
#include <Scene/SceneObjectCAD.h>
#include <Visitors/Intersection/CalculatorParameterized.h>

class BaseIntersectionAlgorithm
{
public:
	explicit BaseIntersectionAlgorithm(IntersectionAlgorithmParameters parameters);

	virtual void Find() = 0;

protected:
	std::unique_ptr<CalculatorParameterized> m_PointCalculator;
	std::unique_ptr<CalculatorParameterized> m_DerivativeCalculatorU;
	std::unique_ptr<CalculatorParameterized> m_DerivativeCalculatorV;

	IntersectionAlgorithmParameters m_Parameters;
};
