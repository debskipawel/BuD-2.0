#pragma once

#include <Behaviors/Mouse/BaseMouseBehaviorLayer.h>
#include <Raycasting/RayFactory.h>

class SelectionMouseBehaviorLayer : public BaseMouseBehaviorLayer
{
public:
	SelectionMouseBehaviorLayer(MainViewModel& viewModel);

	virtual void OnLeftButtonDown(int x, int y);

protected:
	void HandleSelection(int x, int y);

	virtual void MoveCursorAlong(const Ray& ray);

	virtual std::shared_ptr<SceneObjectCAD> GetClosestIntersecting(const Ray& ray);
	virtual std::vector<std::shared_ptr<SceneObjectCAD>> GetAllIntersecting(const Ray& ray);
};