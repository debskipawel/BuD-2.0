#include "NormalVectorVisitor.h"

#include <Visitors/Intersection/PartialDerivativeUVisitor.h>
#include <Visitors/Intersection/PartialDerivativeVVisitor.h>

NormalVectorVisitor::NormalVectorVisitor()
{
	m_DerivativeUVisitor = std::make_unique<PartialDerivativeUVisitor>();
	m_DerivativeVVisitor = std::make_unique<PartialDerivativeVVisitor>();
}

void NormalVectorVisitor::Visit(std::weak_ptr<SceneObjectCAD> object)
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
