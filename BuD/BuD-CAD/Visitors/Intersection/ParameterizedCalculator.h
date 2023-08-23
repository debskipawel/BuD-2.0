#pragma once

#include <Visitors/AbstractVisitor.h>

class ParameterizedCalculator : public AbstractVisitor
{
public:
	virtual void Visit(std::weak_ptr<SceneObjectCAD> object) override;

	inline virtual void SetParameter(const dxm::Vector2& parameter) { m_Parameter = parameter; }
	inline virtual dxm::Vector3 Result() const { return m_Result; }

protected:
	virtual dxm::Vector2 GetPatchParameter(BaseBezierSurface& surface);
	virtual std::array<dxm::Vector3, 16> GetControlPoints(BaseBezierSurface& surface);

	virtual dxm::Vector3 DeCastiljeau2(std::array<dxm::Vector3, 3> controlPoints, float t);
	virtual dxm::Vector3 DeCastiljeau3(std::array<dxm::Vector3, 4> controlPoints, float t);

	virtual std::array<dxm::Vector3, 4> BSplineToBernstein(std::array<dxm::Vector3, 4> controlPoints);

	dxm::Vector2 m_Parameter;
	dxm::Vector3 m_Result;
};
