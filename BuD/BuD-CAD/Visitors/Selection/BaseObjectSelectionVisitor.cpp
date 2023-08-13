#include "BaseObjectSelectionVisitor.h"

#include <Visitors/Transform/ApplyTransformVisitor.h>

BaseObjectSelectionVisitor::BaseObjectSelectionVisitor(SceneDataLayer& sceneDataLayer)
	: m_SceneDataLayer(sceneDataLayer)
{
}

void BaseObjectSelectionVisitor::ApplyAction(std::shared_ptr<Action> action)
{
	auto& selectedGroup = m_SceneDataLayer.m_SelectedGroup;
	auto& scene = m_SceneDataLayer.m_SceneCAD;

	for (auto& id : action->m_CurrentlySelectedObjects)
	{
		auto& originalTransform = selectedGroup.m_InitialTransformCopies[id];
		auto& object = scene.m_ObjectList[id];

		std::unique_ptr<AbstractVisitor> visitor = std::make_unique<ApplyTransformVisitor>(
			originalTransform,
			action->m_TransformComponent,
			action->m_Centroid
		);

		visitor->Visit(object);

		selectedGroup.m_InitialTransformCopies[id] = object->m_Transform;
	}
}
