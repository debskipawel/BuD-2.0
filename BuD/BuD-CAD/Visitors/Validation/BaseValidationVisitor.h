#pragma once

#include <Visitors/AbstractVisitor.h>

class BaseValidationVisitor : public AbstractVisitor
{
public:
	virtual ~BaseValidationVisitor() = default;

	virtual void Visit(std::weak_ptr<SceneObjectCAD> object) override;

	inline virtual bool Valid() const { return m_Valid; }
protected:
	bool m_Valid = true;
};
