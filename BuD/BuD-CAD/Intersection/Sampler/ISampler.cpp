#include "ISampler.h"

#include <Visitors/Intersection/CalculatorPointOnSurface.h>
#include <Visitors/Intersection/CalculatorNormalVector.h>
#include <Visitors/Intersection/CalculatorPartialDerivativeU.h>
#include <Visitors/Intersection/CalculatorPartialDerivativeV.h>

ISampler::ISampler()
{
	m_ParameterWrapper = std::make_unique<ParameterWrapperVisitor>();

	m_PointCalculator = std::make_unique<CalculatorPointOnSurface>();
	m_NormalCalculator = std::make_unique<CalculatorNormalVector>();
	m_UDerivativeCalculator = std::make_unique<CalculatorPartialDerivativeU>();
	m_VDerivativeCalculator = std::make_unique<CalculatorPartialDerivativeV>();
}

dxm::Vector3 ISampler::GetPoint(std::shared_ptr<SceneObjectCAD> surface, float u, float v)
{
	m_PointCalculator->SetParameter({ u, v });
	m_PointCalculator->Visit(surface);

	return m_PointCalculator->Result();
}

dxm::Vector3 ISampler::GetNormal(std::shared_ptr<SceneObjectCAD> surface, float u, float v)
{
	m_NormalCalculator->SetParameter({ u, v });
	m_NormalCalculator->Visit(surface);

	return m_NormalCalculator->Result();
}

dxm::Vector3 ISampler::GetDerivativeU(std::shared_ptr<SceneObjectCAD> surface, float u, float v)
{
	m_UDerivativeCalculator->SetParameter({ u, v });
	m_UDerivativeCalculator->Visit(surface);

	return m_UDerivativeCalculator->Result();
}

dxm::Vector3 ISampler::GetDerivativeV(std::shared_ptr<SceneObjectCAD> surface, float u, float v)
{
	m_VDerivativeCalculator->SetParameter({ u, v });
	m_VDerivativeCalculator->Visit(surface);

	return m_VDerivativeCalculator->Result();
}
