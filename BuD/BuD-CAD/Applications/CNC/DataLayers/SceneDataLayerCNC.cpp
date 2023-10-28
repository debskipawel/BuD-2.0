#include "SceneDataLayerCNC.h"

SceneDataLayerCNC::SceneDataLayerCNC()
	: m_Scene(), m_Material(m_Scene, MaterialBlockParameters::DEFAULT_PARAMETERS, 1024, 1024)
{
	auto camera = m_Scene.ActiveCamera();
	camera->Zoom(20.0f);
	camera->RotateCamera(-std::numbers::pi_v<float> / 4.0f, std::numbers::pi_v<float> / 6.0f);
}
