#include "CalculatorNormalVector.h"

#include <Applications/CAD/Visitors/Intersection/CalculatorPartialDerivativeU.h>
#include <Applications/CAD/Visitors/Intersection/CalculatorPartialDerivativeV.h>

CalculatorNormalVector::CalculatorNormalVector()
{
	m_DerivativeUVisitor = std::make_unique<CalculatorPartialDerivativeU>();
	m_DerivativeVVisitor = std::make_unique<CalculatorPartialDerivativeV>();
}

void CalculatorNormalVector::Visit(std::weak_ptr<SceneObjectCAD> object)
{
	m_DerivativeUVisitor->SetParameter(m_Parameter);
	m_DerivativeVVisitor->SetParameter(m_Parameter);

	m_DerivativeUVisitor->Visit(object);
	m_DerivativeVVisitor->Visit(object);

	auto uDerivative = m_DerivativeUVisitor->Result();
	auto vDerivative = m_DerivativeVVisitor->Result();

	auto normal = vDerivative.Cross(uDerivative);
	normal.Normalize();

	m_Result = normal;
}
