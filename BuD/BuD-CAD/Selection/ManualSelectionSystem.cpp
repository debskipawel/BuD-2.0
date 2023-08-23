#include "ManualSelectionSystem.h"

#include <Visitors/Validation/ControlPointValidationVisitor.h>
#include <Visitors/Validation/CanAddControlPointValidationVisitor.h>
#include <Visitors/Validation/IntersectionEligibilityValidationVisitor.h>

ManualSelectionSystem::ManualSelectionSystem()
{
    m_IntersectionEligibilityValidator = std::make_unique<IntersectionEligibilityValidationVisitor>();
}

void ManualSelectionSystem::Clear()
{
    BaseSelectionSystem::Clear();

    m_ValidatedForControlPoints = false;
    m_ValidatedForAddingControlPoint = false;
}

void ManualSelectionSystem::Select(std::weak_ptr<SceneObjectCAD> sceneObject)
{
    if (m_SelectedObjects.empty())
    {
        m_ValidatedForControlPoints = true;
        m_ValidatedForAddingControlPoint = true;
    }

    BaseSelectionSystem::Select(sceneObject);

    auto sceneObjectShared = sceneObject.lock();
    std::unique_ptr<BaseValidationVisitor> isControlPointVisitor = std::make_unique<ControlPointValidationVisitor>();
    std::unique_ptr<BaseValidationVisitor> canAddControlPointVisitor = std::make_unique<CanAddControlPointValidationVisitor>();

    isControlPointVisitor->Visit(sceneObject);
    canAddControlPointVisitor->Visit(sceneObject);

    m_ValidatedForControlPoints = m_ValidatedForControlPoints && isControlPointVisitor->Valid();
    m_ValidatedForAddingControlPoint = m_ValidatedForAddingControlPoint && canAddControlPointVisitor->Valid();
}

void ManualSelectionSystem::Unselect(uint32_t sceneObjectId)
{
    BaseSelectionSystem::Unselect(sceneObjectId);

    std::unique_ptr<BaseValidationVisitor> isControlPointVisitor = std::make_unique<ControlPointValidationVisitor>();
    std::unique_ptr<BaseValidationVisitor> canAddControlPointVisitor = std::make_unique<CanAddControlPointValidationVisitor>();

    m_ValidatedForControlPoints = !m_SelectedObjects.empty();
    m_ValidatedForAddingControlPoint = !m_SelectedObjects.empty();

    for (auto& [id, object] : m_SelectedObjects)
    {
        isControlPointVisitor->Visit(object);
        canAddControlPointVisitor->Visit(object);

        m_ValidatedForControlPoints = m_ValidatedForControlPoints && isControlPointVisitor->Valid();
        m_ValidatedForAddingControlPoint = m_ValidatedForAddingControlPoint && canAddControlPointVisitor->Valid();
    }
}

std::shared_ptr<SceneObjectCAD> ManualSelectionSystem::First()
{
    auto& [id, object] = *m_SelectedObjects.begin();

    return object.lock();
}

bool ManualSelectionSystem::EligibleForIntersection() const
{
    if (m_SelectedObjects.empty() || m_SelectedObjects.size() > 2)
    {
        return false;
    }

    return std::all_of(m_SelectedObjects.begin(), m_SelectedObjects.end(),
        [this](std::pair<uint32_t, std::weak_ptr<SceneObjectCAD>> pair)
        {
            m_IntersectionEligibilityValidator->Visit(pair.second);

            return m_IntersectionEligibilityValidator->Valid();
        }
    );
}
