#include "BaseIntersectionAlgorithm.h"

#include <Visitors/Validation/IntersectionEligibilityValidationVisitor.h>

#include <Visitors/Intersection/CalculatorPointOnSurface.h>
#include <Visitors/Intersection/CalculatorPartialDerivativeU.h>
#include <Visitors/Intersection/CalculatorPartialDerivativeV.h>

BaseIntersectionAlgorithm::BaseIntersectionAlgorithm(IntersectionAlgorithmParameters parameters)
	: m_Parameters(parameters)
{
	m_PointCalculator = std::make_unique<CalculatorPointOnSurface>();
	m_DerivativeCalculatorU = std::make_unique<CalculatorPartialDerivativeU>();
	m_DerivativeCalculatorV = std::make_unique<CalculatorPartialDerivativeV>();
}
