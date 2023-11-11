#pragma once

#include <Applications/CAD/Visitors/Intersection/CalculatorParameterized.h>
#include <Applications/CAD/Visitors/Intersection/ParameterWrapperVisitor.h>

struct WrappedParameter
{
	dxm::Vector2 m_Parameter;
	dxm::Vector2 m_Wrapped;

	bool m_OutOfBounds;
};

class ISampler
{
public:
	virtual dxm::Vector3 GetPoint(std::weak_ptr<SceneObjectCAD> surface, float u, float v) = 0;
	virtual dxm::Vector3 GetNormal(std::weak_ptr<SceneObjectCAD> surface, float u, float v) = 0;
	virtual dxm::Vector3 GetDerivativeU(std::weak_ptr<SceneObjectCAD> surface, float u, float v) = 0;
	virtual dxm::Vector3 GetDerivativeV(std::weak_ptr<SceneObjectCAD> surface, float u, float v) = 0;

	virtual WrappedParameter WrapParameter(std::weak_ptr<SceneObjectCAD> surface, float u, float v) = 0;
};
