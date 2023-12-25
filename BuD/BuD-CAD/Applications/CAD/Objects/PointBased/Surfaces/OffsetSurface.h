#pragma once

#include <Applications/CAD/Scene/SceneObjectCAD.h>

class OffsetSurface : public SceneObjectCAD
{
public:
	OffsetSurface(BuD::Scene& scene, std::weak_ptr<SceneObjectCAD> internalSurface, float offsetValue);

	virtual void Accept(AbstractVisitor& visitor) override;

	inline virtual auto InternalSurface() const noexcept -> std::weak_ptr<SceneObjectCAD> { return m_InternalSurface; }
	inline virtual auto Offset() const noexcept -> float { return m_OffsetValue; }

protected:
	float m_OffsetValue;
	std::weak_ptr<SceneObjectCAD> m_InternalSurface;
};
