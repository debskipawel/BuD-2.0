#include "VisitorSampler.h"

#include <Applications/CAD/Visitors/Intersection/CalculatorPointOnSurface.h>
#include <Applications/CAD/Visitors/Intersection/CalculatorNormalVector.h>
#include <Applications/CAD/Visitors/Intersection/CalculatorPartialDerivativeU.h>
#include <Applications/CAD/Visitors/Intersection/CalculatorPartialDerivativeV.h>

VisitorSampler::VisitorSampler()
{
	m_ParameterWrapper = std::make_unique<ParameterWrapperVisitor>();

	m_PointCalculator = std::make_unique<CalculatorPointOnSurface>();
	m_NormalCalculator = std::make_unique<CalculatorNormalVector>();
	m_UDerivativeCalculator = std::make_unique<CalculatorPartialDerivativeU>();
	m_VDerivativeCalculator = std::make_unique<CalculatorPartialDerivativeV>();
}

dxm::Vector3 VisitorSampler::GetPoint(std::weak_ptr<SceneObjectCAD> surface, float u, float v)
{
	m_PointCalculator->SetParameter({ u, v });
	m_PointCalculator->Visit(surface);

	return m_PointCalculator->Result();
}

dxm::Vector3 VisitorSampler::GetNormal(std::weak_ptr<SceneObjectCAD> surface, float u, float v)
{
	m_NormalCalculator->SetParameter({ u, v });
	m_NormalCalculator->Visit(surface);

	return m_NormalCalculator->Result();
}

dxm::Vector3 VisitorSampler::GetDerivativeU(std::weak_ptr<SceneObjectCAD> surface, float u, float v)
{
	m_UDerivativeCalculator->SetParameter({ u, v });
	m_UDerivativeCalculator->Visit(surface);

	return m_UDerivativeCalculator->Result();
}

dxm::Vector3 VisitorSampler::GetDerivativeV(std::weak_ptr<SceneObjectCAD> surface, float u, float v)
{
	m_VDerivativeCalculator->SetParameter({ u, v });
	m_VDerivativeCalculator->Visit(surface);

	return m_VDerivativeCalculator->Result();
}

WrappedParameter VisitorSampler::WrapParameter(std::weak_ptr<SceneObjectCAD> surface, float u, float v)
{
	m_ParameterWrapper->SetParameter({ u, v });
	m_ParameterWrapper->Visit(surface);

	WrappedParameter result = {};
	result.m_Parameter = m_ParameterWrapper->Parameter();
	result.m_Wrapped = { m_ParameterWrapper->WrappedU(), m_ParameterWrapper->WrappedV() };
	result.m_OutOfBounds = m_ParameterWrapper->OutOfRange();

	return result;
}
