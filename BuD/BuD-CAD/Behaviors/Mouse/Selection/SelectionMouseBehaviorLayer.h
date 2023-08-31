#pragma once

#include <Behaviors/Mouse/BaseMouseBehaviorLayer.h>
#include <Raycasting/RayFactory.h>

class SelectionMouseBehaviorLayer : public BaseMouseBehaviorLayer
{
public:
	SelectionMouseBehaviorLayer(MainDataLayer& dataLayer);

	virtual void OnLeftButtonDown(int x, int y);

protected:
	void ClearSelected();
	void HandleSelection(int x, int y);

	virtual void MoveCursorAlong(const Ray& ray);

	virtual std::weak_ptr<SceneObjectCAD> GetClosestIntersecting(const Ray& ray);
	virtual std::vector<std::weak_ptr<SceneObjectCAD>> GetAllIntersecting(const Ray& ray);
};
