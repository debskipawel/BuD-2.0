#pragma once

#include <memory>

#include <Visitors/Intersection/CalculatorParameterized.h>

class CalculatorNormalVector : public CalculatorParameterized
{
public:
	explicit CalculatorNormalVector();

	virtual void Visit(std::weak_ptr<SceneObjectCAD> object) override;

protected:
	std::unique_ptr<CalculatorParameterized> m_DerivativeUVisitor;
	std::unique_ptr<CalculatorParameterized> m_DerivativeVVisitor;
};
