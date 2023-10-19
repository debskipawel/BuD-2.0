#include "AbstractCommonPointMarching.h"

AbstractCommonPointMarching::AbstractCommonPointMarching(std::weak_ptr<SceneObjectCAD> object1, std::weak_ptr<SceneObjectCAD> object2)
	: m_ParameterizedObject1(object1), m_ParameterizedObject2(object2)
{
}
