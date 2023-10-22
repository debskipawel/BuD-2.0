#pragma once

#include <Applications/CAD/Actions/TransformActionList.h>
#include <Applications/CAD/Selection/BaseSelectionSystem.h>

class TransformSelectionSystem : public BaseSelectionSystem
{
public:
	explicit TransformSelectionSystem() = default;
	explicit TransformSelectionSystem(const TransformSelectionSystem& other) = default;

	virtual void Clear() override;

	virtual void Select(std::weak_ptr<SceneObjectCAD> sceneObject) override;
	virtual void Unselect(uint32_t sceneObjectId) override;

	virtual TransformComponent InitialTransform(uint32_t objectId) const;
	virtual TransformComponent& GroupTransform();

	virtual dxm::Vector3 Centroid() const;

	virtual bool CanUndo();
	virtual bool CanRedo();

	virtual bool Undo();
	virtual bool Redo();

protected:
	TransformComponent m_GroupTransform;
	dxm::Vector3 m_Centroid;

	virtual std::shared_ptr<TransformAction> CreateAction();
	virtual void UpdateCentroid();

	std::unordered_map<uint32_t, TransformComponent> m_InitialTransforms;
	TransformActionList m_TransformActionList;
};