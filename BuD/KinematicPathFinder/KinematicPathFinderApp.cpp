#include "KinematicPathFinderApp.h"

KinematicPathFinderApp::KinematicPathFinderApp()
{
}

auto KinematicPathFinderApp::OnUpdate(float deltaTime) -> void
{
	return;
}

auto KinematicPathFinderApp::OnGuiRender() -> void
{
	return;
}

auto KinematicPathFinderApp::OnRender() -> void
{
	BuD::Renderer::Clear(0.2f, 0.5f, 0.6f, 1.0f);
}
