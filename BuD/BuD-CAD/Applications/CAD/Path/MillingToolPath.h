#pragma once

#include <BuD.h>

class MillingToolPath
{
public:
	MillingToolPath(float toolRadius, const std::vector<dxm::Vector3>& path);
	MillingToolPath(float toolRadius, std::vector<dxm::Vector3>&& path);

	virtual auto GetPath() const -> const std::vector<dxm::Vector3>&;
	virtual auto GetToolRadius() const -> float;

protected:
	float m_MillingToolRadius;

	std::vector<dxm::Vector3> m_Path;
};
