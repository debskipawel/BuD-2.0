#pragma once

#include <Applications/CNC/Objects/Tools/MillingTool.h>
#include <Applications/CNC/Objects/MaterialBlock/MaterialBlockParameters.h>

class MaterialBlockCutter
{
public:
	MaterialBlockCutter(MaterialBlockParameters materialBlock, std::function<void(int, int, float)> putPixelHandler);

	virtual void MoveMillingTool(std::shared_ptr<MillingTool> millingTool, const dxm::Vector3& endPosition);

protected:
	virtual std::optional<std::pair<dxm::Vector3, dxm::Vector3>> ClipMillingToolLine(const dxm::Vector3& start, const dxm::Vector3& end, float radius);

	virtual void MoveMillingToolVertically(std::shared_ptr<MillingTool> millingTool, const dxm::Vector3& endPosition);
	virtual void MoveMillingToolHorizontally(std::shared_ptr<MillingTool> millingTool, const dxm::Vector3& startPosition, const dxm::Vector3& endPosition);

	virtual void CutCircle(std::shared_ptr<MillingTool> millingTool, const dxm::Vector3& endPosition);

	virtual dxm::Vector3 MapPixelToWorldSpace(uint32_t x, uint32_t y);
	virtual dxm::Vector2 MapWorldSpaceToPixelSpace(const dxm::Vector3& p);

	virtual dxm::Vector3 MapWorldSpaceToLocalMaterialSpace(const dxm::Vector3& p);
	virtual dxm::Vector2 MapLocalMaterialSpaceToPixelSpace(const dxm::Vector3& p);

	MaterialBlockParameters m_MaterialBlock;
	std::function<void(int, int, float)> m_PutPixelHandler;
};
