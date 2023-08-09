#pragma once

#include <Raycasting/RayFactory.h>
#include <ViewModels/MainViewModel.h>

class MouseBehaviorLayer
{
public:
	MouseBehaviorLayer(MainViewModel& viewModel);

	virtual void OnLeftButtonDown(int x, int y);
	virtual void OnRightButtonDown(int x, int y);
	virtual void OnMiddleButtonDown(int x, int y);

	virtual void OnLeftButtonUp(int x, int y);
	virtual void OnRightButtonUp(int x, int y);
	virtual void OnMiddleButtonUp(int x, int y);

	virtual void OnScroll(int x, int y, int delta);
	virtual void OnMouseMove(int dx, int dy);

protected:
	virtual bool IsMouseOnViewport(int x, int y);
	virtual dxm::Vector2 ViewportScreenSpaceCoords(int x, int y);

	bool m_MoveMouse;

	std::unique_ptr<RayFactory> m_RayFactory;
	MainViewModel& m_ViewModel;
};
