#pragma once

#include <Scene/SceneObjectCAD.h>

class BaseSelectionSystem
{
public:
	virtual ~BaseSelectionSystem() = default;

	virtual void Clear();

	virtual void Select(std::weak_ptr<SceneObjectCAD> sceneObject);

	virtual void Unselect(uint32_t sceneObjectId);
	virtual void Unselect(std::weak_ptr<SceneObjectCAD> sceneObject);

	virtual void ForEachSelected(std::function<void(std::shared_ptr<SceneObjectCAD>)> forEachHandler = [](std::shared_ptr<SceneObjectCAD>) {});

	inline virtual bool Selected(uint32_t sceneObjectId) const { return m_SelectedObjects.contains(sceneObjectId); }
	inline virtual size_t Count() const { return m_SelectedObjects.size(); }

	inline const auto& AllSelected() const { return m_SelectedObjects; }

protected:
	std::vector<uint32_t> m_SelectedOrder;
	std::unordered_map<uint32_t, std::weak_ptr<SceneObjectCAD>> m_SelectedObjects;
};
