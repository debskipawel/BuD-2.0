#include "AbstractCommonPointMarching.h"

AbstractCommonPointMarching::AbstractCommonPointMarching(std::shared_ptr<SceneObjectCAD> object1, std::shared_ptr<SceneObjectCAD> object2)
	: m_ParameterizedObject1(object1), m_ParameterizedObject2(object2)
{
}
