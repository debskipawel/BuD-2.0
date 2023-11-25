#pragma once

#include <SimpleMath.h>

#include <Applications/CAD/Scene/SceneCAD.h>

namespace dxm = DirectX::SimpleMath;

class MillingToolGenerator
{
public:
	MillingToolGenerator(SceneCAD& scene, const dxm::Vector3& materialSize, float modelPlaneHeight);

	virtual auto GeneratePaths() -> void;

protected:
	virtual auto GetSurfacesOnScene() -> std::vector<std::weak_ptr<SceneObjectCAD>>;

	SceneCAD& m_Scene;

	dxm::Vector3 m_MaterialSize;
	float m_ModelPlaneHeight;
};
