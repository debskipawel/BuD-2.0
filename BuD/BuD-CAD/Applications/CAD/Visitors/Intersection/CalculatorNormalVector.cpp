#include "CalculatorNormalVector.h"

#include <Applications/CAD/Visitors/Intersection/CalculatorPartialDerivativeU.h>
#include <Applications/CAD/Visitors/Intersection/CalculatorPartialDerivativeV.h>

CalculatorNormalVector::CalculatorNormalVector()
{
}

void CalculatorNormalVector::Visit(Torus& torus)
{
	auto derivativeUVisitor = std::make_unique<CalculatorPartialDerivativeU>();
	auto derivativeVVisitor = std::make_unique<CalculatorPartialDerivativeV>();

	derivativeUVisitor->SetParameter(m_Parameter);
	derivativeVVisitor->SetParameter(m_Parameter);

	derivativeUVisitor->Visit(torus);
	derivativeVVisitor->Visit(torus);

	auto uDerivative = derivativeUVisitor->Result();
	auto vDerivative = derivativeVVisitor->Result();

	auto normal = vDerivative.Cross(uDerivative);
	normal.Normalize();

	m_Result = normal;
}

void CalculatorNormalVector::Visit(InfinitePlane& plane)
{
	auto derivativeUVisitor = std::make_unique<CalculatorPartialDerivativeU>();
	auto derivativeVVisitor = std::make_unique<CalculatorPartialDerivativeV>();

	derivativeUVisitor->SetParameter(m_Parameter);
	derivativeVVisitor->SetParameter(m_Parameter);

	derivativeUVisitor->Visit(plane);
	derivativeVVisitor->Visit(plane);

	auto uDerivative = derivativeUVisitor->Result();
	auto vDerivative = derivativeVVisitor->Result();

	auto normal = vDerivative.Cross(uDerivative);
	normal.Normalize();

	m_Result = normal;
}

void CalculatorNormalVector::Visit(FinitePlane& plane)
{
	auto derivativeUVisitor = std::make_unique<CalculatorPartialDerivativeU>();
	auto derivativeVVisitor = std::make_unique<CalculatorPartialDerivativeV>();

	derivativeUVisitor->SetParameter(m_Parameter);
	derivativeVVisitor->SetParameter(m_Parameter);

	derivativeUVisitor->Visit(plane);
	derivativeVVisitor->Visit(plane);

	auto uDerivative = derivativeUVisitor->Result();
	auto vDerivative = derivativeVVisitor->Result();

	auto normal = vDerivative.Cross(uDerivative);
	normal.Normalize();

	m_Result = normal;
}

void CalculatorNormalVector::Visit(BezierSurfaceC0& surface)
{
	auto derivativeUVisitor = std::make_unique<CalculatorPartialDerivativeU>();
	auto derivativeVVisitor = std::make_unique<CalculatorPartialDerivativeV>();

	derivativeUVisitor->SetParameter(m_Parameter);
	derivativeVVisitor->SetParameter(m_Parameter);

	derivativeUVisitor->Visit(surface);
	derivativeVVisitor->Visit(surface);

	auto uDerivative = derivativeUVisitor->Result();
	auto vDerivative = derivativeVVisitor->Result();

	auto normal = vDerivative.Cross(uDerivative);
	normal.Normalize();

	m_Result = normal;
}

void CalculatorNormalVector::Visit(BezierSurfaceC2& surface)
{
	auto derivativeUVisitor = std::make_unique<CalculatorPartialDerivativeU>();
	auto derivativeVVisitor = std::make_unique<CalculatorPartialDerivativeV>();

	derivativeUVisitor->SetParameter(m_Parameter);
	derivativeVVisitor->SetParameter(m_Parameter);

	derivativeUVisitor->Visit(surface);
	derivativeVVisitor->Visit(surface);

	auto uDerivative = derivativeUVisitor->Result();
	auto vDerivative = derivativeVVisitor->Result();

	auto normal = vDerivative.Cross(uDerivative);
	normal.Normalize();

	m_Result = normal;
}

void CalculatorNormalVector::Visit(OffsetSurface& surface)
{
	CalculatorParameterized::Visit(surface.InternalSurface());
}
