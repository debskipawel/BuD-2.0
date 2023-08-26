#pragma once

#include <Visitors/AbstractVisitor.h>

class ParameterWrapperVisitor : public AbstractVisitor
{
public:
	virtual void Visit(std::weak_ptr<SceneObjectCAD> object) override;

	virtual void Visit(Torus& torus) override;

	virtual void Visit(BezierSurfaceC0& surface) override;
	virtual void Visit(BezierSurfaceC2& surface) override;

	virtual void SetParameter(const dxm::Vector2& parameter);

	virtual inline const dxm::Vector2& Parameter() const { return m_Parameter; }
	
	virtual inline bool WrappedU() const { return m_WrappedU; }
	virtual inline bool WrappedV() const { return m_WrappedV; }

	virtual inline bool OutOfRange() const { return m_ParameterOutOfRange; }

protected:
	dxm::Vector2 m_Parameter;
	
	bool m_ParameterOutOfRange;
	bool m_WrappedU, m_WrappedV;
};

