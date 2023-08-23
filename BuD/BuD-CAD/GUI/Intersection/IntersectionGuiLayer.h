#pragma once

#include <GUI/BaseGuiLayer.h>

#include <Intersection/BaseIntersectionAlgorithm.h>
#include <Visitors/Intersection/CalculatorParameterized.h>

class IntersectionGuiLayer : public BaseGuiLayer
{
public:
	explicit IntersectionGuiLayer(MainDataLayer& dataLayer);

	virtual void DrawGui() override;

protected:
	std::unique_ptr<CalculatorParameterized> m_PointOnSurfaceCalculator;

	IntersectionAlgorithmParameters m_IntersectionParameters;
};
