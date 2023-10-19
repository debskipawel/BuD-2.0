#include "AbstractInitialCommonPointFinder.h"

#include <Applications/CAD/Visitors/Intersection/CalculatorPointOnSurface.h>
#include <Applications/CAD/Visitors/Intersection/CalculatorNormalVector.h>
#include <Applications/CAD/Visitors/Intersection/CalculatorPartialDerivativeU.h>
#include <Applications/CAD/Visitors/Intersection/CalculatorPartialDerivativeV.h>

AbstractInitialCommonPointFinder::AbstractInitialCommonPointFinder(std::weak_ptr<SceneObjectCAD> object1, std::weak_ptr<SceneObjectCAD> object2)
	: m_ParameterizedObject1(object1), m_ParameterizedObject2(object2)
{
}
