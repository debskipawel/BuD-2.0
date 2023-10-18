#pragma once

#include <Intersection/LoopDetectors/AbstractLoopDetector.h>

#include <Visitors/Intersection/CalculatorParameterShortestDistance.h>
#include <Visitors/Intersection/ParameterWrapperVisitor.h>

class ShortestDistanceLoopDetector : public AbstractLoopDetector
{
public:
	ShortestDistanceLoopDetector(std::weak_ptr<SceneObjectCAD> object1, std::weak_ptr<SceneObjectCAD> object2, float pointDistance);

	virtual std::optional<LoopResult> DetectLoop(
		const NextCommonPointResult& starting,
		const NextCommonPointResult& previous,
		const NextCommonPointResult& current
	) override;

protected:
	virtual dxm::Vector2 ShortestVectorBetween(const dxm::Vector2& point1, const dxm::Vector2& point2, std::weak_ptr<SceneObjectCAD> object);
	virtual ParameterWrapResult WrapParameter(const dxm::Vector4& parameter);

	std::weak_ptr<SceneObjectCAD> m_ParameterizedObject1;
	std::weak_ptr<SceneObjectCAD> m_ParameterizedObject2;

	float m_PointDistance;

private:
	std::unique_ptr<CalculatorParameterShortestDistance> m_ShortestDistanceCalculator;
	std::unique_ptr<ParameterWrapperVisitor> m_ParameterWrapper;
};
