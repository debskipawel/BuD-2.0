#include "AbstractInitialCommonPointFinder.h"

#include <Visitors/Intersection/CalculatorPointOnSurface.h>
#include <Visitors/Intersection/CalculatorNormalVector.h>
#include <Visitors/Intersection/CalculatorPartialDerivativeU.h>
#include <Visitors/Intersection/CalculatorPartialDerivativeV.h>

AbstractInitialCommonPointFinder::AbstractInitialCommonPointFinder(std::weak_ptr<SceneObjectCAD> object1, std::weak_ptr<SceneObjectCAD> object2)
	: m_ParameterizedObject1(object1), m_ParameterizedObject2(object2)
{
}
