#include "ManualSelectionSystem.h"

#include <Visitors/Validation/ControlPointValidationVisitor.h>
#include <Visitors/Validation/CanAddControlPointValidationVisitor.h>

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
