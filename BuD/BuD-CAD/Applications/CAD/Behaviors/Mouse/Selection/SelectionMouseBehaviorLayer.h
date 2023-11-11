#pragma once

#include <Applications/CAD/Behaviors/Mouse/BaseMouseBehaviorLayerCAD.h>
#include <Applications/CAD/Raycasting/RayFactory.h>

class SelectionMouseBehaviorLayer : public BaseMouseBehaviorLayerCAD
{
public:
	SelectionMouseBehaviorLayer(MainDataLayerCAD& dataLayer);

	virtual void OnLeftButtonDown(int x, int y);

protected:
	void ClearSelected();
	void HandleSelection(int x, int y);

	virtual void MoveCursorAlong(const Ray& ray);

	virtual std::weak_ptr<SceneObjectCAD> GetClosestIntersecting(const Ray& ray);
	virtual std::vector<std::weak_ptr<SceneObjectCAD>> GetAllIntersecting(const Ray& ray);
};
