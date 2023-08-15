#include "ManualSelectionSystem.h"

#include <Visitors/Validation/ControlPointValidationVisitor.h>

void ManualSelectionSystem::Select(std::weak_ptr<SceneObjectCAD> sceneObject)
{
    if (m_SelectedObjects.empty())
    {
        m_ValidatedForControlPoints = true;
    }

    BaseSelectionSystem::Select(sceneObject);

    auto sceneObjectShared = sceneObject.lock();
    std::unique_ptr<BaseValidationVisitor> visitor = std::make_unique<ControlPointValidationVisitor>();

    visitor->Visit(sceneObject);
    m_ValidatedForControlPoints = m_ValidatedForControlPoints && visitor->Valid();
}

void ManualSelectionSystem::Unselect(uint32_t sceneObjectId)
{
    BaseSelectionSystem::Unselect(sceneObjectId);

    std::unique_ptr<BaseValidationVisitor> visitor = std::make_unique<ControlPointValidationVisitor>();

    m_ValidatedForControlPoints = !m_SelectedObjects.empty();

    for (auto& [id, object] : m_SelectedObjects)
    {
        auto objectShared = object.lock();

        visitor->Visit(object);
        m_ValidatedForControlPoints = m_ValidatedForControlPoints && visitor->Valid();
    }
}

std::shared_ptr<SceneObjectCAD> ManualSelectionSystem::First()
{
    auto& [id, object] = *m_SelectedObjects.begin();

    return object.lock();
}
