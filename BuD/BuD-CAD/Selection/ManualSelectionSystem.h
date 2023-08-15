#pragma once

#include <Selection/BaseSelectionSystem.h>

class ManualSelectionSystem : public BaseSelectionSystem
{
public:
	explicit ManualSelectionSystem() = default;

	virtual void Select(std::weak_ptr<SceneObjectCAD> sceneObject) override;
	virtual void Unselect(uint32_t sceneObjectId) override;

	std::shared_ptr<SceneObjectCAD> First();

	inline virtual bool ValidatedForControlPoints() const { return m_ValidatedForControlPoints; }

protected:
	bool m_ValidatedForControlPoints;
};
