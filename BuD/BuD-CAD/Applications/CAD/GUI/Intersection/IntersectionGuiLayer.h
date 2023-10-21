#pragma once

#include <Applications/CAD/GUI/BaseGuiLayerCAD.h>

#include <Applications/CAD/Intersection/BaseIntersectionAlgorithm.h>
#include <Applications/CAD/Visitors/Intersection/CalculatorParameterized.h>

class IntersectionGuiLayer : public BaseGuiLayerCAD
{
public:
	explicit IntersectionGuiLayer(MainDataLayerCAD& dataLayer);

	virtual void DrawGui() override;

protected:
	std::unique_ptr<CalculatorParameterized> m_PointOnSurfaceCalculator;

	IntersectionAlgorithmParameters m_IntersectionParameters;
};
