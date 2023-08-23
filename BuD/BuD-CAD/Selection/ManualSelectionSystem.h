#pragma once

#include <Visitors/Validation/BaseValidationVisitor.h>
#include <Selection/BaseSelectionSystem.h>

class ManualSelectionSystem : public BaseSelectionSystem
{
public:
	explicit ManualSelectionSystem();
	explicit ManualSelectionSystem(const ManualSelectionSystem& other) = default;

	virtual void Clear() override;

	virtual void Select(std::weak_ptr<SceneObjectCAD> sceneObject) override;
	virtual void Unselect(uint32_t sceneObjectId) override;

	std::shared_ptr<SceneObjectCAD> First();

	inline virtual bool ValidatedForControlPoints() const { return m_ValidatedForControlPoints; }
	inline virtual bool ValidatedForAddingControlPoint() const { return m_ValidatedForAddingControlPoint; }

	virtual bool EligibleForIntersection() const;

protected:
	std::unique_ptr<BaseValidationVisitor> m_IntersectionEligibilityValidator;

	bool m_ValidatedForControlPoints;
	bool m_ValidatedForAddingControlPoint;
};
