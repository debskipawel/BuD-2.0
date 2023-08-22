#pragma once

#include <Selection/BaseSelectionSystem.h>

class ManualSelectionSystem : public BaseSelectionSystem
{
public:
	explicit ManualSelectionSystem() = default;
	explicit ManualSelectionSystem(const ManualSelectionSystem& other) = default;

	virtual void Clear() override;

	virtual void Select(std::weak_ptr<SceneObjectCAD> sceneObject) override;
	virtual void Unselect(uint32_t sceneObjectId) override;

	std::shared_ptr<SceneObjectCAD> First();

	inline virtual bool ValidatedForControlPoints() const { return m_ValidatedForControlPoints; }
	inline virtual bool ValidatedForAddingControlPoint() const { return m_ValidatedForAddingControlPoint; }

protected:
	bool m_ValidatedForControlPoints;
	bool m_ValidatedForAddingControlPoint;
};
