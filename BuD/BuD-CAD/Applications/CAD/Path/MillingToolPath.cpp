#include "MillingToolPath.h"

MillingToolPath::MillingToolPath(float toolRadius, const std::vector<dxm::Vector3>& path)
	: m_MillingToolRadius(toolRadius), m_Path(path)
{
}

MillingToolPath::MillingToolPath(float toolRadius, std::vector<dxm::Vector3>&& path)
	: m_MillingToolRadius(toolRadius), m_Path(path)
{
}

auto MillingToolPath::GetPath() const -> const std::vector<dxm::Vector3>&
{
	return m_Path;
}

auto MillingToolPath::GetToolRadius() const -> float
{
	return m_MillingToolRadius;
}
