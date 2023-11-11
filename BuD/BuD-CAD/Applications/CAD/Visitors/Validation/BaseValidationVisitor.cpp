#include "BaseValidationVisitor.h"

void BaseValidationVisitor::Visit(std::weak_ptr<SceneObjectCAD> object)
{
	m_Valid = true;

	AbstractVisitor::Visit(object);
}
