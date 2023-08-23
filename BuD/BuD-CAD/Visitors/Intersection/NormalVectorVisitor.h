#pragma once

#include <memory>

#include <Visitors/Intersection/ParameterizedCalculator.h>

class NormalVectorVisitor : public ParameterizedCalculator
{
public:
	explicit NormalVectorVisitor();

	virtual void Visit(std::weak_ptr<SceneObjectCAD> object) override;

protected:
	std::unique_ptr<ParameterizedCalculator> m_DerivativeUVisitor;
	std::unique_ptr<ParameterizedCalculator> m_DerivativeVVisitor;
};
